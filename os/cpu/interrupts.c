#include <stdint.h>

#include "interrupts.h"
#include "../drivers/screen.h"
#include "idt.h"
#include "pic.h"
#include "ioports.h"
void handleHardwareInterrupts_keyboard(uint8_t irq)
{
    __asm__ ("cli");
    print("Error: unhandled KEYBOARD hardware interrupt\n\0");
    sendPICEOI(irq);
    uint8_t scanPcode = port_byte_in(0x60);
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
    install_ir(0x20, (IRG_HANDLER) irq0 ); // 0x20 == 32
    install_ir(0x21, (IRG_HANDLER) irq1 );
    install_ir(0x22, (IRG_HANDLER) irq2 );
    install_ir(0x23, (IRG_HANDLER) irq3 );
    install_ir(0x24, (IRG_HANDLER) irq4 );
    install_ir(0x25, (IRG_HANDLER) irq5 );
    install_ir(0x26, (IRG_HANDLER) irq6 );
    install_ir(0x27, (IRG_HANDLER) irq7 );
    install_ir(0x28, (IRG_HANDLER) irq8 );
    install_ir(0x29, (IRG_HANDLER) irq9 );
    install_ir(0x30, (IRG_HANDLER) irq10 );
    install_ir(0x31, (IRG_HANDLER) irq11 );
    install_ir(0x32, (IRG_HANDLER) irq12 );
    install_ir(0x33, (IRG_HANDLER) irq13 );
    install_ir(0x34, (IRG_HANDLER) irq14 );
    install_ir(0x35, (IRG_HANDLER) irq15 );
}
