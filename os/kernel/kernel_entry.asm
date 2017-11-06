; This function calls the kernel. It is compiled such that it is at the start
; of the binary, enabling the kernels 'main' function to be called even if
; it is not the first function in the file.
[bits 32]

%include "os/kernel/paging.asm"

; Linker will substitute for final address
[extern main]
;extern createPageDirectoryTable
;extern createPageTables
;extern activatePagingTables
;extern activatePaging
;extern unmapIdentityMapping

STACKSIZE equ 0x4000

global start
start equ (_start - 0x30000000)
global _start

_start:
    createPageDirectoryTable 
    createPageTables 
    activatePagingTables 
    activatePaging 
    lea ecx, [switchToHigherHalf]
    jmp ecx

switchToHigherHalf:
    unmapIdentityMapping 

    mov ebp, stack + STACKSIZE
    mov esp, ebp

    call main
    jmp $

section .bss
align 32
stack:
    resb STACKSIZE

