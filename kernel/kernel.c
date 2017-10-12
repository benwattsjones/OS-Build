// This is the main kernel file. It is entered at main()

#include "../drivers/screen.h"
#include "../drivers/idt.h"
#include "../drivers/pic.h"
#include "../drivers/pit.h"
#include "../drivers/interrupts.h"

int main() 
{
    idt_initialize();
    clear_screen();
    initializePIC();
    timer_init();
    initializeHardwareInterrupts();
    char *pstr = "hello my name is ben\n\0";
    int i;
    for (i = 0; i < 16; i++) {
        print_at(pstr, -1, -1, i);
    }
    print_at(pstr, -1, -1, 0b01001010);
    print_at(pstr, -1, -1, 0b10001100);
    print(pstr);
    for (i = 0; i < 10; i++) {
        print(pstr);
    }
//    __asm__ ("int $0");
    __asm__ __volatile__ ("sti");
//    __asm__ ("int $80");
//    __asm__ ("int $85");
    for (;;)
        __asm__ ("hlt");
    return 0;
}
