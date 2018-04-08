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

#include "cpu.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "interrupts.h"

/* This file collects together the initialization funcions of this file
 * to create a single point of entry for the kernel to call.
 */

void initializeInterrupts()
{
    initializeIDT();
    initializePIC();
    timer_init();
    initializeHardwareInterrupts();
    initializeSoftwareInterrupts();
    __asm__ __volatile__ ("sti");
}
