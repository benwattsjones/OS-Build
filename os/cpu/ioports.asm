[bits 32]

global portByteIn

portByteIn:
    push ebp
    mov ebp, esp
    ; function takes one arguement:
    ;  [epb + 8] = port (2-bytes)
    ; note 1-byte return result will be put in al
    mov dx, [ebp + 8]
    xor eax, eax
    in al, dx
    pop ebp
    ret

global portByteOut

portByteOut:
    push ebp
    mov ebp, esp
    ; function takes two arguements:
    ;  [ebp + 8] = port (2-bytes)
    ;  [ebp + 12] = data (1-byte)
    ; Note: even thouh param 1 only 2-bytes big, param 2 still 4-bytes along.
    ; no return value
    mov dx, [ebp + 8]
    mov al, [ebp + 12]
    out dx, al
    pop ebp
    ret

