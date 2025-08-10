#pragma once
#include "input_module.h"
#include "flightdata.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include <cstdint>

enum class ButtonMode {
	ActiveLow,
	ActiveHigh,
	Toggle
};

class ButtonInput : public InputModule {
public:

	explicit ButtonInput(uint8_t gpio_pin, std::function<void(PicoData&, bool)> update_function, ButtonMode button_mode = ButtonMode::ActiveLow) : InputModule(gpio_pin), buttonMode(button_mode), updateFunc(update_function) {}
	explicit ButtonInput(uint8_t gpio_pin, uint8_t controller_button_index, ButtonMode button_mode = ButtonMode::ActiveLow) : InputModule(gpio_pin), buttonMode(button_mode) {
		updateFunc = [controller_button_index](PicoData& data, bool button_active) { data.setButton(controller_button_index, button_active); };
	}

	void setup() override {
		gpio_init(gpio_pin);
		gpio_set_dir(gpio_pin, GPIO_IN);
		gpio_set_pulls(gpio_pin, true, false);
	}

	void update(PicoData& data) override {
		bool raw_input = gpio_get(gpio_pin);

		// Check for button input 
		switch (buttonMode) {
			case ButtonMode::ActiveLow:
				is_pressed = !raw_input;
				break;
			case ButtonMode::ActiveHigh:
				is_pressed = raw_input;
				break;
			case ButtonMode::Toggle:
				if ((last_raw_input != raw_input) && !raw_input) is_pressed = 1-is_pressed;
				last_raw_input = raw_input;
				break;
		}

		// Some logic
		updateFunc(data, is_pressed);

		return;
	}

private:
	bool last_raw_input = 1;
	bool is_pressed = 0;
	ButtonMode buttonMode;
	std::function<void(PicoData&, bool)> updateFunc;
};