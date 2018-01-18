#include <common.h>

extern char __kernel_end;
uintptr_t kernel_end = (uintptr_t)&__kernel_end;

// http://wiki.osdev.org/Memory_Map_(x86)#.22Upper.22_Memory_.28.3E_1_MiB.29

// @TODO: Check with multiple memory regions (holes)
// @TODO: Comb this over sometime... probably riddled with idiocy

uintptr_t *page_stack = NULL;
uintptr_t *page_stack_top = NULL;

#define PAGE_SIZE 0x1000

void init_free_memory(multiboot_info_t *mboot) {
	ASSERT(mboot->flags & 0b1000000);

	size_t bytes_for_stack = 0;

#define TRAVERSE_MAP(work)                                                     \
	{                                                                      \
		void *entry_addr = (void *)mboot->mmap_addr;                   \
		multiboot_memory_map_t *entry =                                \
			(multiboot_memory_map_t *)entry_addr;                  \
		while ((uintptr_t)entry_addr <                                 \
		       mboot->mmap_addr + mboot->mmap_length) {                \
			if (entry->type == MULTIBOOT_MEMORY_AVAILABLE &&       \
			    entry->addr + entry->len > kernel_end) {           \
				uint64_t start = entry->addr;                  \
				uint64_t size = entry->len;                    \
				if (start > UINTPTR_MAX) {                     \
					LOG("Memory which cannot be used due " \
					    "to "                              \
					    "architectural limits\n");         \
					break;                                 \
				}                                              \
				if (start < kernel_end) {                      \
					size -= kernel_end - start;            \
					start = kernel_end;                    \
				}                                              \
				work;                                          \
			}                                                      \
			entry_addr += entry->size + sizeof(entry->size);       \
			entry = (multiboot_memory_map_t *)entry_addr;          \
		}                                                              \
	}

	TRAVERSE_MAP({
		if (page_stack == NULL) {
			page_stack = (uintptr_t *)((uintptr_t)start +
						   (uintptr_t)size);
		}
		// @TODO: change to LOG
		writef("Start:\t%X\nEnd:\t%X\nPage Stack: %x\n", start,
		       start + size, page_stack);
		bytes_for_stack += size >> 10;
	});

	page_stack -= bytes_for_stack >> 2;
	page_stack_top = page_stack;

	TRAVERSE_MAP({
		// @NOTE: - PAGE_SIZE to throw out less than full pages
		for (size_t i = start; i < start + size - PAGE_SIZE;
		     i += PAGE_SIZE) {
			// @NOTE: So we don't write over the page stack.
			if (i > (uintptr_t)page_stack - PAGE_SIZE &&
			    i < (uintptr_t)page_stack + bytes_for_stack) {
				break;
			}
			*--page_stack = i;
		}
	});

	writef("%u Free pages\n", page_stack_top - page_stack);

	// @TODO!!!: align the page_stack entries to page sizes (coincidence
	// right now?)
}

void *alloc_physical_page(void) {
	if (page_stack >= page_stack_top) {
		LOG("OUT OF MEMORY!\n");
		return NULL;
	}
	return (void *)*(page_stack++);
}

void free_physical_page(void *addr) {
	*page_stack_top++ = (uintptr_t)addr;
}

void dump_page_stack(void) {
	writef("====\tPage Stack\t====\n");
	for (uintptr_t *p = page_stack_top - 1; p >= page_stack; --p) {
		writef("%x\n", *p);
	}
	writef("====\tEnd Page Stack\t====\n");
}
