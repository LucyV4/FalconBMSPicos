#pragma once 

#include "f16data.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include <cstdint>
#include <functional>

class UARTModule {
public:
	UARTModule(uint8_t uart_tx, uint8_t uart_rx, uart_inst_t* uart_inst, std::function<void(F16Data&, uart_inst_t*)> update_function) : uart_pin_tx(uart_tx), uart_pin_rx(uart_rx), uart_inst(uart_inst), updateFunc(update_function) {}

	void setup() {
		gpio_set_function(uart_pin_tx, GPIO_FUNC_UART);
		gpio_set_function(uart_pin_rx, GPIO_FUNC_UART);
		uart_init(uart_inst, 115200);
		uart_set_hw_flow(uart_inst, false, false);
	}

	void update(F16Data& data) {
		updateFunc(data, uart_inst);
	}

	uart_inst_t* get_uart_inst() { return uart_inst; }

	virtual ~UARTModule();
private:
	uint8_t uart_pin_tx;
	uint8_t uart_pin_rx;
	uart_inst_t* uart_inst;

	std::function<void(F16Data&, uart_inst_t*)> updateFunc;
};