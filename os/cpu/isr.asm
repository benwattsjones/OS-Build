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

; This file defines hardware and software interrupt serivce routines.
; These need to be done in assembly to best ensure the stack and registers
; are preserved and that iret instead of ret is used. Otherwise, unexpected
; errors will occur.
;
; The ISRs are added to the IDT using the 'installISR()' function defined in 
; idt.c. This is done in file 'interrupts.c' for custom ISRs. The default
; ISR is added beforehand in file 'idt.c'.
;
; Hardware ISRs must send an 'end of interrupt'. For more information see file
; 'pic.c' (and pit.c for the timer specifically).
;
; Some software interrputs called by the CPU, such as interrupt 0: divide by
; zero error, store the value of eip that caused the interrupt and will
; return to it upon reaching 'iret' instruction in the ISR. The would cause
; and endless loop of such interrupts. As such, these such interrupts are
; currently set to halt the CPU, waiting for new interrupts.
;
; Some software cpu interrupts also push a 32-bit error code onto the stack.
; In this case, the state of the stack upon entering the interrupt handler
; is:
;     EFLAGS
;     RETURN CS
;     RETURN EIP
;     ERROR CODE
; The error code must be removed from the top of the stack in this case, so
; iret returns to the correct EIP.

[bits 32]

ISR_DEFAULT_MSG:
    db "Error: unhandled interrupt.", 0x0a, 0

ISR_HARDWARE_DEFAULT_MSG:
    db "Error: unhandled hardware interrupt.", 0x0a, 0

ISR_EXCEPTION_0_MSG:
    db "Exception 0: divide by zero error.", 0x0a, 0
ISR_EXCEPTION_1_MSG:
    db "Exception 1: debug error.", 0x0a, 0
ISR_EXCEPTION_2_MSG:
    db "Exception 2: non-maskable interrupt.", 0x0a, 0
ISR_EXCEPTION_3_MSG:
    db "Exception 3: breakpoint.", 0x0a, 0
ISR_EXCEPTION_4_MSG:
    db "Exception 4: overflow.", 0x0a, 0
ISR_EXCEPTION_5_MSG:
    db "Exception 5: bound range exceeded.", 0x0a, 0
ISR_EXCEPTION_6_MSG:
    db "Exception 6: invalid opcode.", 0x0a, 0
ISR_EXCEPTION_7_MSG:
    db "Exception 7: device not available.", 0x0a, 0
ISR_EXCEPTION_8_MSG:
    db "Exception 8: double fault.", 0x0a, 0
ISR_EXCEPTION_9_MSG:
    db "Exception 9: coprocessor segment overrun.", 0x0a, 0
ISR_EXCEPTION_10_MSG:
    db "Exception 10: invalid TSS.", 0x0a, 0
ISR_EXCEPTION_11_MSG:
    db "Exception 11: segment not present.", 0x0a, 0
ISR_EXCEPTION_12_MSG:
    db "Exception 12: stack segment fault.", 0x0a, 0
ISR_EXCEPTION_13_MSG:
    db "Exception 13: general protection fault.", 0x0a, 0
ISR_EXCEPTION_14_MSG:
    db "Exception 14: page fault.", 0x0a, 0
ISR_EXCEPTION_16_MSG:
    db "Exception 16: x87 floating point exception.", 0x0a, 0
ISR_EXCEPTION_17_MSG:
    db "Exception 17: alignment check.", 0x0a, 0
ISR_EXCEPTION_18_MSG:
    db "Exception 18: machine check.", 0x0a, 0
ISR_EXCEPTION_19_MSG:
    db "Exception 19: SIMD floating point exception.", 0x0a, 0
ISR_EXCEPTION_20_MSG:
    db "Exception 20: virtualization exception.", 0x0a, 0
ISR_EXCEPTION_30_MSG:
    db "Exception 30: Security exception.", 0x0a, 0
ISR_EXCEPTION_RESERVED_MSG:
    db "Exception 15: [reserved].", 0x0a, 0

extern print
extern printKeyInput
extern printTimeElapsed

; Default interrupt handler

