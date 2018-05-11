#pragma once

#include "multiboot.h"

void init_phys_mem(multiboot_info_t *mboot);
void *malloc_phys_page(void);
void free_phys_page(void *addr);
void print_allocator_bitmap(void);
