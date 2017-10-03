#include <stdint.h>

#include "screen.h" 
#include "idt.h"

static struct idt_descriptor _idt[X86_MAX_INTERRUPTS];

struct idtr
{
    // size of interrupt descriptor table (idt)
    uint16_t limit;
    // base address of idt
    uint32_t base;
} __attribute__((packed));

// static struct used to help define the cpu's idtr register
static struct idtr _idtr;

static void idt_install() 
{
    __asm__ __volatile__ ("lidt (_idtr)");
}

void idt_default_handler()
{
    print("Error: Unhandled Exception");
    for (;;) ;
}

void install_ir(uint32_t ir_code, IRG_HANDLER irq)
{
    if (ir_code > X86_MAX_INTERRUPTS - 1)
        return;
    if (!irq)
        return;
    // get base address of interrupt handler
    uint32_t irq_handler_addr = (uint32_t)&(*irq);
    // store base address into idt
    _idt[ir_code].irqAddrLow = irq_handler_addr & 0xffff;
    _idt[ir_code].irqAddrHigh = (irq_handler_addr >> 16) & 0xffff;
    _idt[ir_code].reserved = 0;
    _idt[ir_code].bitFlags = 0x8e;
    _idt[ir_code].codeSelector = 0x08;
}

void idt_initialize()
{
    _idtr.limit = sizeof(struct idt_descriptor) * X86_MAX_INTERRUPTS - 1;
    _idtr.base = (uint32_t)&_idt[0];
    // register default handlers
    int i;
    for (i = 0; i < X86_MAX_INTERRUPTS; i++)
        install_ir(i, (IRG_HANDLER) idt_default_handler);
    idt_install();
}
