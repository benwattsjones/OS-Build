CC = gcc
CFLAGS = -m32 -ffreestanding 
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

all: bootloader.bin kernel.bin 
	cat bootloader.bin kernel.bin > OS.bin
	dd if=OS.bin of=OS.iso
	fallocate -l 5120 OS.iso # Make iso 10 pages long
	mv .bin bin
	mv *.bin bin
	mv *.o bin
	mv .run run
	chmod +x run

bootloader.bin:
	$(AS) boot/bootloader.asm -f bin -o bootloader.bin

kernel.bin: kernel_entry.o kernel.o screen.o ioports.o idt.o gdt.o
	ld -o kernel.bin $(LDFLAGS) kernel_entry.o kernel.o screen.o ioports.o idt.o gdt.o
	chmod -x kernel.bin

kernel_entry.o:
	$(AS) kernel/kernel_entry.asm $(ASFLAGS) -o kernel_entry.o

kernel.o:
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

screen.o:
	$(CC) $(CFLAGS) -c drivers/screen.c -o screen.o

ioports.o:
	$(AS) drivers/ioports.asm $(ASFLAGS) -o ioports.o

idt.o:
	$(CC) $(CFLAGS) -c drivers/idt.c -o idt.o

gdt.o:
	$(CC) $(CFLAGS) -c drivers/gdt.c -o gdt.o

.PHONY: clean
clean:
	rm *.iso
	rm bin/*
	mv bin .bin
	chmod -x run
	mv run .run


