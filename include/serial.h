#pragma once

#include <stdint.h>

void enable_serial(void);
uint8_t read_serial(void);
void print_serial(uint8_t b);
