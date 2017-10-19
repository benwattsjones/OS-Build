
OUTPUT_FORMAT("binary")

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
)

OUTPUT("kernel.bin")

/* Offset address of kernel binary is 0x1000 as this is where the bootloader
 * loads the pages */
SECTIONS
{
    . = 0x1000;
    .text : { *(.text) }
    .data : { *(.data) }
}

