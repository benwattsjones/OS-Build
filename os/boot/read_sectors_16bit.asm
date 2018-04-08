; OS-Build. Copyright (C) 2017, 2018. Ben Watts-Jones.
; 
; This program is distributed under the GNU General Public License Version 3 
; (GPLv3) as published by the Free Software Foundation. You should have 
; recieved a copy of the GPLv3 licence along with this program. If not, see
; <http://www.gnu.org/licenses/>.
;
; This program is distributed WITHOUT ANY WARRANTY; without even the implied
; warranty of MECHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
; GPLv3 license for more details.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[bits 16]

read_sectors_16bit:
    push bp
    mov bp, sp
    ; This function accepts 2 param:
    ;  [bp + 4] = Number of 512b sectors to load from disk 
    ;  [bp + 6] = Address to load sectors to 
    ; The function expects the boot drive number to be stored in global variable
    ; BOOT_DRIVE, and for sectors to be loaded being placed immediately after
    ; boot sector code in storage.
    ; Function declares one 16-bit local variable on stack for checking how
    ; many sectors were requested to be read 
    sub sp, 2
    ; We will later call BIOS interrupt 0x13 to read sectors from disc. It
    ; expects to find the drive number to be found in dl
    mov dl, [BOOT_DRIVE]
    ; Load sectors to address 
    mov bx, [bp + 6]
    ; Load [param1] sectors
    mov dh, [bp + 4]
    ; Save dx to stack so we can later check how many sectors were requested
    ; to be read.
    push dx
    ; BIOS read sector function
    mov ah, 0x02
    ; read dh sectors
    mov al, dh
    ; select cylinder 0 (the one where the boot sector code was retrieved from)
    mov ch, 0x00
    ; select head 0 (again, same as that of boot sector code)
    mov dh, 0x00
    ; Start reading from second sector (i.e. after boot sector)
    mov cl, 0x02
    int 0x13
    ; Jump if error (i.e. carry flag set)
    jc disk_error
    ; Restore dx from stack (dh will contain number sectors expected)
    pop dx
    ; BIOS routine sets al to number of sectors read. Want to jump if error.
    cmp dh, al
    jne disk_error
    ; return value of zero if no error
    xor ax, ax
    mov sp, bp
    pop bp
    ret 
disk_error:
    ; return value of one if error
    mov eax, 1
    mov sp, bp
    pop bp
    ret 

