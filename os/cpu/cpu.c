#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "interrupts.h"

void initializeInterrupts()
{
    initializeIDT();
    initializePIC();
    timer_init();
    initializeHardwareInterrupts();
    __asm__ __volatile__ ("sti");
}
