#include <stdint.h>

#include "gdt.h"

static void gdt_install()
{
    __asm__("cli \n lgdt (_gdtr) \n sti");
}

int gdt_initialize()
{
    _gdtr.gdt_size = sizeof(GDTDescriptor) * MAX_GDT_DESCRIPTORS - 1;
    _gdtr.gdt_base_address = (uint32_t)&_gdt[0];

    _gdt[0].segment_limit = 0;
    _gdt[0].base_address_low = 0;
    _gdt[0].base_address_mid = 0;
    _gdt[0].descriptor_bit_flags = 0;
    _gdt[0].base_address_high = 0;

    _gdt[1].segment_limit = 0xffff;
    _gdt[1].base_address_low = 0;
    _gdt[1].base_address_mid = 0;
    _gdt[1].descriptor_bit_flags = 0x9acf;
    _gdt[1].base_address_high = 0;

    _gdt[2].segment_limit = 0xffff;
    _gdt[2].base_address_low = 0;
    _gdt[2].base_address_mid = 0;
    _gdt[2].descriptor_bit_flags = 0x92cf;
    _gdt[2].base_address_high = 0;

    gdt_install();
    return 0;
}
