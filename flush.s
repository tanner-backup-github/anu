global flush_gdt
flush_gdt:
	extern gdtp
	lgdt [gdtp]
	mov ax, 0x10 ;; 0x10 is the offset of DS in GDT
	mov ds, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax
	mov es, ax
	jmp 0x08:flush2 ; 0x08 is offset of CS in GDT
flush2:
	ret

global flush_idt
flush_idt:
	extern idtp
	lidt [idtp]
	ret