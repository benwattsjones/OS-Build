CC = $(HOME)/Src/os-cross-compiler/bin/i686-elf-gcc
CFLAGS = -ffreestanding -Wall -std=c99
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -nostdlib -ffreestanding -lgcc

C_SOURCES = $(wildcard os/cpu/*.c os/drivers/*.c os/kernel/*.c)
ASM_SOURCES = $(wildcard os/cpu/*.asm os/drivers/*.asm os/kernel/*.asm)
HEADERS = $(wildcard os/cpu/*.h os/drivers/*.h os/kernel/*.h)

C_OBJ_FILES = ${C_SOURCES:.c=.o}
ASM_OBJ_FILES = ${ASM_SOURCES:.asm=.o}
OBJ_FILES := $(ASM_OBJ_FILES) $(C_OBJ_FILES)

all: bootloader.bin kernel.bin 
	cat bootloader.bin kernel.bin > OS.bin
	# 2880 pages long for El-Torito ISO 9660 floppy disk ISO
	dd if=/dev/zero of=OS.img bs=512 count=2880 
	dd if=OS.bin of=OS.img seek=0 conv=notrunc
	mv .iso iso
	mv OS.img iso/
	mkisofs -quiet -V 'MyOS' -input-charset iso8859-1 -o OS.iso -b OS.img -hide OS.img iso/
	mv .bin bin
	mv *.bin *.o bin
	mv .run run
	chmod +x run

bootloader.bin:
	$(AS) os/boot/bootloader.asm -f bin -o bootloader.bin

# notdir gives file names only so can be recogniesed by link.ld
kernel.bin: $(notdir ${OBJ_FILES}) 
	$(CC) -T link.ld $(LDFLAGS)
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

cpu.o:
	$(CC) $(CFLAGS) -c os/cpu/cpu.c -o $@

keyboard.o:
	$(CC) $(CFLAGS) -c os/drivers/keyboard.c -o $@

.PHONY: clean
clean:
	rm *.iso
	rm iso/*
	mv iso .iso
	rm bin/*
	mv bin .bin
	chmod -x run
	mv run .run

.PHONY: fail
fail:
	rm *.o *.bin
