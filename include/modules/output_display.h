#pragma once
#include "output_module.h"
#include "flightdata.h"
#include "display.h"
#include <functional>

class DisplayOutput : public OutputModule {
public:
	explicit DisplayOutput(
		uint8_t data_pin,
		uint8_t register_select,
		uint8_t clock_pin,
		uint8_t enable_pin,
		uint8_t reset_pin,
		uint8_t blank_pin,
		uint8_t display_length,
		std::function<void(F16Data&, Display&)> update_function
	) : OutputModule(data_pin), display(data_pin, register_select, clock_pin, enable_pin, reset_pin, blank_pin, display_length), updateFunc(update_function) {}

	void setup() override {
		// Nothing, serup gets done at initialization
	}

	void update(F16Data& data) override {
		updateFunc(data, display);
		display.update();
	}

private:
	Display display;
	std::function<void(F16Data&, Display&)> updateFunc;
};