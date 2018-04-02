#pragma once

#include <stdint.h>
#include <stdarg.h>

void write_string(const char *s);
void write_int(int64_t n, int8_t base);
void write_uint(uint64_t n, int8_t base);
void writef(const char *fmt, ...);
