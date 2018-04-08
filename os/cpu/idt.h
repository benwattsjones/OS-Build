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

#ifndef CPU_IDT_H
#define CPU_IDT_H

#include <stdint.h>

// Function pointer type for addressing of interrupt handlers
typedef void (*IRG_HANDLER)(void); 
void installISR(uint32_t ir_code, IRG_HANDLER irq);
void initializeIDT();

#endif
