#include "basic.h"
#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "irq.h"
#include "multiboot.h"
#include "phys_mem.h"
#include "serial.h"
#include <stdbool.h>
#include <stdint.h>

// @TODO: Higher half kernel. Memory protection
// @TODO: Find PIC documentation for saved_info (Good?)
// @TODO: VGA terminal
// @TODO: Clean out some notes?

int32_t kmain(uint32_t magic, multiboot_info_t *mboot) {

	enable_serial();

	ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

	install_gdt();
	install_idt();
	install_irqs();
	init_phys_mem(mboot);

	asm volatile("sti");

	printf("Entering loop...\n");

	for (size_t i = 0; i < 40; ++i) {
		malloc_phys_page();
	}
	free_phys_page((void *)0x100000 + (0x1000 * 39));
	print_allocator_bitmap();

	while (true) {
		asm volatile("hlt");
	}

	return 0;
}
