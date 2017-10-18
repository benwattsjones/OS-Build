#include <stdint.h>

#include "idt.h"
#include "../drivers/screen.h" 

/* There are hardware and software interrupts. This file initialises software
 * interrupts (e.g. INT $0x80). When an interrupt is fired, its interrupt 
 * handler must be called. Like the GDT, there is a IDT (interrupt descriptor
 * table). The IDT is an array of length 256 (one element for each interrupt/
 * interrupt handler). Each element is of size 8 bytes, and amoungst other 
 * information, contains the address of the interrupt hander function. To 
 * locate the IDT, the interrupt descriptor table register (IDTR) is loaded
 * with the start address of the IDT and its size. Thus, for a given interrupt
 * index, the cpu can locate the appropriate interrupt handler (IR - interrupt
 * routine; ISR - interrupt service routine) using the formula:
 * IDTR.baseAddress + index * 8.
 * We will setup all interrupts to have the same default handler.
 */

static struct IDTDescriptor _idt[X86_MAX_INTERRUPTS];

struct idtr
{
    // size of interrupt descriptor table (idt)
    uint16_t limit;
    // base address of idt
    uint32_t base;
} __attribute__((packed));

// static struct used to help define the cpu's idtr register
static struct idtr _idtr;

static void installIDT() 
{
    __asm__ __volatile__ ("lidt (_idtr)");
}

void idt_default_handler()
{
    print("Error: Unhandled Exception\n\0");
    __asm__ ("sti");
}

void installISR(uint32_t ir_code, IRG_HANDLER irq)
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

void initializeIDT()
{
    _idtr.limit = sizeof(struct IDTDescriptor) * X86_MAX_INTERRUPTS - 1;
    _idtr.base = (uint32_t)&_idt[0];
    // register default handlers
    int i;
    for (i = 0; i < X86_MAX_INTERRUPTS; i++)
        installISR(i, (IRG_HANDLER) idt_default_handler);
    installIDT();
}
