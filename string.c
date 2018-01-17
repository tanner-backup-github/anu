#include <common.h>

size_t strlen(const char *s) {
	size_t l = 0;
	while (s[l] != '\0') {
		++l;
	}
	return l;
}

void memset(const void *b, uint8_t c, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		((char *) b)[i] = c;
	}
}

void memcpy(void *dest, const void *src, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		((char *) dest)[i] = ((char *) src)[i];
	}
}
