CROSS_TOOL ?= arm-none-eabi-

CC = $(CROSS_TOOL)gcc
LD = $(CROSS_TOOL)ld
OC = $(CROSS_TOOL)objcopy

ARCH ?= stm32f303

TARGET ?= omfk

SERIAL_COMMUNICATION ?= minicom -D
SERIAL_DEVICE ?= /dev/ttyACM0

LD_SCRIPT = scripts/ld/$(ARCH).ld

FLASH_TOOL = st-flash
FLASH_ADDR ?= 0x8000000

ARCH_DIR = arch/$(ARCH)
ARCH_SRC_DIR = $(ARCH_DIR)/src
ARCH_INC_DIR = $(ARCH_DIR)/inc

KERNEL_DIR = kernel
KERNEL_SRC_DIR = $(KERNEL_DIR)/src
KERNEL_INC_DIR = $(KERNEL_DIR)/inc

BUILD_DIR ?= build

CC_FLAGS = -mcpu=cortex-m4 -mthumb -g -ffreestanding -std=gnu99
LD_FLAGS = -T $(LD_SCRIPT) --cref -Map $(BUILD_DIR)/$(TARGET).map -nostartfiles -nostdlib

ARCH_OBJS = usart.o clock.o kernel_entry.o dac.o timer.o dma.o gpio.o
KERNEL_OBJS = kernel.o terminal.o string.o

.PHONY: all
all : pre-build $(TARGET).bin

.PHONY: test
test : flash
	$(SERIAL_COMMUNICATION) $(SERIAL_DEVICE)

.PHONY: flash
flash : $(TARGET).bin
	$(FLASH_TOOL) write $(BUILD_DIR)/$(TARGET).bin $(FLASH_ADDR) &> /dev/null

$(TARGET).bin : $(TARGET).elf 
	$(OC) -O binary $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

$(TARGET).elf : $(ARCH_OBJS) $(KERNEL_OBJS)
	$(LD) $(BUILD_DIR)/$(ARCH_DIR)/*.o $(BUILD_DIR)/$(KERNEL_DIR)/*.o $(LD_FLAGS) -o $(BUILD_DIR)/$(TARGET).elf

kernel.o: $(KERNEL_SRC_DIR)/kernel.c pre-build
	$(CC) -c $(CC_FLAGS) $(KERNEL_SRC_DIR)/kernel.c -o $(BUILD_DIR)/$(KERNEL_DIR)/kernel.o -I $(KERNEL_INC_DIR) -I $(ARCH_INC_DIR)

terminal.o: $(KERNEL_SRC_DIR)/terminal.c pre-build
	$(CC) -c $(CC_FLAGS) $(KERNEL_SRC_DIR)/terminal.c -o $(BUILD_DIR)/$(KERNEL_DIR)/terminal.o -I $(KERNEL_INC_DIR) -I $(ARCH_INC_DIR)

string.o: $(KERNEL_SRC_DIR)/string.c pre-build
	$(CC) -c $(CC_FLAGS) $(KERNEL_SRC_DIR)/string.c -o $(BUILD_DIR)/$(KERNEL_DIR)/string.o -I $(KERNEL_INC_DIR)

kernel_entry.o: $(ARCH_SRC_DIR)/kernel_entry.c
	$(CC) -c $(CC_FLAGS) $(ARCH_SRC_DIR)/kernel_entry.c -o $(BUILD_DIR)/$(ARCH_DIR)/kernel_entry.o -I $(ARCH_INC_DIR) -I $(KERNEL_INC_DIR)

usart.o: $(ARCH_SRC_DIR)/usart.c pre-build
	$(CC) -c $(CC_FLAGS) $(ARCH_SRC_DIR)/usart.c -o $(BUILD_DIR)/$(ARCH_DIR)/usart.o -I $(ARCH_INC_DIR)

clock.o: $(ARCH_SRC_DIR)/clock.c pre-build
	$(CC) -c $(CC_FLAGS) $(ARCH_SRC_DIR)/clock.c -o $(BUILD_DIR)/$(ARCH_DIR)/clock.o -I $(ARCH_INC_DIR)

dac.o: $(ARCH_SRC_DIR)/dac.c pre-build
	$(CC) -c $(CC_FLAGS) $(ARCH_SRC_DIR)/dac.c -o $(BUILD_DIR)/$(ARCH_DIR)/dac.o -I $(ARCH_INC_DIR)

timer.o: $(ARCH_SRC_DIR)/timer.c pre-build
	$(CC) -c $(CC_FLAGS) $(ARCH_SRC_DIR)/timer.c -o $(BUILD_DIR)/$(ARCH_DIR)/timer.o -I $(ARCH_INC_DIR)

dma.o: $(ARCH_SRC_DIR)/dma.c pre-build
	$(CC) -c $(CC_FLAGS) $(ARCH_SRC_DIR)/dma.c -o $(BUILD_DIR)/$(ARCH_DIR)/dma.o -I $(ARCH_INC_DIR)

gpio.o: $(ARCH_SRC_DIR)/gpio.c pre-build
	$(CC) -c $(CC_FLAGS) $(ARCH_SRC_DIR)/gpio.c -o $(BUILD_DIR)/$(ARCH_DIR)/gpio.o -I $(ARCH_INC_DIR)

pre-build:
	mkdir -p $(BUILD_DIR)/{$(ARCH_DIR),$(KERNEL_DIR)}

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) &> /dev/null
