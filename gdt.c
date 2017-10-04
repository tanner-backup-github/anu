#include <common.h>

/*
   GDT Entry
     31                                     16 15                              0
                    Offset 0:15                           Size 0:15

		    
     63          56 55      52 51           48 47           40 39              32
      Offset 24:31     Flags     Size 16:19        Flags           Offset 16:23      
      
      Flags 40-47:
	 bit 0: accessed (set by CPU)
	 bit 1: Writable for DS and readable for CS (Readable is always allowed for DS and writable is never allowed for CS)
	 bit 2: Direction / conforming bit
	     Direction bit for DS: 0 = segment grows down, 1 = segment grows up
	     Conforming bit for CS: 1 = code in this segment can be executed from equal or lower privilege level.
	                            0 = code in this segment can only be executed from the ring set in the privilege bits.
         bit 3: Executable bit (1 = CS, 0 = DS)
	 bit 4: always 1
	 bit 5-6: privilege bits, contains ring level.
	 bit 7: present bit. always 1 for valid segments.

      Flags 52-55:
         bit 0: 0
         bit 1: 0
	 bit 2: selector size (0 = 16bit, 1 = 32bit)
	 bit 3: granularity (0 = 1b, 1 = 4kib)
*/
uint64_t gdt[3];
struct {
	uint16_t size;
	uint32_t offset;
} __attribute__ ((packed)) gdtp = {
	sizeof(gdt) - 1,
	(uint32_t) &gdt,
};

extern void flush_gdt(void);

void set_gdt_entry(size_t i, uint32_t offset, uint32_t size, uint8_t access_flags, uint8_t size_flags) {
	gdt[i] =  size & 0xffff;
	gdt[i] |= (offset & 0xffff) << 16;
	gdt[i] |= (uint64_t) ((offset >> 16) & 0xff) << 32;
	gdt[i] |= (uint64_t) ((size >> 16) & 0b1111) << 48;
	
	gdt[i] |= (uint64_t) access_flags << 40;
	gdt[i] |= (uint64_t) size_flags << 52;
}

void install_gdt(void) {
	set_gdt_entry(0, 0, 0, 0, 0);
	// @TODO: doc & turn into #define'd bin numbers
	set_gdt_entry(1, 0, 0xffffffff, 0x9a, 0b1100);
	set_gdt_entry(2, 0, 0xffffffff, 0x92, 0b1100);

	flush_gdt();
}
