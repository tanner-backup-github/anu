#include <common.h>

// @TODO: doc
// @TODO: typedef irq functions
// @TODO: define pic stuff
// plural irq lol

void *irq_handlers[16] = { NULL };

void remap_pic(void) {
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
}

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

// @FIX: test code that needs to be ripped out
int32_t timer_ticks = 0;

void timer_handler(Registers *regs) {
	(void) regs;
	++timer_ticks;

	if (timer_ticks % 18 == 0) {
		/* writef("TEST"); */
	}
}

void install_irqs(void) {
	remap_pic();

        set_idt_entry(32, (uint32_t) irq0, 0x08, 0x8e);
        set_idt_entry(33, (uint32_t) irq1, 0x08, 0x8e);
        set_idt_entry(34, (uint32_t) irq2, 0x08, 0x8e);
        set_idt_entry(35, (uint32_t) irq3, 0x08, 0x8e);
        set_idt_entry(36, (uint32_t) irq4, 0x08, 0x8e);
        set_idt_entry(37, (uint32_t) irq5, 0x08, 0x8e);
        set_idt_entry(38, (uint32_t) irq6, 0x08, 0x8e);
        set_idt_entry(39, (uint32_t) irq7, 0x08, 0x8e);
        set_idt_entry(40, (uint32_t) irq8, 0x08, 0x8e);
        set_idt_entry(41, (uint32_t) irq9, 0x08, 0x8e);
        set_idt_entry(42, (uint32_t) irq10, 0x08, 0x8e);
        set_idt_entry(43, (uint32_t) irq11, 0x08, 0x8e);
        set_idt_entry(44, (uint32_t) irq12, 0x08, 0x8e);
        set_idt_entry(45, (uint32_t) irq13, 0x08, 0x8e);
        set_idt_entry(46, (uint32_t) irq14, 0x08, 0x8e);
        set_idt_entry(47, (uint32_t) irq15, 0x08, 0x8e);

	// @FIX: has no real function
	irq_handlers[0] = timer_handler;
}

void irq_handler(Registers *regs) {
	void (* handler)(Registers *regs) = irq_handlers[regs->int_no - 32];

	if (handler) {
		handler(regs);
	}

	if (regs->int_no >= 40) {
		// @NOTE: irq came from the slave pic so we have to acknowledge it.
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
}
