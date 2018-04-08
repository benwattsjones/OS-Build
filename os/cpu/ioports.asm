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

[bits 32]

; These functions are used with devices with 'memory mapped IO', for example
; the screen in text mode. This is where values of external devices registers
; are mapped by hardware to specific addresses on the IO address bus.

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
    mov esp, ebp
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
    mov esp, ebp
    pop ebp
    ret

