%macro ISR_NO_ERRCODE 1
global isr%1
isr%1:
	cli
	;; we must push a dummy errcode because one isn't supplied
	push byte 0
	push byte %1
	jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
	cli
	push byte %1
	jmp isr_common_stub
%endmacro

;;  Division By Zero
ISR_NO_ERRCODE 0
;; Debug
ISR_NO_ERRCODE 1
;; NMI
ISR_NO_ERRCODE 2
;; Breakpoint
ISR_NO_ERRCODE 3
;; Overflow
ISR_NO_ERRCODE 4
;; Bound Range Exceeded
ISR_NO_ERRCODE 5
;; Invalid Opcode
ISR_NO_ERRCODE 6
;; Device Not Available
ISR_NO_ERRCODE 7
;; Double Fault
ISR_ERRCODE    8
;; Coprocessor Segment Overrun
ISR_NO_ERRCODE 9
;; Invalid TSS
ISR_ERRCODE    10
;; Segment Not Present
ISR_ERRCODE    11
;; General Protection Fault
ISR_ERRCODE    12
;; Stack-Segment Fault
ISR_ERRCODE    13
;; Page Fault
ISR_ERRCODE    14
;; Reserved
ISR_NO_ERRCODE 15
;; x87 Floating-Point
ISR_NO_ERRCODE 16
;; Alignment Check
ISR_ERRCODE    17
;; Machine Check
ISR_NO_ERRCODE 18
;; SIMD Floating-Point
ISR_NO_ERRCODE 19
;; Virtualization
ISR_NO_ERRCODE 20
;; Reserved	
ISR_NO_ERRCODE 21
;; Reserved
ISR_NO_ERRCODE 22
;; Reserved
ISR_NO_ERRCODE 23
;; Reserved
ISR_NO_ERRCODE 24
;; Reserved
ISR_NO_ERRCODE 25
;; Reserved	
ISR_NO_ERRCODE 26
;; Reserved
ISR_NO_ERRCODE 27
;; Reserved
ISR_NO_ERRCODE 28
;; Reserved
ISR_NO_ERRCODE 29
;; Security
ISR_ERRCODE 30
;; Reserved
ISR_NO_ERRCODE 31

extern isr_handler
;; 1. saves processor state
;; 2. sets up kernel mode segments
;; 3. calls isr_handler
;; 4. restores stack frame
isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	;; can't 'mov' literals directly into segment registers
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	;; can't directly call 'extern isr_handler'
	mov eax, isr_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	;; the error codes
	add esp, 8
	;; turns back on interrupts
	iret

%macro IRQ 2
global irq%1
irq%1:
	cli
        ;; we must push a dummy errcode because one isn't supplied
	push byte 0
	push byte %2
	jmp irq_common_stub
%endmacro

IRQ 0,  32
IRQ 1,  33
IRQ 2,  34
IRQ 3,  35
IRQ 4,  36
IRQ 5,  37
IRQ 6,  38
IRQ 7,  39
IRQ 8,  40
IRQ 9,  41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

extern irq_handler
;; 1. saves processor state
;; 2. sets up kernel mode segments
;; 3. calls isr_handler
;; 4. restores stack frame
irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	;; can't 'mov' literals directly into segment registers
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
        ;; can't directly call 'extern irq_handler'
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	;; the error codes
	add esp, 8
	;; turns back on interrupts
	iret
