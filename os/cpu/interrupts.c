#include <stdint.h>

#include "interrupts.h"
#include "../drivers/screen.h"
#include "idt.h"
#include "pic.h"
#include "ioports.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"

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

void handleHardwareInterrupts_timer(uint8_t irq)
{
    __asm__ ("cli");
    printTimeElapsed();
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
    installISR(0x2a, (IRG_HANDLER) irq10 );
    installISR(0x2b, (IRG_HANDLER) irq11 );
    installISR(0x2c, (IRG_HANDLER) irq12 );
    installISR(0x2d, (IRG_HANDLER) irq13 );
    installISR(0x2e, (IRG_HANDLER) irq14 );
    installISR(0x2f, (IRG_HANDLER) irq15 );
}

/* SOFTWARE INTERRUPTS */

void isr0() { __asm__("cli"); print("Exception 0\n\0"); __asm__("sti"); }
void isr1() { __asm__("cli"); print("Exception 1\n\0"); __asm__("sti"); }
void isr2() { __asm__("cli"); print("Exception 2\n\0"); __asm__("sti"); }
void isr3() { __asm__("cli"); print("Exception 3\n\0"); __asm__("sti"); }
void isr4() { __asm__("cli"); print("Exception 4\n\0"); __asm__("sti"); }
void isr5() { __asm__("cli"); print("Exception 5\n\0"); __asm__("sti"); }
void isr6() { __asm__("cli"); print("Exception 6\n\0"); __asm__("sti"); }
void isr7() { __asm__("cli"); print("Exception 7\n\0"); __asm__("sti"); }
void isr8() { __asm__("cli"); print("Exception 8\n\0"); __asm__("sti"); }
void isr9() { __asm__("cli"); print("Exception 9\n\0"); __asm__("sti"); }
void isr10() { __asm__("cli"); print("Exception 10\n\0"); __asm__("sti"); }
void isr11() { __asm__("cli"); print("Exception 11\n\0"); __asm__("sti"); }
void isr12() { __asm__("cli"); print("Exception 12\n\0"); __asm__("sti"); }
void isr13() { __asm__("cli"); print("Exception 13\n\0"); __asm__("sti"); }
void isr14(uint32_t err_code) 
{ 
    __asm__("cli"); 
    char err_msg[] = {'E', 'x', 'c', 'e', 'p', 't', 'i', 'o', 'n', ' ', '1', '4', ':', '0', '\n', '\0'};
    err_msg[13] += err_code;
    char *err_msg2 = (char *) err_msg;
    print(err_msg2); 
    __asm__("sti"); 
}
void isr15() { __asm__("cli"); print("Exception 15\n\0"); __asm__("sti"); }
void isr16() { __asm__("cli"); print("Exception 16\n\0"); __asm__("sti"); }
void isr17() { __asm__("cli"); print("Exception 17\n\0"); __asm__("sti"); }
void isr18() { __asm__("cli"); print("Exception 18\n\0"); __asm__("sti"); }
void isr19() { __asm__("cli"); print("Exception 19\n\0"); __asm__("sti"); }
void isr20() { __asm__("cli"); print("Exception 20\n\0"); __asm__("sti"); }
void isr21() { __asm__("cli"); print("Exception 21\n\0"); __asm__("sti"); }
void isr22() { __asm__("cli"); print("Exception 22\n\0"); __asm__("sti"); }
void isr23() { __asm__("cli"); print("Exception 23\n\0"); __asm__("sti"); }
void isr24() { __asm__("cli"); print("Exception 24\n\0"); __asm__("sti"); }
void isr25() { __asm__("cli"); print("Exception 25\n\0"); __asm__("sti"); }
void isr26() { __asm__("cli"); print("Exception 26\n\0"); __asm__("sti"); }
void isr27() { __asm__("cli"); print("Exception 27\n\0"); __asm__("sti"); }
void isr28() { __asm__("cli"); print("Exception 28\n\0"); __asm__("sti"); }
void isr29() { __asm__("cli"); print("Exception 29\n\0"); __asm__("sti"); }
void isr30() { __asm__("cli"); print("Exception 30\n\0"); __asm__("sti"); }
void isr31() { __asm__("cli"); print("Exception 31\n\0"); __asm__("sti"); }

void initializeSoftwareInterrupts()
{
    installISR(0, (IRG_HANDLER) isr0 );
    installISR(1, (IRG_HANDLER) isr1 );
    installISR(2, (IRG_HANDLER) isr2 );
    installISR(3, (IRG_HANDLER) isr3 );
    installISR(4, (IRG_HANDLER) isr4 );
    installISR(5, (IRG_HANDLER) isr5 );
    installISR(6, (IRG_HANDLER) isr6 );
    installISR(7, (IRG_HANDLER) isr7 );
    installISR(8, (IRG_HANDLER) isr8 );
    installISR(9, (IRG_HANDLER) isr9 );
    installISR(10, (IRG_HANDLER) isr10 );
    installISR(11, (IRG_HANDLER) isr11 );
    installISR(12, (IRG_HANDLER) isr12 );
    installISR(13, (IRG_HANDLER) isr13 );
    installISR(14, (IRG_HANDLER) isr14 );
    installISR(15, (IRG_HANDLER) isr15 );
    installISR(16, (IRG_HANDLER) isr16 );
    installISR(17, (IRG_HANDLER) isr17 );
    installISR(18, (IRG_HANDLER) isr18 );
    installISR(19, (IRG_HANDLER) isr19 );
    installISR(20, (IRG_HANDLER) isr20 );
    installISR(21, (IRG_HANDLER) isr21 );
    installISR(22, (IRG_HANDLER) isr22 );
    installISR(23, (IRG_HANDLER) isr23 );
    installISR(24, (IRG_HANDLER) isr24 );
    installISR(25, (IRG_HANDLER) isr25 );
    installISR(26, (IRG_HANDLER) isr26 );
    installISR(27, (IRG_HANDLER) isr27 );
    installISR(28, (IRG_HANDLER) isr28 );
    installISR(29, (IRG_HANDLER) isr29 );
    installISR(30, (IRG_HANDLER) isr30 );
    installISR(31, (IRG_HANDLER) isr31 );
}

