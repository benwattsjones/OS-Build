#include <stdint.h>

#include "interrupts.h"
#include "screen.h"
#include "idt.h"
#include "pic.h"
void handleHardwareInterrupts_TIMER(uint8_t irq)
{
    print("Error: unhandled TIMER hardware interrupt\n\0");
//    char ascii_num[] = {48+irq, 10, 0};
  //  char *ascii_num2 = ascii_num;
   // print(ascii_num2);
    sendPICEOI(irq);
}

void handleHardwareInterrupts(uint8_t irq)
{
    print("Error: unhandled hardware interrupt\n\0");
//    char ascii_num[] = {48+irq, 10, 0};
  //  char *ascii_num2 = ascii_num;
   // print(ascii_num2);
    sendPICEOI(irq);
}

void irq0() { handleHardwareInterrupts(0); }
void irq1() { handleHardwareInterrupts(1); }
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
void irq12() { handleHardwareInterrupts_TIMER(12); }
void irq13() { handleHardwareInterrupts(13); }
void irq14() { handleHardwareInterrupts(14); }
void irq15() { handleHardwareInterrupts(15); }

void initializeHardwareInterrupts()
{
    install_ir(20, (IRG_HANDLER) irq0 );
    install_ir(21, (IRG_HANDLER) irq1 );
    install_ir(22, (IRG_HANDLER) irq2 );
    install_ir(23, (IRG_HANDLER) irq3 );
    install_ir(24, (IRG_HANDLER) irq4 );
    install_ir(25, (IRG_HANDLER) irq5 );
    install_ir(26, (IRG_HANDLER) irq6 );
    install_ir(27, (IRG_HANDLER) irq7 );
    install_ir(28, (IRG_HANDLER) irq8 );
    install_ir(29, (IRG_HANDLER) irq9 );
    install_ir(30, (IRG_HANDLER) irq10 );
    install_ir(31, (IRG_HANDLER) irq11 );
    install_ir(32, (IRG_HANDLER) irq12 );
    install_ir(33, (IRG_HANDLER) irq13 );
    install_ir(34, (IRG_HANDLER) irq14 );
    install_ir(35, (IRG_HANDLER) irq15 );
}
