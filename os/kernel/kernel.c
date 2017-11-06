// This is the main kernel file. It is entered at main()

#include "../drivers/screen.h"
#include "../cpu/gdt.h"
#include "../cpu/cpu.h"

int main() 
{
    clearScreen();
    //setCursor(160); // make cursor start on second line where typing starts
    initializeGDT();
    initializeInterrupts();

    char *pstr = "Welcome to the OS!\n\0";
    printHeader(pstr, 0, RED_ORANGE_FOREGROUND);

    for (;;)
        __asm__ ("hlt");
    return 0;
}
