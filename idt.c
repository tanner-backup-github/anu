#include <common.h>

/* diagram of IDT entry */

uint64_t idt[256];
struct {
	uint16_t size;
	uint32_t offset;
} __attribute__ ((packed)) idtp = {
	sizeof(idt) - 1,
	(uint32_t) &idt,
};

extern void flush_idt(void);

void set_idt_entry(size_t i, uint32_t offset, uint16_t selector, uint8_t flags) {
	idt[i] = offset & 0xffff;
	idt[i] |= selector << 16;
	idt[i] |= (uint64_t) flags << 40;
	idt[i] |= (uint64_t) (offset >> 16) << 48;
}

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

void install_idt(void) {
	// @TODO: #define flags and change to bin numbers
	set_idt_entry(0, (uint32_t) isr0, 0x08, 0x8e);
	set_idt_entry(1, (uint32_t) isr1, 0x08, 0x8e);
	set_idt_entry(2, (uint32_t) isr2, 0x08, 0x8e);
	set_idt_entry(3, (uint32_t) isr3, 0x08, 0x8e);
	set_idt_entry(4, (uint32_t) isr4, 0x08, 0x8e);
	set_idt_entry(5, (uint32_t) isr5, 0x08, 0x8e);
	set_idt_entry(6, (uint32_t) isr6, 0x08, 0x8e);
	set_idt_entry(7, (uint32_t) isr7, 0x08, 0x8e);
	set_idt_entry(8, (uint32_t) isr8, 0x08, 0x8e);
	set_idt_entry(9, (uint32_t) isr9, 0x08, 0x8e);
	set_idt_entry(10, (uint32_t) isr10, 0x08, 0x8e);
	set_idt_entry(11, (uint32_t) isr11, 0x08, 0x8e);
	set_idt_entry(12, (uint32_t) isr12, 0x08, 0x8e);
	set_idt_entry(13, (uint32_t) isr13, 0x08, 0x8e);
	set_idt_entry(14, (uint32_t) isr14, 0x08, 0x8e);
	set_idt_entry(15, (uint32_t) isr15, 0x08, 0x8e);
	set_idt_entry(16, (uint32_t) isr16, 0x08, 0x8e);
	set_idt_entry(17, (uint32_t) isr17, 0x08, 0x8e);
	set_idt_entry(18, (uint32_t) isr18, 0x08, 0x8e);
	set_idt_entry(19, (uint32_t) isr19, 0x08, 0x8e);
	set_idt_entry(20, (uint32_t) isr20, 0x08, 0x8e);
	set_idt_entry(21, (uint32_t) isr21, 0x08, 0x8e);
	set_idt_entry(22, (uint32_t) isr22, 0x08, 0x8e);
	set_idt_entry(23, (uint32_t) isr23, 0x08, 0x8e);
	set_idt_entry(24, (uint32_t) isr24, 0x08, 0x8e);
	set_idt_entry(25, (uint32_t) isr25, 0x08, 0x8e);
	set_idt_entry(26, (uint32_t) isr26, 0x08, 0x8e);
	set_idt_entry(27, (uint32_t) isr27, 0x08, 0x8e);
	set_idt_entry(28, (uint32_t) isr28, 0x08, 0x8e);
	set_idt_entry(29, (uint32_t) isr29, 0x08, 0x8e);
	set_idt_entry(30, (uint32_t) isr30, 0x08, 0x8e);
	set_idt_entry(31, (uint32_t) isr31, 0x08, 0x8e);

	flush_idt();
}
