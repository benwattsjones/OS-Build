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

; This function calls the kernel. It is compiled such that it is at the start
; of the binary, enabling the kernels 'main' function to be called even if
; it is not the first function in the file.
; The code also initializes paging, using the macros in 'paging.asm', and
; sets the new (virtual space) stack. This is done here (and not in
; kernel.c) as it requires assembly code.
[bits 32]

%include "os/kernel/paging.asm"

; Linker will substitute for final address
[extern main]

global start
start equ (_start - 0x30000000)

_start:
    createPageDirectoryTable 
    createPageTables 
    activatePagingTables 
    activatePaging 
    lea ecx, [switchToHigherHalf]
    jmp ecx

switchToHigherHalf:
    unmapIdentityMapping 
    mov ebp, 0x30300000 ; set start of new stack
    mov esp, ebp

    call main
    jmp $


