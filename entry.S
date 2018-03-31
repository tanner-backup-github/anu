MAGIC equ 0x1badb002
FLAGS equ (1 << 0) | (1 << 1) | (1 << 6)
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
dd MAGIC
dd FLAGS
dd CHECKSUM

;; @NOTE: video modes
	
section .text
global entry
entry:	
        mov esp, stack

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
