#include <common.h>

void *irq_routines[16] = {NULL};
typedef void (* irq_routine)(registers *regs);

// IO ports
#define MASTER_PIC_COMMAND 0x20
#define MASTER_PIC_DATA 0x21
#define SLAVE_PIC_COMMAND 0xA0
#define SLAVE_PIC_DATA 0xA1
#define PIC_EOI 0x20
#define KEYBOARD_DATA 0x60

const uint8_t US_KEYBOARD[128] = {
	0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
	'=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
	'.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
	0,   0,    0,    0,   0,    0,   0,   0,   0,   '-', 0,   0,   0,
	'+', 0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   0,
};

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

uint64_t timer_ticks = 0;
void timer_handler(registers *regs) {
	(void)regs;
	++timer_ticks;

	// 55 ms of precision (% 18 = one sec boundary)
	if (timer_ticks % 18 == 0) {
		writef("TEST\n");
	}
}

void keyboard_handler(registers *regs) {
	(void)regs;
	uint8_t scancode = inb(KEYBOARD_DATA);
	
	if (!(scancode & 0b10000000) && US_KEYBOARD[scancode]) {
		writef("%c", US_KEYBOARD[scancode]);
	}
}

void install_irqs(void) {
	const uint8_t MASTER_OFFSET = 32;
	const uint8_t SLAVE_OFFSET = 40;

	// @NOTE: remap the PIC because in protected mode things overlapIRQ
	// (0-7 are mapped to IDT 8-15 which corresponds to double fault) bad

	// @NOTE: Master pic for IRQ0-IRQ7
	// @NOTE: Slave pic for IRQ8-IRQ15
	// @NOTE: ICW = Initialization Command Words
	// @NOTE: Technically waits should be here?

	// 0x10 + 0x01 = initialization + ICW4 (command right after 8086 mode
	// command) not needed (used to be outb(MASTER&SLAVE_DATA, 0);
	outb(MASTER_PIC_COMMAND, 0x11);
	outb(SLAVE_PIC_COMMAND, 0x11);

	outb(MASTER_PIC_DATA, MASTER_OFFSET);
	outb(SLAVE_PIC_DATA, SLAVE_OFFSET);

	// tells the master pic that there's a slave pic on IRQ2
	outb(MASTER_PIC_DATA, 4);
	// tells the slave pic it's identity (IRQ2)
	outb(SLAVE_PIC_DATA, 2);

	outb(MASTER_PIC_DATA, 1); // 8086 mode
	outb(SLAVE_PIC_DATA, 1);  // 8086 mode

	set_idt_entry(32, (uint32_t)irq0, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(33, (uint32_t)irq1, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(34, (uint32_t)irq2, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(35, (uint32_t)irq3, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(36, (uint32_t)irq4, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(37, (uint32_t)irq5, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(38, (uint32_t)irq6, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(39, (uint32_t)irq7, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(40, (uint32_t)irq8, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(41, (uint32_t)irq9, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(42, (uint32_t)irq10, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(43, (uint32_t)irq11, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(44, (uint32_t)irq12, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(45, (uint32_t)irq13, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(46, (uint32_t)irq14, DATA_SELECTOR, INT_GATE_FLAGS);
	set_idt_entry(47, (uint32_t)irq15, DATA_SELECTOR, INT_GATE_FLAGS);

        irq_routines[0] = timer_handler;
	irq_routines[1] = keyboard_handler;
}

void irq_handler(registers *regs) {
	irq_routine routine = irq_routines[regs->int_no - 32];

	if (routine) {
	        routine(regs);
	}

	if (regs->int_no >= 40) {
		// @NOTE: irq came from the slave pic so we have to acknowledge
		// it.
		outb(SLAVE_PIC_COMMAND, PIC_EOI);
	}
	outb(MASTER_PIC_COMMAND, PIC_EOI);
}
