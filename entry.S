MAGIC equ 0x1badb002
FLAGS equ (1 << 0) | (1 << 1) | (1 << 6)
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
dd MAGIC
dd FLAGS
dd CHECKSUM

;; @NOTE: video modes

KERNEL_VIRT_BASE equ 0xc0000000
KERNEL_PAGE_NUM equ (KERNEL_VIRT_BASE >> 22)

section .data
align 0x1000
boot_page_dir:
	dd 0x83
	times (KERNEL_PAGE_NUM - 1) dd 0
	dd 0x83
	times (1024 - KERNEL_PAGE_NUM - 1) dd 0
	
section .text
global entry
entry equ (_entry - 0xc0000000)	
_entry:
	mov ecx, (boot_page_dir - KERNEL_VIRT_BASE)
	mov cr3, ecx

	mov ecx, cr4
	or ecx, 0x10
	mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000000 ;; enable paging
	mov cr0, ecx

	lea ecx, [start_higher_half]
	jmp ecx

start_higher_half:
	mov dword [boot_page_dir], 0
	invlpg [0]
	
        mov esp, stack

	add ebx, KERNEL_VIRT_BASE
        push ebx
        push eax
	
        extern kmain
        call kmain

global hang
hang:
        cli
        hlt
        jmp hang
	
section .bss
;; @TODO: doc why 32 bit aligned
align 32
resb 0x2000
stack:
