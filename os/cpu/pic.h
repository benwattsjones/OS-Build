#ifndef DRIVERS_PIC_H
#define DRIVERS_PIC_H

void initializePIC();
void sendPICEOI(uint8_t irq);

#endif
