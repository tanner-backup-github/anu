#include <common.h>

#define PAGE_PRESENT 0b1
#define PAGE_RW 0b10

void enable_paging(void) {
	uint32_t *page_dir = alloc_physical_page();
	uint32_t *page_table = alloc_physical_page();

	page_dir[0] = (uint32_t)page_table | (PAGE_PRESENT | PAGE_RW);

	write_cr3((uint32_t)page_dir);
	write_cr0(read_cr0() | (1 << 31));
}
