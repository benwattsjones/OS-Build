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

#ifndef DRIVERS_SCREEN_H
#define DRIVERS_SCREEN_H

#include <stdint.h>

#define BLUE_FOREGROUND 0x01
#define GREEN_FOREGROUND 0x02
#define CYAN_FOREGROUND 0x03
#define RED_ORANGE_FOREGROUND 0x04
#define MAGENTA_FOREGROUND 0x05
#define BROWN_FOREGROUND 0x06
#define GREY_WHITE_FOREGROUND 0x07

#define INTENSE_FOREGROUND 0x08

#define BLUE_BACKGROUND 0x10
#define GREEN_BACKGROUND 0x20
#define CYAN_BACKGROUND 0x30
#define RED_ORANGE_BACKGROUND 0x40
#define MAGENTA_BACKGROUND 0x50
#define BROWN_BACKGROUND 0x60
#define GREY_WHITE_BACKGROUND 0x70

#define INTENSE_BACKGROUND 0x80

// For user defined functions - not hardware defaults
#define DEFAULT_COL -1
#define DEFAULT_ROW -1
#define DEFAULT_STYLE 0

void printAt(char *message, int32_t col, int32_t row, char attribute_byte);
void printHeader(char *message, int32_t col, char attribute_byte);
void clearScreen();
void printk(const char *fmt, ...);

#endif

