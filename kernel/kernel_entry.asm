; This function calls the kernel. It is compiled such that it definitely
; jumps into the kernels entry function (even if not at start of binary)
[bits 32]

; Linker will substitute for final address
[extern main]

global _start

_start:
    call main
    jmp $
