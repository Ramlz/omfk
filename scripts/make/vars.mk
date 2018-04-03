include scripts/make/user_vars.mk

CC                      := $(CROSS_TOOL)gcc
LD                      := $(CROSS_TOOL)ld
OC                      := $(CROSS_TOOL)objcopy
OD                      := $(CROSS_TOOL)objdump

ARCH                    := arm
CORE                    := m4
PLATFORM                := stm32
PLATFORM_SERIES         := f3

FLASH_ADDR              := 0x8000000

SCRIPT_DIR              := scripts
BUILD_DIR               := build-$(BOARD)

TARGET                  := omfk
TARGET_BINARY           := $(BUILD_DIR)/$(TARGET).bin
TARGET_MAP              := $(BUILD_DIR)/$(TARGET).map
TARGET_DUMP             := $(BUILD_DIR)/$(TARGET).dump

SERIAL_COMMUNICATION    := minicom -o -D

FLASH_TOOL              := st-flash

CORE_DIR                := arch/$(ARCH)/$(CORE)
PLATFORM_DIR            := platform/$(PLATFORM)
KERNEL_DIR              := kernel
COMMON_DIR              := common
LIB_DIR                 := lib
DRIVER_DIR              := driver
UTILS_DIR               := utils
BOARD_DIR               := board/$(BOARD)
PEONS_DIR               := peons

ALL_DIR                 := $(KERNEL_DIR) $(COMMON_DIR) $(PLATFORM_DIR) \
                           $(DRIVER_DIR) $(LIB_DIR) $(CORE_DIR) $(UTILS_DIR) \
                           $(PEONS_DIR) $(BOARD_DIR)
INC_DIR                 := -I $(BUILD_DIR)

BUILD_SCRIPT            := $(SCRIPT_DIR)/sh/build.sh
LD_SCRIPT               := $(SCRIPT_DIR)/ld/$(BOARD).ld

CC_FLAGS                := -mcpu=cortex-$(CORE) -mthumb -g -ffreestanding \
                           -std=gnu99 $(INC_DIR) -fomit-frame-pointer -Werror \
                           -Wall -Wextra -mfloat-abi=hard -mapcs-frame \
                           -mlittle-endian
LD_FLAGS                := -T $(LD_SCRIPT) --cref \
                           -Map $(TARGET_MAP) -nostartfiles -nostdlib

SRCS                    := $(shell find $(ALL_DIR) -name '*.c')
OBJS                    := $(foreach obj, $(shell find \
                           $(ALL_DIR) -name '*.c' | sed -e 's/\/src\//\//g' \
                           | sed -e 's/\/$(ARCH)\//\//g' \
                           | sed -e 's/\/$(CORE)\//\//g' \
                           | sed -e 's/\/$(PLATFORM)\//\//g' \
                           | sed -e 's/\/$(PLATFORM_SERIES)\//\//g' \
                           | sed -e 's/\/$(BOARD)\//\//g' \
                           | sed -e 's/\.c/\.o/g'), $(BUILD_DIR)/$(obj))

INCS                    := $(shell find $(ALL_DIR) -name '*.h')
PREPROCESSED_INCS       := $(shell echo $(INCS) \
                           | sed -e 's/\/inc\//\//g' \
                           | sed -e 's/\/$(ARCH)\//\//g' \
                           | sed -e 's/\/$(CORE)\//\//g' \
                           | sed -e 's/\/$(PLATFORM)\//\//g' \
                           | sed -e 's/\/$(PLATFORM_SERIES)\//\//g' \
                           | sed -e 's/\/$(BOARD)\//\//g')
TARGET_INCS             := $(foreach header, $(PREPROCESSED_INCS) \
                           , $(BUILD_DIR)/$(header))
