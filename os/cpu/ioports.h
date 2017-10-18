// This file is a header file for functions written in assembly.
// Must be seperately converted to object files and linked together.

#ifndef CPU_IO_PORTS_H
#define CPU_IO_PORTS_H

#include <stdint.h>

// Returns value at port address. Note: the port address is an I/O address
// which is memory-mapped on RAM to a particular register of a particular
// external device.
unsigned char portByteIn(uint16_t port);
// Writes value in 'data' parameter to port address.
void portByteOut(uint16_t port, unsigned char data);

#endif
