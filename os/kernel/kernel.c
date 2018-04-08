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

/* This is the main kernel file. It is entered at main() and called by 
 * kernel_entry.asm.
 */

#include "../drivers/screen.h"
#include "../cpu/gdt.h"
#include "../cpu/cpu.h"

int main() 
{
    clearScreen();
    initializeGDT();
    initializeInterrupts();

    char *pstr = "Welcome to the OS!\n\0";
    printHeader(pstr, 0, RED_ORANGE_FOREGROUND);

    for (;;)
        __asm__ ("hlt");
    return 0;
}
