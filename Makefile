include Makefile.in

.PHONY: all
all : $(TARGET_BINARY)

.PHONY: test
test : flash
	@echo 'Openning debug tty...'
	@$(SERIAL_COMMUNICATION) $(SERIAL_DEVICE)

.PHONY: flash
flash : $(TARGET_BINARY)
	@echo 'Flasing kernel binary "$(TARGET_BINARY)"...'
	@$(FLASH_TOOL) write $(TARGET_BINARY) $(FLASH_ADDR) &> /dev/null

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

$(TARGET_BINARY) : pre-build $(OBJS)
	@echo 'Building kernel binary $(TARGET_BINARY)'
	$(LD) $(OBJS) $(LD_FLAGS) -o $(BUILD_DIR)/$(TARGET).elf
	$(OC) -O binary $(BUILD_DIR)/$(TARGET).elf $(TARGET_BINARY)

$(OBJS) : $(SRCS)
	@$(BUILD_SCRIPT) '$(CC)' '$(CC_FLAGS)' '$(SRCS)' '$(OBJS)'

pre-build:
	@mkdir -p $(BUILD_DIR)
