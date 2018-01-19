#include <common.h>

// @NOTE: M-% query-replace
// @NOTE: M-z zap to char
// @NOTE: dired g refresh buffer
// @NOTE: the new negative compliant writef hasn't been tested much yet.
// @NOTE: buf-move
// @NOTE: goto line M-g g

int32_t kmain(uint32_t magic, multiboot_info_t *mboot) {

	// Copy mboot?

	// @TODO: Higher half kernel	
	
	uint16_t *real_shit = (uint16_t *)0xc00b8000;
	*real_shit = 'A' | 1 << 8;
	
	enable_serial();
	
	ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

	install_gdt();
	install_idt();
	install_irqs();
	init_free_memory(mboot);
        /* enable_paging(); */

	asm volatile("sti");
	/* asm volatile("int $0x50"); */

	writef("Entering loop...\n");

	while (true) {
		asm volatile("hlt");
	}

	return 0;
}
