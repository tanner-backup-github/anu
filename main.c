#include <common.h>

// @NOTE: M-% query-replace
// @NOTE: M-z zap to char
// @NOTE: dired g refresh buffer
// @NOTE: the new negative compliant writef hasn't been tested much yet.
// @NOTE: buf-move
// @NOTE: goto line M-g g

int32_t kmain(uint32_t magic, multiboot_info_t *mboot) {

	// @TODO: Copy mboot?

	// @TODO: Higher half kernel

	// @TODO: format string for arch dependent types

	enable_serial();

	ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

	install_gdt();
	install_idt();
	install_irqs();
	init_free_memory(mboot);

	uint32_t *x = (uint32_t *) 1;
	writef("IN\n");
	while (x != NULL) {		
		uint32_t *z = alloc_physical_page();
		x = alloc_physical_page();
		if ((uintptr_t) x + 4096 != (uintptr_t) z) {
			writef("%x %x NOT LINEAR\n", x, z);
		}
	}
	writef("OUT\n");
	
	// @NOTE: I do need this, but why?
	asm volatile("sti");
	/* asm volatile("int $0x50"); */

	writef("Entering loop...\n");

	/* @TODO: Think about how to seperate interrupt stuff for software,
	   hardware,
	   and exception interrupts? Think about other source tree
	   architectures? */

	while (true) {
		asm volatile("hlt");
	}

	return 0;
}
