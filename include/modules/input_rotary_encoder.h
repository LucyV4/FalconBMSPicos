#pragma once
#include "input_module.h"
#include "flightdata.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include <cstdint>
#include <functional>
#include <vector>

enum class ButtonMode {
	ActiveLow,
	ActiveHigh,
	Toggle
};

class RotarySwitchInput : public InputModule {
public:

	explicit RotarySwitchInput(uint8_t gpio_pin_1, uint8_t gpio_pin_2, uint8_t gpio_pin_3, std::function<void(PicoData&, bool, bool, bool)> update_function) : InputModule(gpio_pin_1), gpio_pin_1(gpio_pin_1), gpio_pin_2(gpio_pin_2), gpio_pin_3(gpio_pin_3) {}

	void setup() override {
		gpio_init(gpio_pin_1);
		gpio_set_dir(gpio_pin_1, GPIO_IN);
		gpio_set_pulls(gpio_pin_1, true, false);

		gpio_init(gpio_pin_2);
		gpio_set_dir(gpio_pin_2, GPIO_IN);
		gpio_set_pulls(gpio_pin_2, true, false);

		gpio_init(gpio_pin_3);
		gpio_set_dir(gpio_pin_3, GPIO_IN);
		gpio_set_pulls(gpio_pin_3, true, false);
	}

	void update(PicoData& data) override {
		bool pin_1_on = gpio_get(gpio_pin_1);
		bool pin_2_on = gpio_get(gpio_pin_2);
		bool pin_3_on = gpio_get(gpio_pin_3);

		updateFunc(data, pin_1_on, pin_2_on, pin_3_on);
		return;
	}

private:
	uint8_t gpio_pin_1;
	uint8_t gpio_pin_2;
	uint8_t gpio_pin_3;
	std::function<void(PicoData&, bool, bool, bool)> updateFunc;
};