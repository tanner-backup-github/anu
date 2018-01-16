global flush_gdt
flush_gdt:
	extern gdt_basep
	lgdt [gdt_basep]
	;; @NOTE: 0x08 is offset of CS (Code Segment) in GDT
	;; @NOTE: Far return to reload CS (we can't just "mov cs, 0x08")
	jmp 0x08:complete_flush
complete_flush:
	;; @NOTE: 0x10 is the offset of DS (Data Segment) in GDT
	;; @NOTE: Repointing all of these registers to the flat layout at GDT offset 0x10
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax
	mov es, ax
	ret

global flush_idt
flush_idt:
	extern idt_basep
	lidt [idt_basep]
	ret
