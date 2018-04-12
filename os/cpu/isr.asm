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

extern printKeyInput
global handleHardwareInterrupts_keyboard

handleHardwareInterrupts_keyboard:
    pushad
    cli
    xor eax, eax
    in al, 0x60
    push eax
    cld
    call printKeyInput
    pop eax
    xor eax, eax
    mov al, 0x20
    out 0x20, al
    sti
    popad
    iret

extern printTimeElapsed
global handleHardwareInterrupts_timer

handleHardwareInterrupts_timer:
    pushad
    cli
    call printTimeElapsed
    xor eax, eax
    mov al, 0x20
    out 0x20, al
    sti
    popad
    iret

global handleHardwareInterrupts_low

handleHardwareInterrupts_low:
    pushad
    cli 
    mov al, 0x20
    out 0x20, al
    sti 
    popad
    iret

global handleHardwareInterrupts_high

handleHardwareInterrupts_high:
    pushad
    cli
    mov al, 0x20
    out 0xa0, al
    sti
    popad
    iret

