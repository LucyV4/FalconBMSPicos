#pragma once 

#include "f16data.h"
#include <cstdint>

class OutputModule {
public:
	OutputModule(uint8_t gpio) : gpio_pin(gpio) {}

	virtual void setup();
	virtual void update(F16Data& data);
	uint8_t get_gpio() { return gpio_pin; };
	virtual ~OutputModule();
protected:
	uint8_t gpio_pin;
};