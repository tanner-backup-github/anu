#include "phys_mem.h"
#include "io.h"
#include <stddef.h>
#include <stdint.h>

extern const char __kernel_end;
const uintptr_t kernel_end = (uintptr_t)&__kernel_end;

void init_phys_mem(multiboot_info_t *mboot) {
	uintptr_t memory_info_addr = mboot->mmap_addr;
	// @TODO: Remove hardcoded 16
	for (size_t i = 0; i < 16; ++i) {
		multiboot_memory_map_t *e =
			(multiboot_memory_map_t *)memory_info_addr;
		if (e->type == MULTIBOOT_MEMORY_AVAILABLE) {
			printf("%X %X\n", e->addr, e->len);
		}
		memory_info_addr += e->size + 4;
	}
}
