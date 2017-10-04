#include <common.h>

size_t strlen(const char *s) {
	size_t l = 0;
	while (s[l] != '\0') {
		++l;
	}
	return l;
}
