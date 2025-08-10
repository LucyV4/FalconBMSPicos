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

	explicit RotarySwitchInput(std::vector<uint8_t> gpio_pins, std::function<void(PicoData&, uint8_t)> update_function) : InputModule(gpio_pins[0]), gpio_pins(gpio_pins) {}

	void setup() override {
		for (uint8_t gpio_pin : gpio_pins) {
			gpio_init(gpio_pin);
			gpio_set_dir(gpio_pin, GPIO_IN);
			gpio_set_pulls(gpio_pin, true, false);
		}
	}

	void update(PicoData& data) override {
		switch_selected = 0;
		for (uint8_t gpio_pin : gpio_pins)
		{
			bool is_on = gpio_get(gpio_pin);
			if (is_on)
				break;
			switch_selected++;
		}

		updateFunc(data, switch_selected);
		return;
	}

private:
	uint8_t switch_selected = 0;
	std::vector<uint8_t> gpio_pins;
	std::function<void(PicoData&, uint8_t)> updateFunc;
};