#pragma once
#include "usb_device.h"
#include "tusb.h"
#include <vector>
#include "output_module.h"
#include "input_module.h"
#include "f16data.h"

class F16Pico {
public:
	F16Pico(const char* deviceName);
	void start();

	void add_output(OutputModule* module);
	void add_input(InputModule* module);

private:
	UsbDevice myUsb;
	F16Data f16Data = F16Data();

	std::vector<InputModule*> inputModules;
	std::vector<OutputModule*> outputModules;

	void init_modules();

	void update_serial(void);
	void update_output();
	void update_input();
	void send_hid(void);
	void wake_up(void);
};