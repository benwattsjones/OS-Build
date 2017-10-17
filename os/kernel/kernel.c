// This is the main kernel file. It is entered at main()

#include "../drivers/screen.h"
#include "../cpu/cpu.h"

int main() 
{
    clearScreen();
    initializeInterrupts();

    char *pstr = "Welcome to the OS!\n\0";
    printAt(pstr, -1, -1, RED_ORANGE_FOREGROUND);

    for (;;)
        __asm__ ("hlt");
    return 0;
}
