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

#include "ioports.h"

/* The PIC fires hardware IRQs. There are typically two PICs, a master
 * and a slave, each with 8 pins for different hardware interrupts.
 * The PICs must be initialised so they know which IRs to call. This
 * involves sending a series of initialization control words (ICWs) to
 * the ports defined above.
 * ICW1 is 00010001b (0x11)
 *                +- PIC expects to recieve all 4 ICWs
 *               +-- Two PICs total
 *              +--- Not used in x86
 *             +---- Operate in edge triggered, not level triggered
 *                   mode. An IR corresponds to a single electrical 
 *                   pulse
 *            +----- PIC is being initialized
 *         +++------ Reserved
 * ICW2 gives the base address of the idt for interrupts to use. We
 * are using 32 (0x20) as the first, as this is the first non-preset/
 * reserved interrupt number. As each PIC has 8 pins, the slave PIC
 * starts at interrupt number 0x28.
 * ICW3 tells which IRQ line (pin) on the master PIC connects to the 
 * slave PIC. In x86, this must be line 2, which corresponds to 0x04
 * (bit 2) for the master PIC and 0x02 (binary 2) for the slave PIC.
 * ICW4 tells the PICs that we are operating in x86 mode as bit 0 is 1.
 * Finally, we write to the interrupt mask register (same port as data
 * register), to tell it which IRQs are allowed to fire. Each bit corresponds
 * to an IRQ, with 0 allowing it to fire. Thus we send 0x00 to allow all
 * IRQs to fire.
 *
 * An end of interrupt signal (EOI) must also be sent after each interrupt
 * to allow subsiquent interrupts. This is now implemented in isr.asm.
 */

#define PIC_1_CTRL 0x20  // primary PIC control register
#define PIC_2_CTRL 0xa0  // secondary PIC control register
#define PIC_1_DATA 0x21  // primary PIC data register
#define PIC_2_DATA 0xa1  // secondary PIC data registe

#define ICW_1 0x11  // 00010001 binary. Enables initialization mode and ICW 4

#define IRQ_0 0x20  // IRQs 0-7 mapped to interrupts 0x20-0x27
#define IRQ_8 0x28  // IRQs 8-15 mapped to use interrupts 0x28-0x36

#define ICW_3_PRI 0x04  // bit 2 so use IR line 2 to comunicate with secondary PIC
#define ICW_3_SEC 0x02  // use line 2 to communicate with primary PIC

#define ICW_4 0x01  // bit 0 enables 80x86 mode

void initializePIC()
{
    portByteOut(PIC_1_CTRL, ICW_1);
    portByteOut(PIC_2_CTRL, ICW_1);
    portByteOut(PIC_1_DATA, IRQ_0);
    portByteOut(PIC_2_DATA, IRQ_8);
    portByteOut(PIC_1_DATA, ICW_3_PRI);
    portByteOut(PIC_2_DATA, ICW_3_SEC);
    portByteOut(PIC_1_DATA, ICW_4);
    portByteOut(PIC_2_DATA, ICW_4);
    portByteOut(PIC_1_DATA, 0x00);
    portByteOut(PIC_2_DATA, 0x00);
}

