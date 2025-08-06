#pragma once
#include "output_module.h"
#include "flightdata.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include <functional>

// Structure for blinking leds
struct BlinkState {
	bool blink_on;
	uint32_t start_blink_ms;
};

class LedOutput : public OutputModule {
public:
	// Basic constructor where function is self-specified
	explicit LedOutput(uint8_t gpio_pin, std::function<bool(F16Data&)> led_on_function) : OutputModule(gpio_pin), compLedOutFunction(led_on_function) {}

	// 3 COnstructors (each of the lightbit enums) for standard lights (on/off)
	explicit LedOutput(uint8_t gpio_pin, FlightData::LightBits light_bit, bool inverted = false) : OutputModule(gpio_pin) {
		compLedOutFunction = [light_bit, inverted](F16Data& data) {
			bool is_on = data.flightData.IsSet(light_bit);
			if (inverted) is_on = !is_on;
			return is_on;
		};
	}
	explicit LedOutput(uint8_t gpio_pin, FlightData::LightBits2 light_bit, bool inverted = false) : OutputModule(gpio_pin) {
		compLedOutFunction = [light_bit, inverted](F16Data& data) {
			bool is_on = data.flightData.IsSet2(light_bit);
			if (inverted) is_on = !is_on;
			return is_on;
		};
	}
	explicit LedOutput(uint8_t gpio_pin, FlightData::LightBits3 light_bit, bool inverted = false) : OutputModule(gpio_pin) {
		compLedOutFunction = [light_bit, inverted](F16Data& data) {
			bool is_on = data.flightData.IsSet3(light_bit);
			if (inverted) is_on = !is_on;
			return is_on;
		};
	}

	// 3 Constructors (each of the lightbit enums) for blinking lights (on/off with blinking)
	explicit LedOutput(uint8_t gpio_pin, FlightData::LightBits light_bit, FlightData2::BlinkBits blink_bit, uint8_t blink_ms, bool inverted = false) : OutputModule(gpio_pin) {
		auto blink_state = std::make_shared<BlinkState>();
		blink_state->blink_on = true;
		blink_state->start_blink_ms = 0;

		compLedOutFunction = [light_bit, blink_bit, blink_ms, inverted, blink_state](F16Data& data) { 
			bool is_on = data.flightData.IsSet(light_bit); 
			if (inverted) is_on = !is_on;
			bool is_blinking = data.flightData2.IsSetBlink(blink_bit);

			if (board_millis() - blink_state->start_blink_ms >= blink_ms) {
				blink_state->start_blink_ms += blink_ms;
				blink_state->blink_on = !blink_state->blink_on;
			}

			return  ((is_on && !is_blinking) || (is_on && is_blinking && blink_state->blink_on));
		};
	}
	explicit LedOutput(uint8_t gpio_pin, FlightData::LightBits2 light_bit, FlightData2::BlinkBits blink_bit, uint8_t blink_ms, bool inverted = false) : OutputModule(gpio_pin) {
		auto blink_state = std::make_shared<BlinkState>();
		blink_state->blink_on = true;
		blink_state->start_blink_ms = 0;

		compLedOutFunction = [light_bit, blink_bit, blink_ms, inverted, blink_state](F16Data& data) { 
			bool is_on = data.flightData.IsSet2(light_bit); 
			if (inverted) is_on = !is_on;
			bool is_blinking = data.flightData2.IsSetBlink(blink_bit);

			if (board_millis() - blink_state->start_blink_ms >= blink_ms) {
				blink_state->start_blink_ms += blink_ms;
				blink_state->blink_on = !blink_state->blink_on;
			}

			return  ((is_on && !is_blinking) || (is_on && is_blinking && blink_state->blink_on));
		};
	}
	explicit LedOutput(uint8_t gpio_pin, FlightData::LightBits3 light_bit, FlightData2::BlinkBits blink_bit, uint8_t blink_ms, bool inverted = false) : OutputModule(gpio_pin) {
		auto blink_state = std::make_shared<BlinkState>();
		blink_state->blink_on = true;
		blink_state->start_blink_ms = 0;

		compLedOutFunction = [light_bit, blink_bit, blink_ms, inverted, blink_state](F16Data& data) { 
			bool is_on = data.flightData.IsSet3(light_bit);
			if (inverted) is_on = !is_on;
			bool is_blinking = data.flightData2.IsSetBlink(blink_bit);

			if (board_millis() - blink_state->start_blink_ms >= blink_ms) {
				blink_state->start_blink_ms += blink_ms;
				blink_state->blink_on = !blink_state->blink_on;
			}

			return ((is_on && !is_blinking) || (is_on && is_blinking && blink_state->blink_on));
		};
	}

	void setup() override {
		gpio_init(gpio_pin);
		gpio_set_dir(gpio_pin, GPIO_OUT);
	}

	void update(F16Data& data) override {
		bool out = compLedOutFunction(data);
		gpio_put(gpio_pin, out);
	}
private:
	std::function<bool(F16Data&)> compLedOutFunction;
};

