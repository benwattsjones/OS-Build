/* OS-Build. Copyright (C) 2017, 2018. Ben Watts-Jones.
 *
 * This program is distributed under the GNU General Public License Version 3 
 * (GPLv3) as published by the Free Software Foundation. You should have 
 * recieved a copy of the GPLv3 licence along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MECHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GPLv3 license for more details.
 */

OUTPUT_FORMAT("binary")

ENTRY(start)

/* Note: Do not include STARTUP object file in INPUT as well. 
   INPUT files now defined in Makefile */
STARTUP(.bin/kernel_entry.o)

OUTPUT(".bin/kernel.bin")

/* Offset address of kernel binary is 0x9000 as this is where the bootloader
 * loads the pages */
SECTIONS
{
    . = 0x30009000;
    
    .text : AT(ADDR(.text) - 0x30000000) { 
        *(.text)
        *(.rodata*)
    }

    .data : AT(ADDR(.data) - 0x30000000) { 
        *(.data) 
    }

    .bss : AT(ADDR(.bss) - 0x30000000) {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }
}

