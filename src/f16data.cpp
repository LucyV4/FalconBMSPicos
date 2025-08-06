#include "f16data.h"

F16Data::F16Data() {}

void F16Data::setThrottle(uint8_t index, uint16_t value) { report.throttle[index] = value; }
void F16Data::setButton(uint8_t index, bool pressed) {
	int byteIndex = index / 8;
    int bitIndex = index % 8;

    if (pressed)
        report.buttons[byteIndex] |= (1 << bitIndex);
    else
        report.buttons[byteIndex] &= ~(1 << bitIndex);
}