include vars.mk

.PHONY: all
all : $(TARGET_BINARY)

.PHONY: test
test : flash
	@echo "[OPENNING] debug tty \"$(SERIAL_DEVICE)\"..."
	@$(SERIAL_COMMUNICATION) $(SERIAL_DEVICE)

.PHONY: flash
flash : $(TARGET_BINARY)
	@echo ""
	@echo "[FLASHING] kernel binary \"$(TARGET_BINARY)\"..."
	@$(FLASH_TOOL) write $(TARGET_BINARY) $(FLASH_ADDR) &> /dev/null
	@echo ""

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

$(TARGET_BINARY) : pre-build $(OBJS)
	@echo "[BUILDING] kernel binary \"$(TARGET_BINARY)\""
	$(LD) $(OBJS) $(LD_FLAGS) -o $(BUILD_DIR)/$(TARGET).elf
	$(OC) -O binary $(BUILD_DIR)/$(TARGET).elf $(TARGET_BINARY)
	@echo "[BUILT]    \"$(TARGET_BINARY)\""

dump : $(TARGET_BINARY)
	@echo "Dumping kernel listing $(TARGET_DUMP)"
	$(OD) -d $(BUILD_DIR)/$(TARGET).elf > $(TARGET_DUMP)

$(OBJS) : $(SRCS)
	@$(BUILD_SCRIPT) '$(CC)' '$(CC_FLAGS)' '$(SRCS)' '$(OBJS)'

pre-build:
	@mkdir -p $(BUILD_DIR)
