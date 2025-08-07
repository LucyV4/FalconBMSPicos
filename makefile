BUILD_DIR ?= build
FIRMWARE_DIR ?= firmware
TARGET ?= ewpi

CMFLAGS += --fresh
CMFLAGS += --log-level WARNING
CMFLAGS += -Wno-deprecated

.PHONY: FORCE

all: FORCE compile

$(BUILD_DIR)/build.ninja: CMakeLists.txt
	@mkdir -p $(BUILD_DIR) $(FIRMWARE_DIR)
	@cmake -B $(BUILD_DIR) -G Ninja $(CMFLAGS)

compile: $(BUILD_DIR)/build.ninja FORCE
	@ninja -C $(BUILD_DIR)

clean: FORCE
	$(RM) -r $(BUILD_DIR) $(FIRMWARE_DIR)

flash: FORCE compile
	@./helper/pico_off.exe $(TARGET)
	@picotool load firmware/$(TARGET).uf2
	@picotool reboot