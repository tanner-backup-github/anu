#pragma once

#include <stddef.h>
#include <stdint.h>

void install_idt(void);
void set_idt_entry(size_t i, uint32_t offset, uint16_t selector, uint8_t flags);
