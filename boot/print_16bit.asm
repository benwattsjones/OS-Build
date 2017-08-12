[bits 16]

print_16bit_scrolling_teletype:
    push bp
    mov bp, sp
    ; This function accepts 1 param -
    ;   [bp+4] = address of start of null-terminated string to print
    ; Note [bp+2] holds return address
    ; Scrolling teletype bios routine
    mov ah, 0x0e
    ; Address of start of null-terminated string
    mov bx, [bp + 4]
print_16bit_scrolling_teletype_loop_start:
    mov al, [bx]
    cmp al, 0
    je print_16bit_scrolling_teletype_finished
    ; BIOS interrupt for visual routines
    int 0x10
    ; move to next character/byte
    inc bx
    jmp print_16bit_scrolling_teletype_loop_start
print_16bit_scrolling_teletype_finished:
    ; Return value goes in eax, but no return needed here
    mov sp, bp
    pop bp
    ret

