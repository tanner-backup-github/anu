#include "basic.h"
#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "irq.h"
#include "multiboot.h"
#include "serial.h"
#include <stdbool.h>
#include <stdint.h>

// @NOTE: M-% query-replace
// @NOTE: M-z zap to char
// @NOTE: dired g refresh buffer
// @NOTE: the new negative compliant writef hasn't been tested much yet.
// @NOTE: buf-move
// @NOTE: goto line M-g g

extern const char __kernel_end;
const uintptr_t kernel_end = (uintptr_t)&__kernel_end;
uintptr_t free_mem;

void *kmalloc(size_t size) {
	void *give = (void *)free_mem;
	free_mem += size;
	return give;
}

int32_t kmain(uint32_t magic, multiboot_info_t *mboot) {

	// @TODO: Higher half kernel
	// @TODO: Find PIC documentation for saved_info (Good?)

	enable_serial();

	ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

	install_gdt();
	install_idt();
	install_irqs();

	asm volatile("sti");

	writef("Entering loop...\n");

	free_mem = kernel_end;
	int *x = kmalloc(4);
	writef("%x\n", x);
	*x = 64;
	writef("%d\n", *x);
	
	uintptr_t memory_info_addr = mboot->mmap_addr;
	for (size_t i = 0; i < 16; ++i) {
		multiboot_memory_map_t *e = (multiboot_memory_map_t
	*)memory_info_addr;
		if (e->type == MULTIBOOT_MEMORY_AVAILABLE) {
			writef("%X %X %a\n", e->addr, e->len, kernel_end);
		}
	        memory_info_addr += e->size + 4;
	}

	while (true) {
		asm volatile("hlt");
	}

	return 0;
}
