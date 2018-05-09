#include "basic.h"
#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "irq.h"
#include "multiboot.h"
#include "serial.h"
#include <stdbool.h>
#include <stdint.h>

extern const char __kernel_end;
const uintptr_t kernel_end = (uintptr_t)&__kernel_end;

int32_t kmain(uint32_t magic, multiboot_info_t *mboot) {

	// @TODO: Higher half kernel. Memory protection
	// @TODO: Find PIC documentation for saved_info (Good?)

	enable_serial();

	ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

	install_gdt();
	install_idt();
	install_irqs();

	asm volatile("sti");

	writef("Entering loop...\n");

	uintptr_t memory_info_addr = mboot->mmap_addr;
	for (size_t i = 0; i < 16; ++i) {
		multiboot_memory_map_t *e =
			(multiboot_memory_map_t *)memory_info_addr;
		if (e->type == MULTIBOOT_MEMORY_AVAILABLE) {
			writef("%X %X\n", e->addr, e->len);
		}
		memory_info_addr += e->size + 4;
	}

	while (true) {
		asm volatile("hlt");
	}

	return 0;
}
