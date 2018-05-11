#include "phys_mem.h"
#include "basic.h"
#include "io.h"
#include "string.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

typedef struct {
	uint64_t start;
	uint64_t len;
	uint32_t *bitmap;
	size_t bitmap_len;
} memory_region;

// @NOTE: Only choses one memory region.
memory_region valid_mem = {0};

extern const char __kernel_end;
const uintptr_t kernel_end = (uintptr_t)&__kernel_end;
uintptr_t bootstrap_mem = (uintptr_t)&__kernel_end;

// @NOTE: boot_malloc should really only be used in init_phys_mem, after that
// malloc_phys_page is available.
void *boot_malloc(size_t size) {
	void *r = (void *)bootstrap_mem;
	bootstrap_mem += size;
	return r;
}

void init_phys_mem(multiboot_info_t *mboot) {
	uintptr_t memory_info_addr = mboot->mmap_addr;

	size_t left = mboot->mmap_length;
	while (left) {
		multiboot_memory_map_t *e =
			(multiboot_memory_map_t *)memory_info_addr;

		// @NOTE / @TODO: Prune any address that isn't explicitly
		// greater than the kernel's end. There are more sophisticated
		// solutions, but this should be a small memory segment.
		bool available = e->type == MULTIBOOT_MEMORY_AVAILABLE;
		// @NOTE / @TODO: Leave PAGE_SIZE*2 bytes for boot_malloc.
		// Make this more dynamic?
		bool kernel_infringe = e->addr <= kernel_end + (PAGE_SIZE << 1);
		// @NOTE / @TODO: PAE
		bool out_of_range = e->addr + e->len > UINTPTR_MAX;
		if (available && !kernel_infringe && !out_of_range) {
			valid_mem.start = e->addr;
			valid_mem.len = e->len;
			break;
		} else {
			/* @NOTE: LOG("Rejected Memory Region: { Start: %X,
			 * Length: %X }", */
			/* e->addr, e->len); */
		}

		left -= e->size + 4;
		memory_info_addr += e->size + 4;
	}

	// Let's carve out some memory for the allocator's bitmaps
	const uint64_t pages = valid_mem.len >> 12;
	const uint64_t needed_u32s = pages >> 5;

	size_t bitmap_size = needed_u32s * sizeof(valid_mem.bitmap_len);
	valid_mem.bitmap = boot_malloc(bitmap_size);

	memset(valid_mem.bitmap, 0, bitmap_size);
	valid_mem.bitmap_len = needed_u32s;

	LOG("Chosen Memory Region: { Start: %X, End: %X }", valid_mem.start,
	    valid_mem.start + valid_mem.len);
}

void *malloc_phys_page(void) {
	uint32_t *const bitmap = valid_mem.bitmap;
	const size_t bitmap_len = valid_mem.bitmap_len;

	for (size_t i = 0; i < bitmap_len; ++i) {
		if (bitmap[i] == 0xffffffff) {
			continue;
		}

		for (size_t j = 0; j < sizeof(*bitmap) * 8; ++j) {
			if (!CHECK_BIT(bitmap[i], j)) {
				SET_BIT(bitmap[i], j);

				// @TODO: Casts valid (x86 vs x64?)
				return (void *)(uintptr_t)(
					valid_mem.start + (i * PAGE_SIZE * 32) +
					(j * PAGE_SIZE));
			}
		}
	}
	PANIC();
	return NULL;
}
