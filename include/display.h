#pragma once

#include <string.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <stdarg.h>

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/time.h"

#include "font5x7.h"

#define MSBFIRST 1
#define LSBFIRST 0
#define LEDDISPLAY_MAXCHARS  32

class Display {
	uint8_t buffer[32];
	public:
		Display() {}
		// constructor:
		Display(uint8_t _dataPin, uint8_t _registerSelect, uint8_t _clockPin, uint8_t _chipEnable, uint8_t _resetPin, uint8_t _blankPin, uint8_t _displayLength) {
			// Define pins for the LED display:
			this->dataPin = _dataPin;		 			// connects to the display's data in
			this->registerSelect = _registerSelect;   	// the display's register select pin
			this->clockPin = _clockPin;					// the display's clock pin
			this->chipEnable = _chipEnable;	   		// the display's chip enable pin
			this->resetPin = _resetPin;		 		// the display's reset pin
			this->blankPin = _blankPin;		 		// the display's reset pin
			this->displayLength = _displayLength;		// number of bytes needed to pad the string

			// do not allow a long multiple display to use more than LEDDISPLAY_MAXCHARS
			if (_displayLength > LEDDISPLAY_MAXCHARS) {
				_displayLength = LEDDISPLAY_MAXCHARS;
			}

			this->clear();

			gpio_init(dataPin);
			gpio_set_dir(dataPin, GPIO_OUT);
			gpio_init(registerSelect);
			gpio_set_dir(registerSelect, GPIO_OUT);
			gpio_init(clockPin);
			gpio_set_dir(clockPin, GPIO_OUT);
			gpio_init(chipEnable);
			gpio_set_dir(chipEnable, GPIO_OUT);
			gpio_init(resetPin);
			gpio_set_dir(resetPin, GPIO_OUT);

			// Sets PWM for brightness control on leds
			gpio_set_function(blankPin, GPIO_FUNC_PWM);
			this->sliceNum = pwm_gpio_to_slice_num(blankPin);
			this->channel = pwm_gpio_to_channel(blankPin);

			pwm_set_wrap(sliceNum, 255);
			if (channel == PWM_CHAN_A) {
				pwm_set_output_polarity(sliceNum, true, false);
			} else {
			    pwm_set_output_polarity(sliceNum, false, true);
			}

			pwm_set_chan_level(sliceNum, channel, 255);
			pwm_set_enabled(sliceNum, true);

			gpio_put(resetPin, 0);
			sleep_ms(10);
			gpio_put(resetPin, 1);
			
			loadDotRegister();

			loadDotRegister();
			loadAllControlRegisters(0b01111111);
			loadDotRegister();
		}

		void clear() {
			for (int displayPos = 0; displayPos < displayLength; displayPos++) {
				writeCharacter(' ', displayPos);
			}

			// send the dot register array out to the display:
			loadDotRegister();
		}

		void write(char character, uint8_t position) {
			this->writeCharacter(character, position);
		}

		void writef(uint8_t fromPosition, const char *str, ...) {
			char temp_str[LEDDISPLAY_MAXCHARS+1] = {};
			va_list args;
  			va_start (args, str);
			vsnprintf(temp_str, LEDDISPLAY_MAXCHARS, str, args);
			va_end(args);

			const char *out = temp_str;

			int8_t charPos = 0;
			while (*out != '\0') {
				this->writeCharacter(*out, fromPosition+charPos);
				out++;
				charPos++;
			}
		}

		void update() {
			loadDotRegister();
		}

		void setBrightness(uint8_t brightness) {
			uint8_t pwm_brightness = (uint16_t(brightness) * 255) / 15;
			pwm_set_chan_level(sliceNum, channel, pwm_brightness);
		}
	
	private:
		void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
			for (uint8_t i = 0; i < 8; i++) {
				bool data = (bitOrder == LSBFIRST) ? !!(val & (1 << i)) : (val & (1 << (7 - i))); 
				// gpio_put(25, data); // This gives a funny half-lit gpio 25 as it flickers on and off very fast 
				gpio_put(dataPin, data);

				gpio_put(clockPin, 1); // Clock pulse high
				sleep_us(10); // Short delay
				gpio_put(clockPin, 0); // Clock pulse low
			}
		}

  		// Character display setters:
		void writeCharacter(char character, uint8_t position) {
			uint8_t startChar = Font5x7[0]; // ASCII start offset (0x01)
			uint16_t thisPosition = position * 5;
			uint16_t fontChar = ((int)character) - startChar;

			for (int i = 0; i < 5; i++) {
			    uint16_t fontCharByte = 5 + fontChar * 5 + i; // +5 skips metadata
			    dotRegister[thisPosition + i] = Font5x7[fontCharByte];
			}
		}

		void loadDotRegister() {
  			// define max data to send, patch for 4 length displays by KaR]V[aN
  			int maxData = displayLength * 5;

  			// select the dot register:
  			gpio_put(registerSelect, 0);
  			// enable writing to the display:
  			gpio_put(chipEnable, 0);
  			// shift the data out:
  			for (int i = 0; i < maxData; i++) {
  				shiftOut(dataPin, clockPin, MSBFIRST, dotRegister[i]);
  			}
  			// disable writing:
  			gpio_put(chipEnable, 1);
		}

		void loadControlRegister(uint8_t dataByte) {
			// select the control registers:
  			gpio_put(registerSelect, 1);
			// enable writing to the display:
			gpio_put(chipEnable, 0);
			// shift the data out:
			shiftOut(dataPin, clockPin, MSBFIRST, dataByte);
			// disable writing:
			gpio_put(chipEnable, 1);
		}

		void loadAllControlRegisters(uint8_t dataByte) {
			int chip_count = displayLength/4;

			for (int i = 0; i < chip_count; i++) {
				loadControlRegister(0b10000001);
				sleep_us(1);
  			}

  			// Load the specified value into the control register.
  			loadControlRegister(dataByte);

			// Put all the chips back into serial mode. Because they're still
			// all in simultaneous mode, we only have to write this word once.
  			loadControlRegister(0b10000000);
			writef(15, "%d", chip_count);
			update();
		}

		uint8_t dotRegister[LEDDISPLAY_MAXCHARS * 5]; // 5 bytes per character * maxchars

		// Define pins for the LED display:
		uint8_t dataPin;			// connects to the display's data in
		uint8_t registerSelect;		// the display's register select pin
		uint8_t clockPin;			// the display's clock pin
		uint8_t chipEnable;			// the display's chip enable pin
		uint8_t resetPin;			// the display's reset pin
		uint8_t blankPin;			// the display's blank pin
		uint8_t displayLength;		// number of bytes needed to pad the string
		uint8_t sliceNum;
		uint8_t channel;
};