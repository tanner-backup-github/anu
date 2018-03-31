#include "io.h"
#include "serial.h"
#include "string.h"
#include <stddef.h>

void write_string(const char *s) {
	for (size_t i = 0; s[i] != '\0'; ++i) {
		write_serial(s[i]);
	}
}

void write_int(int64_t n, int8_t base) {
	char buf[64] = {0};
	int8_t digits = 0;
	if (n < 0) {
		n *= -1;
		buf[0] = '-';
		++digits;
	}
	int64_t n_copy = n;
	do {
		++digits;
	} while (n_copy /= base);
	do {
		--digits;
		buf[digits] = "0123456789abcdef"[n % base];
	} while (n /= base);
	write_string(buf);
}

void write_uint(uint64_t n, int8_t base) {
	uint64_t n_copy = n;
	int8_t digits = 0;
	do {
		++digits;
	} while (n_copy /= base);
	char buf[64] = {0};
	do {
		--digits;
		buf[digits] = "0123456789abcdef"[n % base];
	} while (n /= base);
	write_string(buf);
}

void writef(const char *fmt, ...) {
	va_list vl;
	va_start(vl, fmt);

	size_t fmt_length = strlen(fmt);
	for (size_t i = 0; i < fmt_length; ++i) {
		char c = fmt[i];
		if (c == '%' && i + 1 < fmt_length) {
			size_t fmt_char = fmt[i + 1];
			switch (fmt_char) {
			case 's':
				write_string(va_arg(vl, char *));
				break;
			case 'a':
#ifdef X86
				write_string("0x");
				write_uint(va_arg(vl, uint32_t), 16);
#else
				write_string("0x");
				write_uint(va_arg(vl, uint64_t), 16);
#endif
				break;
			case 'c':
				write_serial(va_arg(vl, int32_t));
				break;
			case 'd':
				write_int(va_arg(vl, int32_t), 10);
				break;
			case 'D':
				write_int(va_arg(vl, int64_t), 10);
				break;
			case 'u':
				write_uint(va_arg(vl, uint32_t), 10);
				break;
			case 'U':
				write_uint(va_arg(vl, uint64_t), 10);
				break;
			case 'x':
				write_string("0x");
				write_uint(va_arg(vl, uint32_t), 16);
				break;
			case 'X':
				write_string("0x");
				write_uint(va_arg(vl, uint64_t), 16);
				break;
			case 'b':
				write_string("0b");
				write_uint(va_arg(vl, uint32_t), 2);
				break;
			default:
				write_serial('%');
				break;
			}
			++i;
		} else {
			write_serial(c);
		}
	}
}
