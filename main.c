#include <common.h>

// @NOTE: M-% query-replace
// @NOTE: M-z zap to char
// @NOTE: C-u s --group-directories-first
// @NOTE: dired g refresh buffer
// @NOTE: C-SPC toggle maximize window
// @NOTE: SUPER-DIRECTION moves windows
// @NOTE: the new negative compliant writef hasn't been tested much yet.
// @NOTE: buf-move
// @NOTE: goto line M-g g

// @TODO: make writef more printf compliant. althought printf doesn't print binary for some reason.
// @TODO: log writef
// @TODO: ASSERT

int32_t kernel_main(uint32_t magic, multiboot_info_t *mboot) {
	
	// @TODO: Copy mboot?
	
	enable_serial();
	
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		writef("Invalid magic passed from bootloader!\n");
		return -1;
	}
	
	install_gdt();
	install_idt();
	install_irqs();
        memory_xxx(mboot);
	
	// @TODO: I can still have interrupts without this? Figure out why.
	// @TODO: When is the exact right time to enable this?
	asm volatile ("sti");
	
	writef("Entering loop...\n");
	
	/* @TODO: Think about how to seperate interrupt stuff for software, hardware,
	   and exception interrupts? Think about other source tree architectures? */

	while (true) {
		asm volatile ("hlt");
	}

	return 0;
}
