MAGIC equ 0x1BADB002
FLAGS equ (1 << 0) | (1 << 1) | (1 << 6)
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
dd MAGIC
dd FLAGS
dd CHECKSUM

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
align 32
resb 8192
stack: