#include <common.h>

#define PORT 0x3f8

// DOC & READ SERIAL OPTIONS
// Better names for received_serial_data and transmit_empty

void enable_serial(void) {
	outb(PORT + 1, 0x00); // Disable all interrupts
	outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud (hi byte)
	outb(PORT + 1, 0x00); // (hi byte)
	outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xc7); // enable fifo, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0b); // IRQs enabled, RTS/DSR set
}

bool received_serial_data(void) { return inb(PORT + 5) & 1; }

uint8_t read_serial(void) {
	while (received_serial_data() == false) {
	}
	return inb(PORT);
}

bool transmit_empty(void) { return inb(PORT + 5) & 0x20; }

void write_serial(uint8_t b) {
	while (transmit_empty() == false) {
	}

	outb(PORT, b);
}
