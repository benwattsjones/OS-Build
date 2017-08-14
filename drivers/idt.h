#include <stdint.h>

#define X86_MAX_INTERRUPTS 256

struct idt_descriptor 
{
    // Low bits (0-16) of interrupt request (IRQ) routine address
    uint16_t irqAddrLow;
    // following two parts are same for all interrupts
    uint16_t codeSelector;
    uint8_t reserved;
    uint8_t bitFlags;
    // High bits (16-32) ofinterrupt request (IRQ) routine address
    uint16_t irqAddrHigh;
};

typedef void (*IRG_HANDLER)(void); 

void idt_initialize();
