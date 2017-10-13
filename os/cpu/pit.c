#include <stdint.h>

#include "ioports.h"

void timer_init()
{
    port_byte_out(0x43, 0x36);
    uint16_t count = 1193180 / 100;
    port_byte_out(0x40, count & 0x0f);
    port_byte_out(0x40, count >> 8);
}
