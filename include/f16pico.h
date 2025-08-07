#pragma once
#include "usb_device.h"
#include "tusb.h"
#include <vector>
#include "output_module.h"
#include "input_module.h"
#include "uart_module.h"
#include "f16data.h"

class F16Pico {
public:
	F16Pico(const char* deviceName);
	void start();

	void add_input(InputModule* module);
	void add_output(OutputModule* module);
	void add_uart(UARTModule* module);

private:
	UsbDevice myUsb;
	F16Data f16Data = F16Data();

	std::vector<InputModule*> inputModules;
	std::vector<OutputModule*> outputModules;

	UARTModule* uartModule0 = nullptr;
	UARTModule* uartModule1 = nullptr;

	void init_modules();

	void update_serial();
	void update_input();
	void update_output();
	void update_uart();
	void send_hid();
	void wake_up();
};