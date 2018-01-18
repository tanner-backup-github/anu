#include <common.h>

void alloc_physical_page_test(void) {
	dump_page_stack();

	char *z = NULL;
	for (size_t i = 0; i < 12; ++i) {
		char *a = alloc_physical_page();
		if ((i % 4) == 0) {
			writef("Was freed: %x\n", a);
		 	free_physical_page(a);
		} else {
			z = a;
			writef("Wasn't freed: %x\n", a);
		}
	}
	
	dump_page_stack();

	writef("%x\n", alloc_physical_page());
	writef("%x\n", alloc_physical_page());
	writef("%x\n", alloc_physical_page());
	writef("%x\n", alloc_physical_page());

	writef("%x\n", alloc_physical_page());
	writef("%x\n", alloc_physical_page());
	writef("%x\n", alloc_physical_page());
	writef("%x\n", alloc_physical_page());
	writef("%x\n", alloc_physical_page());
	
	free_physical_page(z);

	dump_page_stack();

	writef("%x\n", alloc_physical_page());
	
}