global handleInterruptDefault
handleInterruptDefault:
    pushad
    cli
    push ISR_DEFAULT_MSG
    call print
    add esp, 4
    sti
    popad

; Hardware interrupt handlers (IRQ 0-15; interrupts 32-48)

global handleInterruptKeyboard
handleInterruptKeyboard:
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

global handleInterruptTimer
handleInterruptTimer:
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
    push ISR_HARDWARE_DEFAULT_MSG
    call print
    add esp, 4
    mov al, 0x20
    out 0x20, al
    sti 
    popad
    iret

global handleHardwareInterrupts_high
handleHardwareInterrupts_high:
    pushad
    cli
    push ISR_HARDWARE_DEFAULT_MSG
    call print
    add esp, 4
    mov al, 0x20
    out 0xa0, al
    sti
    popad
    iret

; Software interrupt (exception) service routines from CPU (0-31)

%macro exception_plain 1
    cli
    pushad
    push %1
    call print
    add esp, 4
    popad
    sti
    iret
%endmacro

%macro exception_with_hlt 1
    cli
    pushad
    push %1
    call print
    add esp, 4
    sti
.hlt_loop:
    hlt
    jmp .hlt_loop
    popad
    iret
%endmacro

%macro exception_with_error_code_hlt 1
    cli
    add esp, 4 ; get rid of error code (will implement printing at later date)
    pushad
    push %1
    call print
    add esp, 4
    sti
.hlt_loop2:
    hlt
    jmp .hlt_loop2
    popad
    iret
%endmacro

global handleException0
handleException0:
    exception_with_hlt ISR_EXCEPTION_0_MSG

global handleException1
handleException1:
    exception_plain ISR_EXCEPTION_1_MSG

global handleException2
handleException2:
    exception_with_hlt ISR_EXCEPTION_2_MSG

global handleException3
handleException3:
    exception_plain ISR_EXCEPTION_3_MSG

global handleException4
handleException4:
    exception_with_hlt ISR_EXCEPTION_4_MSG

global handleException5
handleException5:
    exception_with_hlt ISR_EXCEPTION_5_MSG

global handleException6
handleException6:
    exception_with_hlt ISR_EXCEPTION_6_MSG

global handleException7
handleException7:
    exception_with_hlt ISR_EXCEPTION_7_MSG

global handleException8
handleException8:
    exception_with_error_code_hlt ISR_EXCEPTION_8_MSG

global handleException9
handleException9:
    exception_with_hlt ISR_EXCEPTION_9_MSG

global handleException10
handleException10:
    exception_with_error_code_hlt  ISR_EXCEPTION_10_MSG

global handleException11
handleException11:
    exception_with_error_code_hlt  ISR_EXCEPTION_11_MSG
    
global handleException12
handleException12:
    exception_with_error_code_hlt  ISR_EXCEPTION_12_MSG

global handleException13
handleException13:
    exception_with_error_code_hlt  ISR_EXCEPTION_13_MSG 

global handleException14
handleException14:
    exception_with_error_code_hlt  ISR_EXCEPTION_14_MSG  
 
global handleException15
handleException15:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException16
handleException16:
    exception_with_hlt  ISR_EXCEPTION_16_MSG  

global handleException17
handleException17:
    exception_with_hlt ISR_EXCEPTION_17_MSG

global handleException18
handleException18:
    exception_with_hlt ISR_EXCEPTION_18_MSG

global handleException19
handleException19:
    exception_with_hlt ISR_EXCEPTION_19_MSG

global handleException20
handleException20:
    exception_with_hlt ISR_EXCEPTION_20_MSG

global handleException21
handleException21:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException22
handleException22:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException23
handleException23:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException24
handleException24:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException25
handleException25:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException26
handleException26:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException27
handleException27:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException28
handleException28:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException29
handleException29:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG

global handleException30
handleException30:
    exception_with_hlt ISR_EXCEPTION_30_MSG

global handleException31
handleException31:
    exception_with_hlt ISR_EXCEPTION_RESERVED_MSG


