ENTRY(entry)

SECTIONS {

	.text ALIGN(4K) : {
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) : {
		*(.rodata)
	}

	.data ALIGN(4K) : {
		*(.data)
	}

	.bss ALIGN(4K) : {
		*(COMMON)
		*(.bss)
	}

	__kernel_end = ALIGN(4K);
	
}
