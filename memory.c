#include <common.h>

extern char __kernel_end;
uintptr_t kernel_end = (uintptr_t) &__kernel_end;

/* @TODO(!!): Figure out if memory can be out of the 2**32-1 range. I don't believe this,
   but I have heard addresses can be 52 bits? Not for x86 (unless PAE)?
   Using uintptr's for now.
 */

void memory_xxx(multiboot_info_t *mboot) {
	(void) mboot;

	writef("%x\n", kernel_end);
	writef("%b\n", mboot->flags);
}
