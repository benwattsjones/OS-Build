#include "cpu.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "interrupts.h"

/* This file collects together the initialization funcions of this file
 * to create a single point of entry for the kernel to call.
 */

void initializeInterrupts()
{
    initializeIDT();
    initializePIC();
    timer_init();
    initializeHardwareInterrupts();
    __asm__ __volatile__ ("sti");
}
