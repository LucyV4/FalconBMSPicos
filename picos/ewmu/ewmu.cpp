// ewpi.cpp
#include "f16pico.h"
#include "output_led.h"
#include "output_display.h"
#include "input_button.h"
#include "input_potentiometer.h"
#include "f16data.h"

enum GpioPins: uint8_t {
	EWMUTX = 0,		// EWMU uart TX pin
	EWMURX = 1, 	// EWMU uart RX pin

	LNKLED = 6,		// LNK led
	LNCHLED = 7,	// LNCH led
	PRILED = 8,		// PRI led
	OPNLED = 9,		// OPN led

	PRIBTN = 10,	// PRI button
	SEPBTN = 11,	// SEP button
	UNKBTN = 12,	// UNK button
	MODEBTN = 13,	// MODE button

	DISBLANK = 16,	// Display Blank (can go?)
	DISCE = 17,		// Display chip enable
	DISCLOCK = 18,	// Display clock
	DISDATAIN = 19,	// Display data in
	DISRESET = 20,	// Display Reset
	DISRS = 21,		// Display Register Select

	BRTPOT = 28,	// BRT pot
	BRTADC = 2,		// BRT pot adc num
};

F16Pico myPico("EWMU");

int main() {
	myPico.add_output(new LedOutput(LNKLED, FlightData::LightBits2::Unk));
	myPico.add_output(new LedOutput(LNCHLED, FlightData::LightBits2::Launch, FlightData2::BlinkBits::Launch, 250));
	myPico.add_output(new LedOutput(PRILED, FlightData::LightBits2::PriMode, FlightData2::BlinkBits::PriMode, 100));
	myPico.add_output(new LedOutput(OPNLED, FlightData::LightBits2::PriMode, FlightData2::BlinkBits::PriMode, 100, true));

	std::function<void(F16Data&, Display&)> ewpiDisplayFunc;
	ewpiDisplayFunc = [](F16Data& data, Display& display) {
		display.writef(0, "%d", data.flightData.ChaffCount);
		display.writef(4, "%*d", 4, data.flightData.FlareCount);
		display.writef(8, "JMR.data.");
	};
	myPico.add_output(new DisplayOutput(DISDATAIN, DISRS, DISCLOCK, DISCE, DISRESET, DISBLANK, 16, ewpiDisplayFunc));

	std::function<void(F16Data&, bool)> toggleButtonFunc;
	toggleButtonFunc = [](F16Data& data, bool button_on) {
		gpio_put(25, button_on);
		data.setButton(0, button_on);
	};
	myPico.add_input(new ButtonInput(PRIBTN, toggleButtonFunc, ButtonMode::Toggle));
	// myPico.add_input(new ButtonInput(PRIBTN, 0, ButtonMode::Toggle));
	// myPico.add_input(new PotInput(BRTPOT, BRTADC, 0));

	myPico.start();
	return 0;
}