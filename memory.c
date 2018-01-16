#include <common.h>

extern char __kernel_end;
uintptr_t kernel_end = (uintptr_t)&__kernel_end;

void memory_xxx(multiboot_info_t *mboot) {
	writef("%x\n", kernel_end);

	const uint32_t mmap_addr = mboot->mmap_addr;
	const uint32_t mmap_len = mboot->mmap_length;

	/*
	  struct multiboot_mmap_entry {
	  multiboot_uint32_t size;
	  multiboot_uint64_t addr;
	  multiboot_uint64_t len;
	  #define MULTIBOOT_MEMORY_AVAILABLE 1
	  #define MULTIBOOT_MEMORY_RESERVED 2
	  multiboot_uint32_t type;
	  } __attribute__((packed));
	  typedef struct multiboot_mmap_entry multiboot_memory_map_t;

	 */
	multiboot_memory_map_t *entry = (multiboot_memory_map_t *) mmap_addr;
	
	writef("%x\n", entry->type);
}
