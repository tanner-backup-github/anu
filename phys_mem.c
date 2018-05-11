#include "phys_mem.h"
#include "basic.h"
#include "io.h"
#include "string.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint64_t start;
	uint64_t len;
	uint32_t *bitmap;
	size_t bitmap_len;
} memory_region;

#define PAGE_SIZE 4096

extern const char __kernel_end;
const uintptr_t kernel_end = (uintptr_t)&__kernel_end;
uintptr_t bootstrap_mem = (uintptr_t)&__kernel_end;

memory_region *valid_mem = NULL;
size_t valid_mem_len = 0;

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
	// @NOTE: Max of 8 valid memory regions. I haven't seen more than 3.
	valid_mem = boot_malloc(8 * sizeof(*valid_mem));
	while (left) {
		multiboot_memory_map_t *e =
			(multiboot_memory_map_t *)memory_info_addr;

		// @NOTE / @TODO: Prune any address that isn't explicitly
		// greater than the kernel's end. There are more sophisticated
		// solutions, but this should be a small memory segment.
		bool available = e->type == MULTIBOOT_MEMORY_AVAILABLE;
		// @NOTE / @TODO: Leave PAGE_SIZE bytes for boot_malloc.
		// Make this more dynamic?
		bool kernel_infringe = e->addr <= kernel_end + PAGE_SIZE;
		// @NOTE / @TODO: PAE
		bool out_of_range = e->addr + e->len > UINTPTR_MAX;
		if (available && !kernel_infringe && !out_of_range) {
			valid_mem[valid_mem_len].start = e->addr;
			valid_mem[valid_mem_len].len = e->len;
			++valid_mem_len;
		} else {
			LOG("Rejected Memory Region: { Start: %X, Length: %X }",
			    e->addr, e->len);
		}

		left -= e->size + 4;
		memory_info_addr += e->size + 4;
	}

	// Let's carve out some memory for the allocator's bitmaps
	for (size_t i = 0; i < valid_mem_len; ++i) {
		const uint64_t pages = to_pow2(valid_mem[i].len >> 12);
		const uint64_t needed_u32s = pages >> 5;

		size_t bitmap_size =
			needed_u32s * sizeof(valid_mem[i].bitmap_len);
		valid_mem[i].bitmap = boot_malloc(bitmap_size);

		memset(valid_mem[i].bitmap, 0, bitmap_size);
		valid_mem[i].bitmap_len = needed_u32s;

		LOG("Memory Region %d: { Start: %X, Length: %X }", i,
		    valid_mem[i].start, valid_mem[i].len);
	}
}

void *malloc_phys_page(void) {
	for (size_t i = 0; i < valid_mem_len; ++i) {
		uint32_t *const bitmap = valid_mem[i].bitmap;
		const size_t bitmap_len = valid_mem[i].bitmap_len;

		for (size_t j = 0; j < bitmap_len; ++j) {
			for (size_t k = 0; k < sizeof(*bitmap) * 8; ++k) {
				if (!CHECK_BIT(bitmap[j], k)) {
					SET_BIT(bitmap[j], k);

					// @TODO: Casts valid (x86 vs x64?)
					return (void *)(uintptr_t)(
						valid_mem[i].start +
						(j * PAGE_SIZE * 32) +
						(k * PAGE_SIZE));
				}
			}
		}
	}
	PANIC();
	return NULL;
}
