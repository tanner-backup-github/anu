ENTRY(entry)

SECTIONS {

	. = 0xC0100000;

	.text ALIGN(4K) : AT(ADDR(.text) - 0xc0000000) {
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xc0000000) {
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - 0xc0000000) {
		*(.data)
	}

	.bss ALIGN(4K): AT(ADDR(.bss) - 0xc0000000) {
		*(COMMON)
		*(.bss)
	}

	__kernel_virt_end = ALIGN(4K);
	__kernel_phys_end = . - 0xc0000000;

}
