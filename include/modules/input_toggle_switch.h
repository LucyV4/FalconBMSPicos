#pragma once
#include "input_module.h"
#include "flightdata.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include <cstdint>
#include <functional>

enum class ButtonMode {
	ActiveLow,
	ActiveHigh,
	Toggle
};

class ToggleSwitchInput : public InputModule {
public:

	explicit ToggleSwitchInput(uint8_t gpio_pin1, uint8_t gpio_pin2, std::function<void(PicoData&, bool, bool, bool)> update_function) : InputModule(gpio_pin), gpio_pin_1(gpio_pin1), gpio_pin_2(gpio_pin2), updateFunc(update_function) {}

	void setup() override {
		gpio_init(gpio_pin_1);
		gpio_set_dir(gpio_pin_1, GPIO_IN);
		gpio_set_pulls(gpio_pin_1, true, false);
		
		gpio_init(gpio_pin_2);
		gpio_set_dir(gpio_pin_2, GPIO_IN);
		gpio_set_pulls(gpio_pin_2, true, false);
	}

	void update(PicoData& data) override {
		bool input_1 = gpio_get(gpio_pin_1);
		bool input_3 = gpio_get(gpio_pin_2);
		bool input_2 = (!input_1 && !input_3);

		// Some logic
		updateFunc(data, input_1, input_2, input_3);

		return;
	}

private:
	uint8_t gpio_pin_1;
	uint8_t gpio_pin_2;
	std::function<void(PicoData&, bool, bool, bool)> updateFunc;
};