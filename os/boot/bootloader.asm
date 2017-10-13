; This is the boot sector code that is initially run by the bios.
; It is recognised by the bios as it is 512b large (page size) and ends
; in the bytes 0xaa55 (in little edian format). The purpose of this file is to
; convert to 32-bit protected mode and call the kernel.

; The computer starts in 16 bit real mode, which enables legacy OSs to work.
; Thus all memory addresses must be 16 bits.

; The bootloader is placed in memory at address 0x7c00 by the bios. Here, we
; tell the assembler that addresses are to be measured relative to this
; address - i.e. add 0x7c00 to the addresses accessed. We do not have to do
; this in normal asm coding as the OS (which we don't have) places programs
; at the same point in virtual memory (known by the linker), which also
; recognises the program start directive.
[org 0x7c00]

[bits 16]

task1_setup_stack_and_save_bios_info:
    ; Prevent interrupts during setup
    cli
    ; Set certain segments to zero (prob. are anyway)
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    ; BIOS puts boot drive number (e.g. 0x80 if hard drive) in dl before
    ; giving control to boot sector code. We will save this as a global 
    ; variable for later use.
    mov [BOOT_DRIVE], dl
    ; Set stack to start at 0x8000. The 512-byte boot sector is at 0x7c00
    ; to 0x7e00. Thus 0x8000 is safely out of the way. Addresses 0x9fc00 to
    ; 0x100000 are also reverved by the BIOS. Note that addresses above 0x100000
    ; are ours to use as we please. Remember the stack grows downwards, here
    ; from 0x8000 to 0x7e00.
    mov bp, 0x8000
    mov sp, bp
    ; Enable interrupts again
    sti
    
task2_print_loaded_16bit_message:
    ; We will later use `int 0x10` to interrupt the program with a call to the
    ; BIOS. If ah register holds `0x0e`, the scrolling teletype BIOS routine is
    ; called. This causes the ascii char in al register to be printed to the 
    ; screen, and then the 'cursor' to be moved along. We only set ah once as 
    ; we will not change its value through the printing process.
    ; Note we are in 16bit mode, so must store addresses in 16bit register.
    ; Note also ebx is used for indexed addressing
    ; Code changed to use function: put addess of null-terminated string to
    ; print onto stack
    push WELCOME_16BIT_MSG
    call print_16bit_scrolling_teletype
    add sp, 2

; The BIOS will initially only load the first 512-byte sector from the disk.
; However, the OS will be larger than 512-bytes so here we load subsequent
; sectors into RAM as well, to a location we specify.
task3_read_sectors_from_disk:
    ; 2nd arg is address to load sectors into. Note: Ttext in ld kernel
    push word 0x1000
    ; 1st arg is number of sectors to load (located immediately after boot
    ; sector code). NOTE: qemu fails if more requested than present in .iso
    ; Note: set by fallocate in Makefile
    push word 9
    call read_sectors_16bit
    add sp, 4
    cmp ax, 0
    je task5_load_gdt
    ; If BIOS routine failed, prints error message and hangs.
    push DISK_ERROR_MESSAGE
    call print_16bit_scrolling_teletype
    add sp, 2
    cli
    jmp $

task5_load_gdt:
    cli
    ; Load Glabal Descriptor Table, which defines the protected mode segments
    ; for code and data. GDT is 8 bytes long, and placed into the GDTR register
    ; (also 8 bytes). lgdt expects to be passed a pointer to the gdt in the 
    ; format: dw size_of_gdt-1; dd address_of_gdt.
    lgdt [gdt_descriptor]
    ; Set first bit of CR0 control register to switch to protected mode
    ; Control registers mainly reserved for cpu use and cannot be modified
    ; directly. If bit 0 of cr0 is 1 than the cpu switches to 32-bit protected
    ; mode (Note we also need to tell nasm this change for assembly).
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    ; Far jump to new code segment to our 32-bit code. 
    ; Also forces CPU to flush cache of instructions, so definitely in 
    ; protected mode in new segment.
    jmp CODE_SEG:task6_setup_protected_mode

; Note code here jumped over - paths relative to Makefile
%include "os/boot/print_16bit.asm"
%include "os/boot/read_sectors_16bit.asm"
%include "os/boot/gdt.asm"

[bits 32]
; Initialise registers and stack once in protected mode
; Segment registers must be set to GDT segment offset. Old registers invalid.
task6_setup_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; Set top of stack to near top of free space (i.e. region 0x7e00 - 0x9fc00)
    mov ebp, 0x90000
    mov esp, ebp

task7_call_kernel:
    ; jump to address where kernel_entry loaded into memory in "task3_"
    call 0x1000

; This causes the program to 'hang' (i.e. jump to the current address forever).
; No further code will be executed in this file. This allows us to define 
; data below, and stops the OS terminating as soon as it loads.
jmp $

; Here we define data. This will not be executed, as we hang permanently
; above. However it is still accessible in memory. Note strings are null
; terminated. Note 0xd is ascii code for carriage return.
WELCOME_16BIT_MSG:
    db "Started in 16bit real mode", 0x0d, 0x0a, 0
BOOT_DRIVE:
    db 0
DISK_ERROR_MESSAGE:
    db "Error: failed to read from storage - exiting process.", 0x0d, 0x0a, 0

; Padding and magic number so code is recognised as a bootloader. Again, this
; code will not be executed. Note we add zeros to 510 bytes, then 2 bytes for
; magic number.
times 510-($-$$) db 0
dw 0xaa55

