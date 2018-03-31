#pragma once

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *s);
void memset(const void *b, uint8_t c, size_t n);
void memcpy(void *dest, const void *src, size_t n);
