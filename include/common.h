#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <multiboot.h>

// @NOTE: esp useless?
typedef struct {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} Registers;

extern void hang();

#define __ASSERT(b, file, func, line) do { if (!(b)) {			\
		writef("Assertion failed in file %s in function %s on line %d.\n", file, func, line); \
		hang(); } } while (0);
#define ASSERT(b) __ASSERT(b, __FILE__, __FUNCTION__, __LINE__);

#define __PANIC(file, func, line) do { writef("Panic in file %s in function %s on line %d.\n", file, func, line); \
		hang(); } while(0);
#define PANIC() __PANIC(__FILE__, __FUNCTION__, __LINE__);

#ifdef DEBUG
#define __LOG(s, file, func, line) do { writef("[Log (%s %s %d)]: %s\n", file, func, line, (s)); } while(0);
#define LOG(s) __LOG(s, __FILE__, __FUNCTION__, __LINE__);
#else
#define __LOG(s, file, func, line)
#define LOG(s)
#endif

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof(*(arr)))
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) < (y) ? (y) : (x))

// asm.c
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

// gdt.c
void install_gdt(void);

// idt.c
void install_idt(void);
void set_idt_entry(size_t i, uint32_t offset, uint16_t selector, uint8_t flags);

// irq.c
void install_irqs(void);

// memory.c
void memory_xxx(multiboot_info_t *mboot);

#endif
