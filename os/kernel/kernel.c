// This is the main kernel file. It is entered at main()

#include "../drivers/screen.h"
#include "../cpu/cpu.h"

int main() 
{
    clearScreen();
    initializeInterrupts();
    char *pstr = "hello my name is ben\n\0";
    int i;
    for (i = 0; i < 16; i++) {
        printAt(pstr, -1, -1, i);
    }
    printAt(pstr, -1, -1, 0b01001010);
    printAt(pstr, -1, -1, 0b10001100);
    print(pstr);
    for (i = 0; i < 10; i++) {
        print(pstr);
    }
    __asm__ ("int $0");
    for (;;)
        __asm__ ("hlt");
    return 0;
}
