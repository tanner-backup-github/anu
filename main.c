#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "irq.h"
#include "multiboot.h"
#include "serial.h"
#include "universe.h"
#include <stdbool.h>
#include <stdint.h>

// @NOTE: M-% query-replace
// @NOTE: M-z zap to char
// @NOTE: dired g refresh buffer
// @NOTE: the new negative compliant writef hasn't been tested much yet.
// @NOTE: buf-move
// @NOTE: goto line M-g g

int32_t kmain(uint32_t magic, multiboot_info_t *mboot) {

	// Copy mboot?

	// @TODO: Higher half kernel

	enable_serial();

	ASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

	install_gdt();
	install_idt();
	install_irqs();

	asm volatile("sti");

	writef("Entering loop...\n");

	while (true) {
		asm volatile("hlt");
	}

	return 0;
}
