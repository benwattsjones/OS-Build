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

#include "screen.h"

/* The 'printKeyInput' function in this file is called by interrupts.c upon 
 * an IRQ1 interrupt request. The interrpt handler recieves a scan code from
 * the keyboard encoder microcontroller. This file decodes the scan code to
 * the corresponding ASCII code and prints the result. Whilst there are
 * different scan code sets, this driver assumes the keyboard uses the common
 * XT scan code set.
 */

static char *scan_codes2[0x40] = {"?\0", "?\0", "1\0", "2\0", "3\0", "4\0", "5\0", "6\0",
                                  "7\0", "8\0", "9\0", "0\0", "-\0", "=\0", "?\0", "?\0",
                                  "q\0", "w\0", "e\0", "r\0", "t\0", "y\0", "u\0", "i\0",
                                  "o\0", "p\0", "[\0", "]\0", "\n\0", "?\0", "a\0", "s\0", 
                                  "d\0", "f\0", "g\0", "h\0", "j\0", "k\0", "l\0", ";\0",
                                  "'\0", "#\0", "?\0", "\\\0", "z\0", "x\0", "c\0", "v\0",
                                  "b\0", "n\0", "m\0", ",\0", ".\0", "/\0", "?\0", "*\0",
                                  "?\0", " \0"};


// covers 0x0 - 0x39 of original XT scan codes.
// Called by keyboard IRQ1 handler in interrupts.c in os/cpu/
void printKeyInput(uint8_t scan_code)
{
    if (scan_code <= 0x39) {
        print(scan_codes2[scan_code]);
    } else if (scan_code < 0x80) { // Key up codes at 0x80+
        print("?\0");
    }
}
