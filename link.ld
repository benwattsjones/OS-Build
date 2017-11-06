
OUTPUT_FORMAT("binary")

ENTRY(start)

/* Note: Do not include STARTUP object file in INPUT as well */
STARTUP(kernel_entry.o)

INPUT 
(
    kernel.o
    screen.o
    ioports.o
    idt.o
    pic.o
    pit.o
    interrupts.o
    cpu.o
    keyboard.o
    timer.o
    paging.o
    gdt.o
)

OUTPUT("kernel.bin")

/* Offset address of kernel binary is 0x1000 as this is where the bootloader
 * loads the pages */
SECTIONS
{
    . = 0x30001000;
    
    .text : AT(ADDR(.text) - 0x30000000) { 
        *(.text)
        *(.rodata*)
    }

    .data : AT(ADDR(.data) - 0x30000000) { 
        *(.data) 
    }

    .bss : AT(ADDR(.bss) - 0x30000000) {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }
}

