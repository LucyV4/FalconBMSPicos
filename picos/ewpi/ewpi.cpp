// ewpi.cpp
#include "pico_device.h"
#include "output_led.h"
#include "output_display.h"
#include "input_button.h"
#include "input_potentiometer.h"
#include "uart_module.h"
#include "pico_data.h"

enum GpioPins: uint8_t {
	EWMUTX = 0,		// EWPI to EWMU uart TX pin
	EWMURX = 1, 	// EWPI to EWMU uart RX pin

	LNKLED = 6,		// LNK led
	LNCHLED = 7,	// LNCH led
	PRILED = 8,		// PRI led
	OPNLED = 9,		// OPN led

	PRIBTN = 12,	// PRI button
	SEPBTN = 13,	// SEP button
	UNKBTN = 14,	// UNK button
	MODEBTN = 15,	// MODE button

	DISBLANK = 16,	// Display Blank (can go?)
	DISCE = 17,		// Display chip enable
	DISCLOCK = 18,	// Display clock
	DISDATAIN = 19,	// Display data in
	DISRESET = 20,	// Display Reset
	DISRS = 21,		// Display Register Select

	BRTPOT = 28,	// BRT pot
	BRTADC = 2,		// BRT pot adc num
};

PicoDevice myPico("EWPI");

int main() {
	/*
	 ** EXAMPLE OUTPUT MODULES
	 */
	myPico.add_output(new LedOutput(LNKLED, FlightData::LightBits2::Unk));
	myPico.add_output(new LedOutput(LNCHLED, FlightData::LightBits2::Launch, FlightData2::BlinkBits::Launch, 250));
	myPico.add_output(new LedOutput(PRILED, FlightData::LightBits2::PriMode, FlightData2::BlinkBits::PriMode, 100));
	myPico.add_output(new LedOutput(OPNLED, FlightData::LightBits2::PriMode, FlightData2::BlinkBits::PriMode, 100, true));

	std::function<void(PicoData&, Display&)> ewpiDisplayFunc;
	ewpiDisplayFunc = [](PicoData& data, Display& display) {
		uint8_t chaffCount = data.flightData.ChaffCount;
		uint8_t flareCount = data.flightData.FlareCount;


		uint8_t CMDSMode = static_cast<uint8_t>(data.flightData2.cmdsMode);

		switch(CMDSMode) {
			case 0:
				//display.clear();
				break;
			case 1:
				display.write(CHAR_STBY, 4);
				break;
			case 2:
				display.write(CHAR_MAN, 4);
				break;
			case 3:
				display.write(CHAR_SEMI, 4);
				break;
			case 4:
				display.write(CHAR_AUTO, 4);
				break;
		}


		uint8_t ecmOn = data.flightData.IsSet2(FlightData::LightBits2::Degr);
		uint8_t ECMMode = static_cast<uint8_t>(data.flightData2.ecmOper);

		if (ecmOn == 0) {
			display.writef(8, "OFF     ");
		} else {
			switch (ECMMode) {
				case 1:
					display.writef(8, " SBY SSS");
					break;
				case 2:
					display.writef(8, " RDY SSS");
					break;
			}
		}


		display.writef(0, "A");
		display.writef(1, "%d", chaffCount);
		display.writef(5, "%*d", 3, flareCount);


		uint8_t display_brightness = data.dict.get("disp_brightness", 15);
		display.setBrightness(display_brightness);
	};
	myPico.add_output(new DisplayOutput(DISDATAIN, DISRS, DISCLOCK, DISCE, DISRESET, DISBLANK, 16, ewpiDisplayFunc));

	/*
	 ** EXAMPLE INPUT MODULES
	 */
	// std::function<void(PicoData&, bool)> toggleButtonFunc;
	// toggleButtonFunc = [](PicoData& data, bool button_on) {
	// 	data.setButton(0, button_on);
	// };
	// myPico.add_input(new ButtonInput(PRIBTN, toggleButtonFunc, ButtonMode::Toggle));
	// myPico.add_input(new ButtonInput(PRIBTN, 0, ButtonMode::Toggle));

	for (int i = 0; i < 4; ++i) {
		auto toggleButtonFunc = [i](PicoData& data, bool button_on) {
			data.setButton(i, button_on);
    	};
    	myPico.add_input(new ButtonInput(PRIBTN + i, toggleButtonFunc, ButtonMode::ActiveLow)); // Adjust PRIBTN + i as needed
	}
	
	std::function<void(PicoData&, uint16_t)> potFunc;
	potFunc = [](PicoData& data, uint16_t raw_val) {
		uint8_t brightness = (raw_val) >> 8;
		data.dict.set("disp_brightness", brightness);
	};
	myPico.add_input(new PotInput(BRTPOT, BRTADC, potFunc));

	/*
	 ** EXAMPLE UART MODULES
	 */
	// std::function<void(PicoData&, uart_inst_t*)> uart0Func;
	// uart0Func = [](PicoData& data, uart_inst_t* uart_inst) {
	// 	bool flag = data.flightData.IsSet2(FlightData::LightBits2::Unk);
	// 	uint8_t byte_to_send = flag;
	// 	uart_putc(uart_inst, byte_to_send);
	// };
	// myPico.add_uart(new UARTModule(EWMUTX, EWMURX, uart0, uart0Func));
	
	// Test uart 0 to uart1
	// std::function<void(PicoData&, uart_inst_t*)> uart1Func;
	// uart1Func = [](PicoData& data, uart_inst_t* uart_inst) {
	// 	uint8_t rec = uart_getc(uart_inst);
	// 	gpio_put(25, rec > 0);
	// };
	// myPico.add_uart(new UARTModule(4, 5, uart1, uart1Func));

	myPico.start();
	return 0;
}