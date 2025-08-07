
#pragma once

#include <stdint.h>
#include "flightdata.h"

class PicoData {
public:
	PicoData();

	void setThrottle(uint8_t index, uint16_t value);
	void setButton(uint8_t index, bool pressed);
	
	#pragma pack(push, 1)
	struct HIDReport {
		uint16_t throttle[6];
		uint8_t buttons[16];
	};
	#pragma pack(pop)
	
	HIDReport report;
	FlightData flightData = FlightData();
	FlightData2 flightData2 = FlightData2();
};