#include <common.h>

void outb(uint16_t port, uint8_t value) {
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
	uint8_t return_value;
	asm volatile ("inb %1, %0" : "=a"(return_value) : "Nd"(port));
	return return_value;
}
