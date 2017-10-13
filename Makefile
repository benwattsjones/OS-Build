CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pic 
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

all: bootloader.bin kernel.bin 
	cat bootloader.bin kernel.bin > OS.bin
	# 2880 pages long for El-Torito ISO 9660 floppy disk ISO
	dd if=/dev/zero of=OS.img bs=512 count=2880 
	dd if=OS.bin of=OS.img seek=0 conv=notrunc
	mv .iso iso
	mv OS.img iso/
	mkisofs -quiet -V 'MyOS' -input-charset iso8859-1 -o OS.iso -b OS.img -hide OS.img iso/
	mv .bin bin
	mv *.bin bin
	mv *.o bin
	mv .run run
	chmod +x run

bootloader.bin:
	$(AS) os/boot/bootloader.asm -f bin -o bootloader.bin

kernel.bin: kernel_entry.o kernel.o screen.o ioports.o idt.o pic.o pit.o interrupts.o
	ld -o kernel.bin $(LDFLAGS) kernel_entry.o kernel.o screen.o ioports.o idt.o pic.o pit.o interrupts.o
	chmod -x kernel.bin

kernel_entry.o:
	$(AS) os/kernel/kernel_entry.asm $(ASFLAGS) -o $@

kernel.o:
	$(CC) $(CFLAGS) -c os/kernel/kernel.c -o $@

screen.o:
	$(CC) $(CFLAGS) -c os/drivers/screen.c -o $@

ioports.o:
	$(AS) os/cpu/ioports.asm $(ASFLAGS) -o $@

idt.o:
	$(CC) $(CFLAGS) -c os/cpu/idt.c -o $@

pic.o:
	$(CC) $(CFLAGS) -c os/cpu/pic.c -o $@

pit.o:
	$(CC) $(CFLAGS) -c os/cpu/pit.c -o $@

interrupts.o:
	$(CC) $(CFLAGS) -c os/cpu/interrupts.c -o $@

.PHONY: clean
clean:
	rm *.iso
	rm iso/*
	mv iso .iso
	rm bin/*
	mv bin .bin
	chmod -x run
	mv run .run


