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

#include <stdint.h>

#include "interrupts.h"
#include "../drivers/screen.h"
#include "idt.h"
#include "isr.h"

/* This file acts an intermediary between the interrupt handlers defined in
 * isr.asm and the interrupt descriptor table in idt.c. Exceptions from the
 * CPU are mapped to interrupts 0-31, whilst hardware interrupts are mapped
 * by the PIC (see pic.c) to interrupts 32-48.
 * Note that IRQ being the timer explains why interrupts had to be disabled 
 * in protected mode, as in protected mode int 0 is a divide by zero error. 
 * This would cause a double fault and the OS would restart. Once the 
 * interrupts have been handled (and after each is called), we must enable 
 * interrupts with 'sti'.
*/

void initializeHardwareInterrupts()
{
    installISR(0x20, handleInterruptTimer); // 0x20 == 32
    installISR(0x21, handleInterruptKeyboard);
    installISR(0x22, handleHardwareInterrupts_low);
    installISR(0x23, handleHardwareInterrupts_low);
    installISR(0x24, handleHardwareInterrupts_low);
    installISR(0x25, handleHardwareInterrupts_low);
    installISR(0x26, handleHardwareInterrupts_low);
    installISR(0x27, handleHardwareInterrupts_low);
    installISR(0x28, handleHardwareInterrupts_high);
    installISR(0x29, handleHardwareInterrupts_high);
    installISR(0x2a, handleHardwareInterrupts_high);
    installISR(0x2b, handleHardwareInterrupts_high);
    installISR(0x2c, handleHardwareInterrupts_high);
    installISR(0x2d, handleHardwareInterrupts_high);
    installISR(0x2e, handleInterruptPrimaryATA);
    installISR(0x2f, handleInterruptSecondaryATA);
}

void initializeSoftwareInterrupts()
{
    installISR(0,  handleException0);
    installISR(1,  handleException1);
    installISR(2,  handleException2);
    installISR(3,  handleException3);
    installISR(4,  handleException4);
    installISR(5,  handleException5);
    installISR(6,  handleException6);
    installISR(7,  handleException7);
    installISR(8,  handleException8);
    installISR(9,  handleException9);
    installISR(10, handleException10);
    installISR(11, handleException11);
    installISR(12, handleException12);
    installISR(13, handleException13);
    installISR(14, handleException14);
    installISR(15, handleException15);
    installISR(16, handleException16);
    installISR(17, handleException17);
    installISR(18, handleException18);
    installISR(19, handleException19);
    installISR(20, handleException20);
    installISR(21, handleException21);
    installISR(22, handleException22);
    installISR(23, handleException23);
    installISR(24, handleException24);
    installISR(25, handleException25);
    installISR(26, handleException26);
    installISR(27, handleException27);
    installISR(28, handleException28);
    installISR(29, handleException29);
    installISR(30, handleException30);
    installISR(31, handleException31);
}

