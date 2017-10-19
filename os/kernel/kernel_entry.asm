; This function calls the kernel. It is compiled such that it is at the start
; of the binary, enabling the kernels 'main' function to be called even if
; it is not the first function in the file.
[bits 32]

; Linker will substitute for final address
[extern main]

global _start

_start:
    call main
    jmp $
