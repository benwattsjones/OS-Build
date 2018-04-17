# OS-Build. Copyright (C) 2017, 2018. Ben Watts-Jones.
# 
# This program is distributed under the GNU General Public License Version 3 
# (GPLv3) as published by the Free Software Foundation. You should have 
# recieved a copy of the GPLv3 licence along with this program. If not, see
# <http://www.gnu.org/licenses/>.
#
# This program is distributed WITHOUT ANY WARRANTY; without even the implied
# warranty of MECHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GPLv3 license for more details.


### Define parameters for compilation etc.

CC = cross-compiler/bin/i686-elf-gcc
CFLAGS = -ffreestanding -Wall -Wextra -Wpedantic -std=c99 -O3
AS = nasm
ASFLAGS = -f elf
LDFLAGS = -nostdlib -ffreestanding -lgcc -O3 -Wall -Wextra

### Locate object/program files for creation of kernel.bin (NOT bootloader.bin)

C_SOURCES = $(shell find os/ -type f -name '*.c' -not -path 'os/boot/*' 2>/dev/null)
ASM_SOURCES = $(shell find os/ -type f -name '*.asm' -not -path 'os/boot/*' 2>/dev/null)

C_OBJ_FILES = ${C_SOURCES:.c=.o}
ASM_OBJ_FILES = ${ASM_SOURCES:.asm=.o}
OBJ_FILES := $(ASM_OBJ_FILES) $(C_OBJ_FILES)

OBJ_FILES_NAMES = $(basename $(notdir $(OBJ_FILES)))
ifneq ($(words $(sort $(OBJ_FILES_NAMES))), $(words $(OBJ_FILES_NAMES)))
$(info "Your file basenames: $(OBJ_FILES_NAMES)")
$(error "All file basenames must be unique compile.")
endif

### Check required dependecies exist / are installed

NASM := $(shell command -v nasm 2> /dev/null)
MKISOFS := $(shell command -v mkisofs 2> /dev/null)

ifndef NASM
$(error "No nasm in PATH, consider installing nasm. Required for assembly.")
endif

ifeq ("$(wildcard $(CC))", "")
$(error "Cross-compiler not present in expected location.")
endif

ifndef MKISOFS
$(error "No mkisofs in path, consider installing mkisofs. Required for cdrom ISO boot.")
endif

### Compilation instructions

all: bootloader.bin kernel.bin 
	cat .bin/bootloader.bin .bin/kernel.bin > .bin/OS.bin
	# 2880 pages long for El-Torito ISO 9660 floppy disk ISO
	dd if=/dev/zero of=.bin/OS.img bs=512 count=2880 
	dd if=.bin/OS.bin of=.bin/OS.img seek=0 conv=notrunc
	mkisofs -quiet -V 'MyOS' -input-charset iso8859-1 -o OS.iso -b OS.img -hide OS.img .bin/
	test -f run || mv .run run
	chmod +x run

bootloader.bin:
	$(AS) os/boot/bootloader.asm -f bin -o .bin/bootloader.bin

kernel.bin: $(OBJ_FILES) 
	$(eval OBJ_INPUT_FILES = $(shell find .bin/ -type f -name '*.o' -not -path '.bin/kernel_entry.o'))
	$(CC) $(OBJ_INPUT_FILES) -T link.ld $(LDFLAGS)
	chmod -x .bin/kernel.bin

$(C_OBJ_FILES): 
	$(CC) $(CFLAGS) -c $(basename $@).c -o .bin/$(notdir $@)

$(ASM_OBJ_FILES): 
	$(AS) $(basename $@).asm $(ASFLAGS) -o .bin/$(notdir $@)

### Clean up instructions

BIN_FILES = $(wildcard .bin/*.o .bin/*.img .bin/*.bin)

.PHONY: clean
clean:
ifneq ("$(wildcard $(BIN_FILES))", "")
	rm $(BIN_FILES)
endif
	test ! -f OS.iso || rm OS.iso
	test ! -f run || chmod -x run
	test ! -f run || mv run .run

