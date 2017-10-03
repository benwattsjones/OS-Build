#ifndef DRIVERS_GDT_H
#define DRIVERS_GDT_H

#include <stdint.h>

#define MAX_GDT_DESCRIPTORS 3

typedef struct gdt_descriptor {
    uint16_t segment_limit;
    uint16_t base_address_low;
    uint8_t base_address_mid;
    uint16_t descriptor_bit_flags;
    uint8_t base_address_high;
} __attribute__((packed)) GDTDescriptor;

typedef struct gdtr_pointer {
    uint16_t gdt_size;
    uint32_t gdt_base_address;
} __attribute__((packed)) GDTRPointer;

// Global descriptor table (GDT)
static GDTDescriptor _gdt[MAX_GDT_DESCRIPTORS];
// GDTR pointer
static GDTRPointer _gdtr;

int gdt_initialize();

#endif
