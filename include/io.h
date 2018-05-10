#pragma once

#include <stdint.h>
#include <stdarg.h>

void print_string(const char *s);
void print_int(int64_t n, int8_t base);
void print_uint(uint64_t n, int8_t base);
void printf(const char *fmt, ...);
