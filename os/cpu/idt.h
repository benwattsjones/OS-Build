#ifndef CPU_IDT_H
#define CPU_IDT_H

#include <stdint.h>

// Function pointer type for addressing of interrupt handlers
typedef void (*IRG_HANDLER)(void); 
void installISR(uint32_t ir_code, IRG_HANDLER irq);
void initializeIDT();

#endif
