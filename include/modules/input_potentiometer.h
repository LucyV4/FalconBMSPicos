#pragma once
#include "input_module.h"
#include "flightdata.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "bsp/board.h"


class PotInput : public InputModule {
public:

	explicit PotInput(const uint8_t gpio_pin, const uint8_t adc_num, std::function<void(const F16Data&, uint16_t)> update_function) : InputModule(gpio_pin), adcNum(adc_num), updateFunc(update_function) {}
	explicit PotInput(const uint8_t gpio_pin, const uint8_t adc_num, uint8_t controller_throttle_index) : InputModule(gpio_pin), adcNum(adc_num) {
		updateFunc = [controller_throttle_index](F16Data& data, uint16_t raw_value) { data.setThrottle(controller_throttle_index, raw_value); };
	}

	void setup() override {
		adc_init();
		adc_gpio_init(gpio_pin);
	}

	void update(F16Data& data) override {
		const uint16_t maxValue = 4095;
		const uint16_t minChange = 20;
		
		adc_select_input(adcNum);
		rawValue = adc_read();
		
		updateFunc(data, rawValue);

		return;
	}

private:
	uint8_t adcNum; // 0, 1, 2
	int16_t rawValue = 0;

	std::function<void(F16Data&, uint16_t)> updateFunc;
};