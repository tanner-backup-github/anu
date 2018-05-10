#pragma once

#include "io.h"
#include <stdint.h>

typedef struct {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers;

extern void hang(void);

#define ARRAY_SIZE(a) ((sizeof(a)) / sizeof(*(a)))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define X86

#define __ASSERT(b, file, func, line)                                          \
	do {                                                                   \
		if (!(b)) {                                                    \
			printf("Assertion failed in file %s in function %s "   \
			       "on line %d.\n",                                \
			       file, func, line);                              \
			hang();                                                \
		}                                                              \
	} while (0);
#define ASSERT(b) __ASSERT(b, __FILE__, __FUNCTION__, __LINE__);

#define __PANIC(file, func, line)                                              \
	do {                                                                   \
		printf("Panic in file %s in function %s on line %d.\n", file,  \
		       func, line);                                            \
		hang();                                                        \
	} while (0);
#define PANIC() __PANIC(__FILE__, __FUNCTION__, __LINE__);

#ifdef DEBUG
#define __LOG(s, file, func, line, ...)                                        \
	do {                                                                   \
		printf("[Log (%s %s %d)]: ", file, func, line);                \
		printf(s "\n", ##__VA_ARGS__);                                 \
	} while (0);
#define LOG(s, ...) __LOG(s, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define __LOG(s, file, func, line)
#define LOG(s)
#endif
