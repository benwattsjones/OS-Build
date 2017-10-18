#ifndef CPU_IDT_H
#define CPU_IDT_H

#include <stdint.h>

#define X86_MAX_INTERRUPTS 256

struct IDTDescriptor 
{
    // Low bits (0-15) of interrupt request (IRQ) routine address
    uint16_t irqAddrLow;
    // Following three parts are same for all interrupts
    // Set to 0x08 by GDT
    uint16_t codeSelector;
    // Must be zero
    uint8_t reserved;
    // Set to 10001110b (0x8e)
    //           +++++- Shows it is a 32 bit descriptor of interrupt gate. 
    //         ++------ Tells cpu to execute IR in ring 0 privalage.
    //        +-------- Shows this segment is present
    uint8_t bitFlags;
    // High bits (16-32) ofinterrupt request (IRQ) routine address
    uint16_t irqAddrHigh;
} __attribute__((packed));

// Function pointer type for addressing of interrupt handlers
typedef void (*IRG_HANDLER)(void); 

void installISR(uint32_t ir_code, IRG_HANDLER irq);

void initializeIDT();

#endif
