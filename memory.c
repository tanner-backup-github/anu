#include <common.h>

extern char __kernel_end;
uintptr_t kernel_end = (uintptr_t)&__kernel_end;

typedef struct {
	uint64_t start;
	uint64_t len;
	uint8_t *pages;
} memory_region;

memory_region *regions = NULL;
size_t memory_regions_len = 0;

#define MEMORY_REGIONS_BUFFER_SIZE (16 * sizeof(memory_region))

void init_free_memory(multiboot_info_t *mboot) {
	ASSERT(mboot->flags & 0b1000000);

	regions = (memory_region *)kernel_end;
	memset(regions, 0, MEMORY_REGIONS_BUFFER_SIZE);

	void *entry_addr = (void *)mboot->mmap_addr;
	multiboot_memory_map_t *entry = (multiboot_memory_map_t *)entry_addr;

	while ((uintptr_t)entry_addr < mboot->mmap_addr + mboot->mmap_length) {
		if (entry->type == MULTIBOOT_MEMORY_AVAILABLE &&
		    entry->addr + entry->len > kernel_end) {
			uint64_t start = entry->addr;
			uint64_t len = entry->len;
			if (start < kernel_end) {
				len -= (kernel_end +
					MEMORY_REGIONS_BUFFER_SIZE - start);
				start = kernel_end + MEMORY_REGIONS_BUFFER_SIZE;
			}
			regions[memory_regions_len] =
				(memory_region){.start = start, .len = len};
			++memory_regions_len;
		}

		entry_addr += entry->size + sizeof(entry->size);
		entry = (multiboot_memory_map_t *)entry_addr;
	}

	for (size_t i = 0; i < memory_regions_len; ++i) {
		
		writef("%X %X\n", regions[i].start, regions[i].len);
	}
	
}
