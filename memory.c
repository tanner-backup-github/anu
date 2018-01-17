#include <common.h>

extern char __kernel_end;
uintptr_t kernel_end = (uintptr_t)&__kernel_end;

// http://wiki.osdev.org/Memory_Map_(x86)#.22Upper.22_Memory_.28.3E_1_MiB.29

// @TODO: Check with multiple memory regions (holes)

uintptr_t *page_stack = NULL;
uintptr_t *page_stack_top = NULL;

#define PAGE_SIZE 0x1000

void init_free_memory(multiboot_info_t *mboot) {
	ASSERT(mboot->flags & 0b1000000);

	void *entry_addr = (void *)mboot->mmap_addr;
	multiboot_memory_map_t *entry = (multiboot_memory_map_t *)entry_addr;
	size_t bytes_for_stack = 0;
	while ((uintptr_t)entry_addr < mboot->mmap_addr + mboot->mmap_length) {
		if (entry->type == MULTIBOOT_MEMORY_AVAILABLE &&
		    entry->addr + entry->len > kernel_end) {
			uint64_t start = entry->addr;
			uint64_t size = entry->len;
			if (start > UINTPTR_MAX) {
				break;
			}
			if (start < kernel_end) {
				size -= kernel_end - start;
				start = kernel_end;
			}
			if (page_stack == NULL) {
				page_stack = (uintptr_t *)((uintptr_t)start +
							   (uintptr_t)size);
			}
			bytes_for_stack += size >> 10;
		}

		entry_addr += entry->size + sizeof(entry->size);
		entry = (multiboot_memory_map_t *)entry_addr;
	}

	page_stack -= bytes_for_stack >> 2;
	page_stack_top = page_stack;

	entry_addr = (void *)mboot->mmap_addr;
	entry = (multiboot_memory_map_t *)entry_addr;
	while ((uintptr_t)entry_addr < mboot->mmap_addr + mboot->mmap_length) {
		if (entry->type == MULTIBOOT_MEMORY_AVAILABLE &&
		    entry->addr + entry->len > kernel_end) {
			uint64_t start = entry->addr;
			uint64_t size = entry->len;
			if (start > UINTPTR_MAX) {
				LOG("Memory cannot be used due to architectural limits\n");
				break;
			}
			if (start < kernel_end) {
				size -= kernel_end - start;
				start = kernel_end;
			}

			// @NOTE: - PAGE_SIZE to throw out less than full pages
			for (size_t i = start; i < start + size - PAGE_SIZE;
			     i += PAGE_SIZE) {
				if (i > (uintptr_t)page_stack - PAGE_SIZE &&
				    i < (uintptr_t)page_stack +
						    bytes_for_stack) {
					break;
				}
				*--page_stack = i;
			}
		}

		entry_addr += entry->size + sizeof(entry->size);
		entry = (multiboot_memory_map_t *)entry_addr;
	}
	writef("%u Free pages\n", bytes_for_stack >> 2);

	// @TODO!!!: align the page_stack entries to page sizes
	// @TODO!!!: chuck the last pages of memory regions! (unless perfectly
	// aligned lol)
}

void *alloc_physical_page(void) {
	if (page_stack >= page_stack_top) {
		LOG("OUT OF MEMORY!\n");
		return NULL;
	}
	return (void *)*(page_stack++);
}
