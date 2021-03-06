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

; This file defines the Global Descriptor table, used for switching from
; 16-bit real mode to 32-bit protected mode. This defines cs and ds addresses,
; and their privalages - i.e. cs can only execute data, ds only read data.
; There are better mechanisms of virtual memory (e.g. paging) that we will
; implement later so this is not too important, however it is a requirement
; of the hardware.

gdt_start:

; Mandatory 9-byte null descriptor
gdt_null:
    times 8 db 0

; The code segment descriptor
gdt_code:
    ; Segment limit - can access entire 2-byte space in each segment
    dw 0xffff
    ; Base Address - addresses start at 0 so can access entire 2-byte space
    dw 0
    db 0
    ; Access byte - set flags
    db 10011010b
    ;         + Access bit = 0 as not using virtual memory
    ;        +- Readable/Writable bit = 1 so can read and execute data in 
    ;           segment
    ;       +-- Expansion direction bit = 0 (ignore)
    ;      +--- 1 as code not data descriptor
    ;     +---- 1 as code/data descriptor not system descriptor
    ;   ++----- Descriptor privilege level = Ring 0 (Highest)
    ;  +------- 0 as not using virtual memory (indicates segment in memory)
    ; Granularity byte - more on size
    db 11001111b
    ;      ++++ extends segment limit from 0xffff to 0xfffff
    ;    ++---- 0 as reserved
    ;   +------ 1 as 32-bit not 16-bit segment type
    ;  +------- Granularity = 1 (each segment will be bounded by 4kb)
    ; Base address
    db 0
; Data segment descriptor. This is the same as the code segment desctiptor
; except one bit in access byte to identify as data and not code descriptor
gdt_data:
    dw 0xffff
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
; Label usefull for callting sizes
gdt_end:

; GDT descriptor
gdt_descriptor:
    ; size of GDT
    dw gdt_end - gdt_start -1
    ; Start address of GDT
    dd gdt_start

; Define constants for the GDT segment descriptor offests, which are what
; segment registers must contain when in protected mode. I.e.:
; 0x0 -> NULL; 0x8 -> CODE; 0x10 (16) -> DATA
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

