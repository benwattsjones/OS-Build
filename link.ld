
OUTPUT_FORMAT("binary")

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

SECTIONS
{
    . = 0x1000;
    .text : { *(.text) }
    .data : { *(.data) }
}

