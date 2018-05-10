;; http://wiki.osdev.org/CPU_Registers_x86
	
global read_cr0
read_cr0:
	mov eax, cr0
	retn

global print_cr0
print_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0,  eax
	pop ebp
	retn

global read_cr3
read_cr3:
	mov eax, cr3
	retn

global print_cr3
print_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	retn
