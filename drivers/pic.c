#include <stdint.h>

#include "ioports.h"

#define PIC_1_CTRL 0x20  // primary PIC control register
#define PIC_2_CTRL 0xa0  // secondary PIC control register
#define PIC_1_DATA 0x21  // primary PIC data register
#define PIC_2_DATA 0xa1  // secondary PIC data registe

#define ICW_1 0x11  // 00010001 binary. Enables initialization mode and ICW 4

#define IRQ_0 0x20  // IRQs 0-7 mapped to interrupts 0x20-0x27
#define IRQ_8 0x28  // IRQs 8-15 mapped to use interrupts 0x28-0x36

#define ICW_3_PRI 0x04  // bit 2 so use IR line 2 to comunicate with secondary PIC
#define ICW_3_SEC 0x02  // use line 2 to communicate with primary PIC

#define ICW_4 0x01  // bit 0 enables 80x86 mode

#define PIC_EOI 0x20

void initializePIC()
{
    port_byte_out(PIC_1_CTRL, ICW_1);
    port_byte_out(PIC_2_CTRL, ICW_1);
    port_byte_out(PIC_1_DATA, IRQ_0);
    port_byte_out(PIC_2_DATA, IRQ_8);
    port_byte_out(PIC_1_DATA, ICW_3_PRI);
    port_byte_out(PIC_2_DATA, ICW_3_SEC);
    port_byte_out(PIC_1_DATA, ICW_4);
    port_byte_out(PIC_2_DATA, ICW_4);
    port_byte_out(PIC_1_DATA, 0);
    port_byte_out(PIC_2_DATA, 0);
}

void sendPICEOI(uint8_t irq)
{
    if (irq >=8)
        port_byte_out(PIC_2_CTRL, PIC_EOI);
    port_byte_out(PIC_1_CTRL, PIC_EOI);
}
