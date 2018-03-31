#include "serial.h"
#include "port.h"
#include <stdbool.h>

// https://www.lammertbies.nl/comm/info/serial-uart.html
// http://wiki.osdev.org/Serial_Ports
// https://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming

#define COM1_PORT 0x3f8
#define DIVISOR_LATCH_LSB (COM1_PORT + 0)
#define DIVISOR_LATCH_MSB (COM1_PORT + 1)
#define INT_ENABLE (COM1_PORT + 1)
#define FIFO_CONTRTOL (COM1_PORT + 2)
#define LINE_CONTROL (COM1_PORT + 3)
#define MODEM_CONTROL (COM1_PORT + 4)
#define LINE_STATUS (COM1_PORT + 5)

#define DATA_AVAILABLE 0b1
#define STATUS_CHANGE 0b100000

// @NOTE: Divisor Latch Value = 115200 / BaudRate

void enable_serial(void) {
	// Disable all interrupts
	outb(INT_ENABLE, 0);

	// Enable DLAB (set baud rate divisor)
	outb(LINE_CONTROL, 0x80);

	// Set divisor to 3 (low byte)
	// Set high byte to 0
	// BaudRate = 38400
	// https://www.lammertbies.nl/comm/info/serial-uart.html#DLX
	outb(DIVISOR_LATCH_LSB, 3);
	outb(DIVISOR_LATCH_MSB, 0);

	// 8 bits (0b11), no parity bit (bit 3-5 of 0b000011), one stop bit (bit
	// two of 0b000011)
	outb(LINE_CONTROL, 0b000011);

	// enable FIFO (bit 0), clear rx/tx FIFOs (bit 1&2), with 14-byte(bit
	// 6&7) threshold
	outb(FIFO_CONTRTOL, 0b11000111);

	// IRQs enabled (IRQ4), RTS (Request To Send: RTS / CTS (Clear To Send)
	// handshake) / DSR (Data Set Ready: I'm
	// ready to communicate) set
	outb(MODEM_CONTROL, 0b1011);
}

bool received_serial_data(void) { return inb(LINE_STATUS) & DATA_AVAILABLE; }

uint8_t read_serial(void) {
	while (received_serial_data() == false) {
	}
	return inb(COM1_PORT);
}

bool transmit_empty(void) { return inb(LINE_STATUS) & STATUS_CHANGE; }

void write_serial(uint8_t b) {
	while (transmit_empty() == false) {
	}

	outb(COM1_PORT, b);
}
