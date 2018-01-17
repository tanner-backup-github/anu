#pragma once

#include <multiboot.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// @NOTE: esp useless?
typedef struct {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers;

extern void hang(void);

// @TODO: varargs
#define __ASSERT(b, file, func, line)                                          \
	do {                                                                   \
		if (!(b)) {                                                    \
			writef("Assertion failed in file %s in function %s "   \
			       "on line %d.\n",                                \
			       file, func, line);                              \
			hang();                                                \
		}                                                              \
	} while (0);
#define ASSERT(b) __ASSERT(b, __FILE__, __FUNCTION__, __LINE__);

#define __PANIC(file, func, line)                                              \
	do {                                                                   \
		writef("Panic in file %s in function %s on line %d.\n", file,  \
		       func, line);                                            \
		hang();                                                        \
	} while (0);
#define PANIC() __PANIC(__FILE__, __FUNCTION__, __LINE__);

#ifdef DEBUG
#define __LOG(s, file, func, line)                                             \
	do {                                                                   \
		writef("[Log (%s %s %d)]: %s\n", file, func, line, (s));       \
	} while (0);
#define LOG(s) __LOG(s, __FILE__, __FUNCTION__, __LINE__);
#else
#define __LOG(s, file, func, line)
#define LOG(s)
#endif

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof(*(arr)))
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) < (y) ? (y) : (x))

// port.c
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

// serial.c
void enable_serial(void);
uint8_t read_serial(void);
void write_serial(uint8_t b);

// io.c
// @NOTE: supports: 's', 'c', 'd', 'u', 'U', 'x', 'X', and 'b'
void writef(const char *fmt, ...);

// string.c
size_t strlen(const char *s);
void memset(const void *b, uint8_t c, size_t n);
void memcpy(void *dest, const void *src, size_t n);

#define DATA_SELECTOR 0x08

#define PRESENT 0b10000000
#define DPL_KERNEL 0b00000000
#define DPL_USER 0b01100000
#define INT_TRAP_STORAGE_SEGMENT 0b00000000
#define TASK_STORAGE_SEGMENT 0b00001000

// @NOTE: there are 16-bit gates too
#define TASK_GATE_32BIT 0b00000101
#define INT_GATE_32BIT 0b00001110
#define TRAP_GATE_32BIT 0b00001111

#define INT_GATE_FLAGS                                                         \
	PRESENT | DPL_KERNEL | INT_TRAP_STORAGE_SEGMENT | INT_GATE_32BIT

// gdt.c
void install_gdt(void);

// idt.c
void install_idt(void);
void set_idt_entry(size_t i, uint32_t offset, uint16_t selector, uint8_t flags);

// irq.c
void install_irqs(void);

// memory.c
void init_free_memory(multiboot_info_t *mboot);
void *alloc_physical_page(void);
