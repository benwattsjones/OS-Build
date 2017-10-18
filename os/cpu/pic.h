#ifndef CPU_PIC_H
#define CPU_PIC_H

void initializePIC();
void sendPICEOI(uint8_t irq);

#endif
