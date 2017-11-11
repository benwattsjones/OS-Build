#ifndef CPU_IO_PORTS_H
#define CPU_IO_PORTS_H

#include <stdint.h>

unsigned char portByteIn(uint16_t port);
void portByteOut(uint16_t port, unsigned char data);

#endif
