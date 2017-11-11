/* This is the main kernel file. It is entered at main() and called by 
 * kernel_entry.asm.
 */

#include "../drivers/screen.h"
#include "../cpu/gdt.h"
#include "../cpu/cpu.h"

int main() 
{
    clearScreen();
    initializeGDT();
    initializeInterrupts();

    char *pstr = "Welcome to the OS!\n\0";
    printHeader(pstr, 0, RED_ORANGE_FOREGROUND);

    for (;;)
        __asm__ ("hlt");
    return 0;
}
