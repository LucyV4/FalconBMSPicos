# Falcon BMS Picos
This project adds an Object Oriented way to program picos for a Falcon BMS simulation setup using C++.

This can be used in parralel with my other project [FalconBMSToSerial](https://github.com/LucyV4/FalconBMSToSerial) which sends the memory data from Falcon BMS to serial ports.

## Requirements
For this project I would recommend using the [VSCode Raspberry Pi Pico extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico). This allows for easy selection of the Raspberry Pi Pico type and Pico SDK version. If you want to build it manually you will need to change the Cmake files as well as install the following packages.
- CMake
- An arm gcc compiler
- Ninja
- pico-sdk
- picotool 
- openocd

## Credits
The `display.h` was adapted from [LedDisplay](https://github.com/PaulStoffregen/LedDisplay) by Paul Stoffregen. The font used in `display.h` was made by Pascal Stang.