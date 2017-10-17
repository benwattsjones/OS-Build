#include <stdint.h>

#include "interrupts.h"
#include "../drivers/screen.h"
#include "idt.h"
#include "pic.h"
#include "ioports.h"
#include "../drivers/keyboard.h"

/* This file sets up hardware interrupt handlers using the functions from 
 * 'idt.c'. These are set to interrupts 32-48, as previous interrupts either
 * have use cases defined by the cpu (e.g. 0 is divide by zero) or are 
 * reserved.
 * The hardware interrupts fire as a result of the PIC, coded for in pic.c and 
 * also pit.c for the case of the timer. Different devices cause the PIC to 
 * fire different IRQs. We will remap these to interrupts in the idt in pic.c.
 * The defaults are listed below. Note that IRQ being the timer explains why 
 * interrupts had to be disabled in protected mode, as in protected mode int 0 
 * is a divide by zero error. This would cause a double fault and the OS would
 * restart. Once the interrupts have been handled (and after each is called),
 * we must enable interrupts with 'sti'.
 * IRQ0  - Timer
 * IRQ1  - Keyboard
 * IRQ2  - Cascade for 8259A slave controller
 * IRQ3  - Serial port 2
 * IRQ4  - Serial port 1
 * IRQ5  - AT systems: parrallel port 2. PS/2 systems: reserved
 * IRQ6  - Diskette drive
 * IRQ7  - Parrrallel port 1
 * IRQ8  - CMOS Real time clock
 * IRQ9  - CGA vertical retrace
 * IRQ10 - Reserved
 * IRQ11 - Reserved
 * IRQ12 - AT systems: reserved. PS/2: auxillary device
 * IRQ13 - FPU
 * IRQ14 - Hard disk controller
 * IRQ15 - Reserved
 * Note that after each hardware interrupt, an end of interrupt (EOI) is sent
 * to the pic, allowing another interrupt to be fired.
 */

void handleHardwareInterrupts_keyboard(uint8_t irq)
{
    __asm__ ("cli");
    uint8_t scan_code = portByteIn(0x60);
    printKeyInput(scan_code);
    sendPICEOI(irq);
    __asm__ ("sti");
}

void handleHardwareInterrupts(uint8_t irq)
{
    __asm__ ("cli");
    print("Error: unhandled hardware interrupt\n\0");
    sendPICEOI(irq);
    __asm__ ("sti");
}

static uint8_t c = 0;
void handleHardwareInterrupts_timer(uint8_t irq)
{
    __asm__ ("cli");
    c++;
    if (c >= 20) {
        print(".\0");
        c = 0;
    }
    sendPICEOI(irq);
    __asm__ ("sti");
}

void irq0() { handleHardwareInterrupts_timer(0); }
void irq1() { handleHardwareInterrupts_keyboard(1); }
void irq2() { handleHardwareInterrupts(2); }
void irq3() { handleHardwareInterrupts(3); }
void irq4() { handleHardwareInterrupts(4); }
void irq5() { handleHardwareInterrupts(5); }
void irq6() { handleHardwareInterrupts(6); }
void irq7() { handleHardwareInterrupts(7); }
void irq8() { handleHardwareInterrupts(8); }
void irq9() { handleHardwareInterrupts(9); }
void irq10() { handleHardwareInterrupts(10); }
void irq11() { handleHardwareInterrupts(11); }
void irq12() { handleHardwareInterrupts(12); }
void irq13() { handleHardwareInterrupts(13); }
void irq14() { handleHardwareInterrupts(14); }
void irq15() { handleHardwareInterrupts(15); }

void initializeHardwareInterrupts()
{
    installISR(0x20, (IRG_HANDLER) irq0 ); // 0x20 == 32
    installISR(0x21, (IRG_HANDLER) irq1 );
    installISR(0x22, (IRG_HANDLER) irq2 );
    installISR(0x23, (IRG_HANDLER) irq3 );
    installISR(0x24, (IRG_HANDLER) irq4 );
    installISR(0x25, (IRG_HANDLER) irq5 );
    installISR(0x26, (IRG_HANDLER) irq6 );
    installISR(0x27, (IRG_HANDLER) irq7 );
    installISR(0x28, (IRG_HANDLER) irq8 );
    installISR(0x29, (IRG_HANDLER) irq9 );
    installISR(0x30, (IRG_HANDLER) irq10 );
    installISR(0x31, (IRG_HANDLER) irq11 );
    installISR(0x32, (IRG_HANDLER) irq12 );
    installISR(0x33, (IRG_HANDLER) irq13 );
    installISR(0x34, (IRG_HANDLER) irq14 );
    installISR(0x35, (IRG_HANDLER) irq15 );
}

