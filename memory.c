#include <common.h>

extern char __kernel_end;
uintptr_t kernel_end = (uintptr_t)&__kernel_end;

// http://wiki.osdev.org/Memory_Map_(x86)#.22Upper.22_Memory_.28.3E_1_MiB.29

typedef struct {
	uintptr_t *page_stack;
	size_t pages_len;
	size_t start;
	size_t size;
} memory_region;

memory_region *regions = NULL;
size_t regions_len = 0;

#define MEMORY_REGIONS_BUFFER_SIZE (16 * sizeof(memory_region))
#define PAGE_SIZE 0x1000
// @NOTE: never gets evaluated

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
			uint64_t size = entry->len;
			if (start > UINTPTR_MAX) {
				break;
			}
			if (start < kernel_end) {
				size -= kernel_end +
					MEMORY_REGIONS_BUFFER_SIZE - start;
				start = kernel_end + MEMORY_REGIONS_BUFFER_SIZE;
			}

			regions[regions_len] =
				(memory_region){.start = start, .size = size};
			++regions_len;
		}

		entry_addr += entry->size + sizeof(entry->size);
		entry = (multiboot_memory_map_t *)entry_addr;
	}

	for (size_t i = 0; i < regions_len; ++i) {
		size_t pages_len = (regions[i].size >> 12) + 1;
		// @TODO!!!: align the 'start' to page sizes
		// @TODO!!!: chuck the last page!
		regions[i].page_stack =
			(uintptr_t *)((regions[i].start & (~0xfff)) +
				      PAGE_SIZE);
		regions[i].start =
			(uintptr_t)regions[i].page_stack + (pages_len << 2);
		regions[i].size -=
			regions[i].start - (uintptr_t)regions[i].page_stack;
		regions[i].pages_len = pages_len;

		for (size_t j = 0; j < pages_len; ++j) {
			regions[i].page_stack[j] = regions[i].start + (j << 12);
			/* writef("%x %x\n", regions[i].start + regions[i].size,
			 * regions[i].page_stack[j]); */
		}

		/* writef("Start: %x\nPageStack: %x\nSize: %x\nEnd: " */
		/*        "%x\nPages_Len: %x\n", */
		/*        regions[i].start, regions[i].page_stack, */
		/* regions[i].size, */
		/*        regions[i].start + regions[i].size, pages_len); */
	}
}

// @NOTE: Simple watermark
void *malloc_physical_page(void) {
	for (size_t i = 0; i < regions_len; ++i) {
		if ((uintptr_t)regions[i].page_stack == regions[i].start) {
			continue;
		}

		void *page = (void *)*regions[i].page_stack;
		++regions[i].page_stack;
		return page;
	}

	LOG("Out of Memory!\n");
	return NULL;
}

void free_physical_page(void *page) {
}
