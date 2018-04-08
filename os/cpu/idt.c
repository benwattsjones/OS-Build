/* OS-Build. Copyright (C) 2017, 2018. Ben Watts-Jones.
 *
 * This program is distributed under the GNU General Public License Version 3 
 * (GPLv3) as published by the Free Software Foundation. You should have 
 * recieved a copy of the GPLv3 licence along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MECHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GPLv3 license for more details.
 */

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

#define X86_MAX_INTERRUPTS 256
#define X86_IDT_BIT_FLAGS 0x8e
#define GDT_CODE_SELECTOR 0x08

struct IDTDescriptor 
{
    // Low bits (0-15) of interrupt request (IRQ) routine address
    uint16_t irq_addr_low;
    // Following three parts are same for all interrupts
    // Set to 0x08 by GDT
    uint16_t code_selector;
    // Must be zero
    uint8_t reserved;
    // Set to 10001110b (0x8e)
    //           +++++- Shows it is a 32 bit descriptor of interrupt gate. 
    //         ++------ Tells cpu to execute IR in ring 0 privalage.
    //        +-------- Shows this segment is present
    uint8_t bit_flags;
    // High bits (16-32) ofinterrupt request (IRQ) routine address
    uint16_t irq_addr_high;
} __attribute__((packed));

static struct IDTDescriptor _idt[X86_MAX_INTERRUPTS];

struct idtr {
    uint16_t idt_size;
    uint32_t idt_base_address;
} __attribute__((packed));

static struct idtr _idtr;

static void installIDT() 
{
    __asm__ __volatile__ ("lidt (_idtr)");
}

void handleInterruptDefault()
{
    print("Error: Unhandled Exception\n\0");
    __asm__ ("sti");
}

void installISR(uint32_t irq_number, IRG_HANDLER irq_routine)
{
    if (irq_number > X86_MAX_INTERRUPTS - 1 || !(irq_routine))
        return;
    uint32_t irq_handler_addr = (uint32_t)&(*irq_routine);
    _idt[irq_number].irq_addr_low = irq_handler_addr & 0xffff;
    _idt[irq_number].irq_addr_high = (irq_handler_addr >> 16) & 0xffff;
    _idt[irq_number].reserved = 0;
    _idt[irq_number].bit_flags = X86_IDT_BIT_FLAGS;
    _idt[irq_number].code_selector = GDT_CODE_SELECTOR;
}

void initializeIDT()
{
    _idtr.idt_size = sizeof(struct IDTDescriptor) * X86_MAX_INTERRUPTS - 1;
    _idtr.idt_base_address = (uint32_t)&_idt[0];
    int interrupt_num;
    for (interrupt_num = 0; interrupt_num < X86_MAX_INTERRUPTS; interrupt_num++)
        installISR(interrupt_num, (IRG_HANDLER) handleInterruptDefault);
    installIDT();
}
