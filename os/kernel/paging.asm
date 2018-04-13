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

; Note each table is 1024*4-bytes large (0x1000 bytes). There are 256 page
; tables implemented total, 0x100000 bytes. The tables are stored from physical
; memory address 0x19000. Thus the page directory table is at addresses
; 0x10000 - 0x20000, and the page tables are at addresses 0x20000 - 0x120000
; (0x120000 is ~1.07MiB).
; This gives the Kernel addresses 0x1000 - 0x19000 (0x10000 bytes total). Note
; though that bios int 0x13 cannot address RAM past address 0x10000.
; Each page directory entry covers 4MB (0x3e8000-bytes) memory. Thus a single
; page directory entry is sufficient to cover The entire kernel code and paging
; info.
; Note choice of implementing 256 page tables / 1GB memory is because no 
; physical memory checks but 1GB RAM likely exists without gaps.
; Note kernel uses page directory table index 192 (0.75GiB) for virtual address
; space. This is address 0x30000000.

ENTRIES_PER_TABLE equ 1024
NUMBER_PAGE_TABLES equ 250
BYTES_ADDRESSED_PER_PAGE_TABLE equ 4096
BYTES_PER_TABLE equ 4096
STARTING_PYSICAL_ADDRESS_TO_MAP equ 0
TABLES_START_ADDRESS equ 0x19000
NUMBER_PAGE_TABLES_IMPLEMENTED equ 256
KERNEL_PAGE_DIRECTORY_INDEX equ 192 ; kernel virtual address space 0.75-1.00 GiB
KERNEL_PAGE_DIRECTORY_ADDRESS equ 0x30000000

; Creates a page directory table from address TABLES_START_ADDRESS with entries
; pointing to tables packed from physical address TABLES_START_ADDRESS +
; BYTES_PER_TABLE.
; Note: need to ensure code is position independent and uses physical addresses.
%macro createPageDirectoryTable 0 
    mov eax, TABLES_START_ADDRESS + BYTES_PER_TABLE ; address of page table
    xor ecx, ecx ; counter for page directory entry iteration
    and eax, 0xfffff000 ; only want bits 12-31 (not strictly neccessary)
    or eax, 2 ; flags: supervisor, write enabled, page table NOT present
.fillPageDirectoryTable_startLoop:
    mov [TABLES_START_ADDRESS + ecx*4], eax
    inc ecx
    cmp ecx, ENTRIES_PER_TABLE - 1
    je .fillPageDirectoryTable_endLoop
    jmp .fillPageDirectoryTable_startLoop
.fillPageDirectoryTable_endLoop:
%endmacro

%macro createPageTables 0
    mov eax, STARTING_PYSICAL_ADDRESS_TO_MAP ; Page table entry frame address
    xor ecx, ecx ; Counter for page table entry iteration
.fillPageTable_startLoop:
    mov ebx, eax
    and ebx, 0xfffff000
    or ebx, 3 ; Flags: supervisor, write enabled, page in memory
    mov [(TABLES_START_ADDRESS + BYTES_PER_TABLE) + ecx*4], ebx
    add eax, BYTES_PER_TABLE 
    inc ecx
    cmp ecx, ENTRIES_PER_TABLE - 1
    je .fillPageTable_endLoop
    jmp .fillPageTable_startLoop
.fillPageTable_endLoop:
%endmacro

;  Need to set bit 0 (page table present) for page directory entry 0 and 192
; Have to map entry 0 (identity mapping), otherwise OS will crash as soon as
; paging is enabled as it will not be able to fetch the next instruction.
; The identity mapping will be unmapped after the switch to virtual addresses.
%macro activatePagingTables 0
    mov eax, [TABLES_START_ADDRESS]
    or eax, 1
    mov [TABLES_START_ADDRESS], eax
    mov eax, [TABLES_START_ADDRESS + KERNEL_PAGE_DIRECTORY_INDEX*4]
    or eax, 1
    mov [TABLES_START_ADDRESS + KERNEL_PAGE_DIRECTORY_INDEX*4], eax
%endmacro

; To enable paging, must store the start address of the page directory table
; in cr3 register, and set bit 31 of the cr0 register (this tells the cpu
; paging is being used).
%macro activatePaging 0
    mov eax, TABLES_START_ADDRESS 
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
%endmacro

; Changes bitflags to page not present in first page table pointer in
; page directory table, and invalidates cache to ensure changes recieved.
%macro unmapIdentityMapping 0
    mov dword [TABLES_START_ADDRESS], 0x00000002
    invlpg [0]
%endmacro


