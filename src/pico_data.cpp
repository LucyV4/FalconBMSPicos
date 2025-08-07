#include "pico_data.h"

PicoData::PicoData() {}

void PicoData::setThrottle(uint8_t index, uint16_t value) { report.throttle[index] = value; }
void PicoData::setButton(uint8_t index, bool pressed) {
	int byteIndex = index / 8;
    int bitIndex = index % 8;

    if (pressed)
        report.buttons[byteIndex] |= (1 << bitIndex);
    else
        report.buttons[byteIndex] &= ~(1 << bitIndex);
}