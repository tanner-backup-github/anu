#include <common.h>

/*
   @NOTE: GDT Entry
     31                                     16 15                              0
		    Offset 0:15                           Size 0:15


     63          56 55      52 51           48 47           40 39 32
      Offset 24:31     Flags     Size 16:19         Flags         Offset 16:23

      Flags 40-47:
	 bit 0: accessed (set by CPU)
	 bit 1: Writable for DS and readable for CS (Readable is always allowed
   for DS and writable is never allowed for CS)
	 bit 2: Direction / conforming bit
	     Direction bit for DS: 0 = segment grows down, 1 = segment grows up
	     Conforming bit for CS: 1 = code in this segment can be executed
   from equal or lower privilege level.
				    0 = code in this segment can only be
   executed from the ring set in the privilege bits.
	 bit 3: Executable bit (1 = CS, 0 = DS)
	 bit 4: always 1
	 bit 5-6: privilege bits, contains ring level.
	 bit 7: present bit. always 1 for valid segments.

      Flags 52-55:
	 bit 0: 0
	 bit 1: 0
	 bit 2: selector size (0 = 16bit, 1 = 32bit)
	 bit 3: granularity (0 = 1b, 1 = 4KiB)
*/
uint64_t gdt[3];
struct {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) gdt_basep = {
	sizeof(gdt) - 1,
	(uint32_t)&gdt,
};

extern void flush_gdt(void);

// access flags
// @NOTE: DESCRIPTOR_PRESENT defined in common.h as 0b10000000
#define DPL_KERNEL 0b00000000
#define DPL_USER 0b01100000
#define MANDATORY_ONE 0b00010000
#define GDT_EXEC_SELECTOR 0b00001000
#define GDT_DATA_SELECTOR                                                      \
	0b00000000 // @NOTE: GDT_* because conflicted with common.h
		   // DATA_SELECTOR
#define DATA_GROWS_UP 0b00000000
#define DATA_GROWNS_DOWN 0b00000100
#define CODE_EXEC_LOWER 0b00000100
#define CODE_EXEC_EXACT 0b00000000
#define DATA_CODE_WR 0b00000010
#define DATA_CODE_NOT_WR 0b00000000
#define NOT_ACCESSED 0b00000000

// size flags
#define PAGE_GRANULARITY 0b1000
#define BYTE_GRANULARITY 0b0000
#define SIZE_PROT_16BIT 0b0000
#define SIZE_PROT_32BIT 0b0100

#define CODE_ACCESS_FLAGS                                                      \
	DESCRIPTOR_PRESENT | DPL_KERNEL | MANDATORY_ONE | GDT_EXEC_SELECTOR |  \
		CODE_EXEC_EXACT | DATA_CODE_WR | NOT_ACCESSED
#define DATA_ACCESS_FLAGS                                                      \
	DESCRIPTOR_PRESENT | DPL_KERNEL | MANDATORY_ONE | GDT_DATA_SELECTOR |  \
		DATA_GROWS_UP | DATA_CODE_WR | NOT_ACCESSED
#define BOTH_SIZE_FLAGS PAGE_GRANULARITY | SIZE_PROT_32BIT

void set_gdt_entry(size_t i, uint32_t offset, uint32_t size,
		   uint8_t access_flags, uint8_t size_flags) {
	gdt[i] = size & 0xffff;
	gdt[i] |= (offset & 0xffff) << 16;
	gdt[i] |= (uint64_t)((offset >> 16) & 0xff) << 32;
	gdt[i] |= (uint64_t)((size >> 16) & 0b1111) << 48;

	gdt[i] |= (uint64_t)access_flags << 40;
	gdt[i] |= (uint64_t)size_flags << 52;
}

void install_gdt(void) {
	// @NOTE: The null descriptor is never referenced by the processor.
	// Needed for certain emulators.
	set_gdt_entry(0, 0, 0, 0, 0);

	set_gdt_entry(1, 0, 0xffffffff, CODE_ACCESS_FLAGS, BOTH_SIZE_FLAGS);
	set_gdt_entry(2, 0, 0xffffffff, DATA_ACCESS_FLAGS, BOTH_SIZE_FLAGS);

	// @TODO: CS & DS descriptors for user mode

	flush_gdt();
}
