CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pic 
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

all: bootloader.bin kernel.bin 
	cat bootloader.bin kernel.bin > OS.bin
	dd if=/dev/zero of=OS.img bs=512 count=2880 # 2880 pages long for El-Torito ISO 9660 floppy disk ISO
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
	$(AS) boot/bootloader.asm -f bin -o bootloader.bin

kernel.bin: kernel_entry.o kernel.o screen.o ioports.o idt.o pic.o pit.o
	ld -o kernel.bin $(LDFLAGS) kernel_entry.o kernel.o screen.o ioports.o idt.o pic.o pit.o
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

pic.o:
	$(CC) $(CFLAGS) -c drivers/pic.c -o pic.o

pit.o:
	$(AS) drivers/pit.asm $(ASFLAGS) -o pit.o

.PHONY: clean
clean:
	rm *.iso
	rm iso/*
	mv iso .iso
	rm bin/*
	mv bin .bin
	chmod -x run
	mv run .run


