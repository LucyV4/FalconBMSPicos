#include "f16pico.h"
#include "pico/stdlib.h"
#include "tusb.h"
#include "bsp/board.h"
#include <cstdint>

F16Pico::F16Pico(const char* deviceName): myUsb(deviceName) {
	usbDevice_setInstance(&myUsb);
}

void F16Pico::start() {
	stdio_init_all();

	board_init();
	tusb_init();

	init_modules();
	
	const uint8_t ONBRDLED = 25;
	gpio_init(ONBRDLED);
	gpio_set_dir(ONBRDLED, GPIO_OUT);
	gpio_put(ONBRDLED, 1);
	for (int i = 0; i < 300; i++) {
		sleep_ms(10);
		tud_task();
	}
	gpio_put(ONBRDLED, 0);

	while (true) {
		// WORKS
		tud_task();
		wake_up();
		update_serial();
		
		update_input();
		update_output();
		send_hid();

		sleep_ms(1);
	}
}

void F16Pico::update_serial(void) {
	// poll every 100ms
	const uint32_t interval_ms_cdc = 100;
	static uint32_t start_ms_cdc = 0;

	if (board_millis() - start_ms_cdc < interval_ms_cdc)
		return; // not enough time

	start_ms_cdc += interval_ms_cdc;

	if (tud_cdc_n_available(0) && tud_cdc_n_connected(0)) {
		// Determines size of message and buffer
		size_t receivedSize = sizeof(FlightData) + sizeof(FlightData2);
		uint8_t receivedBuffer[receivedSize];
		
		FlightData cur_flightData = FlightData();
		FlightData2 cur_flightData2 = FlightData2();

		// Reads the entire buffer
		tud_cdc_n_read(0, receivedBuffer, receivedSize);

		memcpy(&cur_flightData, receivedBuffer, sizeof(FlightData));
		memcpy(&cur_flightData2, receivedBuffer+sizeof(FlightData), sizeof(FlightData2));

		f16Data.flightData = cur_flightData;
		f16Data.flightData2 = cur_flightData2;
	}
}

void F16Pico::send_hid(void) {
	// Poll every 10ms
	const uint32_t interval_ms_hid = 10;
	static uint32_t start_ms_hid = 0;

	if (board_millis() - start_ms_hid < interval_ms_hid)
		return; // not enough time
	start_ms_hid += interval_ms_hid;

	if (tud_hid_ready()) {
		tud_hid_n_report(0x00, 0x01, &f16Data.report, sizeof(f16Data.report));
	}
}

void F16Pico::wake_up(void) {
	const uint32_t interval_ms_wakeup = 10;
	static uint32_t start_ms_wakeup = 0;

	if (board_millis() - start_ms_wakeup < interval_ms_wakeup)
		return;
	start_ms_wakeup += interval_ms_wakeup;


	// Remote wakeup
	if (tud_suspended()) {
		tud_remote_wakeup();
	}
}


void F16Pico::add_input(InputModule* module) { inputModules.push_back(module); }
void F16Pico::add_output(OutputModule* module) { outputModules.push_back(module); }

void F16Pico::init_modules() {
	for (auto *module : outputModules) { module->setup(); }
	for (auto *module : inputModules) { module->setup(); }
}

void F16Pico::update_input(void) {
	const uint32_t interval_ms_update_in = 10;
	static uint32_t start_ms_update_in = 5;
	
	if (board_millis() - start_ms_update_in < interval_ms_update_in) return;
	start_ms_update_in += interval_ms_update_in;

	for (auto *module : inputModules) { module->update(f16Data); }
}

void F16Pico::update_output(void) {
	const uint32_t interval_ms_update_out = 10;
	static uint32_t start_ms_update_out = 5;
	
	if (board_millis() - start_ms_update_out < interval_ms_update_out) return;
	start_ms_update_out += start_ms_update_out;

	for (auto *module : outputModules) { module->update(f16Data); }
}