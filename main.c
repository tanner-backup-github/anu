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

	enable_serial();

	ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

	install_gdt();
	install_idt();
	install_irqs();
	init_free_memory(mboot);

	char *x = malloc_page();
	memset(x, 'a', 4096);
	x[4095] = '\0';
	writef("%s\n", x);
	
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
