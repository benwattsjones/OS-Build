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
#include <stdarg.h>

#include "screen.h"
#include "../cpu/ioports.h"

/* Monitors can be controlled by the motherboards VGA (video graphics array)
 * controller. For reasons of backwards compatability, this emulates a CRT
 * (cathode ray tube) microcontroller. Computers boot in VGA color text
 * mode with dimensions 80x25 characters. A font is already defined in the
 * internal memory of the VGA display device. The character display of 
 * the screens internal memory buffer is memory-mapped from address
 * 0xb8000 (VIDEO_MEMORY) (for 7 pages) in RAM. The registers of device 
 * controllers are mapped into an I/O address space in RAM. These mappings are
 * known as ports. The CPU issues commands to the device controllers via
 * buses.
 *
 * VIDEO_MEMORY is interpreted sequentially, with 2-bytes per character.
 * The first byte is the ascii code for the character. The second byte
 * is the attribute byte, which defines the color of the character. The '\n'
 * character will not start a new line - This must be implemented manually.
 *
 * Attribute byte definition:
 *   Bits 0 - 2: Foreground color
 *     Bit 0: Blue
 *     Bit 1: Green
 *     Bit 2: Red
 *   Bit 3: Foreground intensity
 *   Bits 4 - 6: Background color
 *     Bit 4: Blue
 *     Bit 5: Green
 *     Bit 6: Red
 *   Bit 7: Background intensity
 * Regarding combinations of the color bits:
 *   000 = Black (but set to default, DEF_STYLE, if attribute byte is 0x0)
 *   001 = Blue
 *   010 = Green
 *   011 = Cyan
 *   100 = Red (orange if relevant intensity bit is 1)
 *   101 = Magenta 
 *   110 = Brown
 *   111 = Light Grey (white if relevant intensity bit is one)
 * Thus, DEF_STYLE is light grey foreground on black background
 *
 * CPU interactions with device microcontrollers are implemented in 
 * assembly (see ioports.asm). The CRT microcontroller contains an
 * Index Register (memory-mapped to 0x3d5, REG_SCREEN_CTRL), and a
 * Data Register (memory-mapped to 0x3d4, REG_SCREEN_DATA) [and others].
 * In order to determine a propery, or set a write a value to a property,
 * in the data register, we must make the data register point to the 
 * correct location. This is done by writing an index offset value
 * to the index register.
 *
 * The properties pointed to by the data register upon writing to the index
 * register with different index offsets are shown below:
 *  0x0    Horizontal total
 *  0x1    Horizontal display enable end
 *  0x2    Start horizontal blanking
 *  0x3    End horizontal blanking
 *  0x4    Start horizontal retrace pulse
 *  0x5    End horizontal retrace
 *  0x6    Vertical total
 *  0x7    Overflow
 *  0x8    Preset row scan
 *  0x9    Maximum scan line
 *  0xA    Cursor start
 *  0xB    Cursor end
 *  0xC    Start address high
 *  0xD    Start address low
 *  0xE    Cursor location high
 *  0xF    Cursor location low
 *  0x10   Vertical retrace start
 *  0x11   Vertical retrace end
 *  0x12   Vertica display enable end
 *  0x13   Offset
 *  0x14   Underline location
 *  0x15   Start vertical blanking
 *  0x16   End vertical blanking
 *  0x17   CRT mode control
 *  0x18   Line compare
 *
 * Thus, using 0xE (CURSOR_LOCATION_HIGH_OFFSET) and 0xF
 * (CURSOR_LOCATION_LOW_OFFSET), I/O ports can be used to read and write
 * values for the cursor location.
 */

#define VIDEO_ADDRESS 0x300b8000 // note: this is physical address 0xb8000
#define MAX_ROWS 24 // only use 24 of 25 rows as cursor cannot be seen on 25th
#define MAX_COLS 80
#define DEF_STYLE 0x07
#define BYTES_PER_CHAR 2

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

#define CURSOR_LOCATION_HIGH_OFFSET 0xe
#define CURSOR_LOCATION_LOW_OFFSET 0xf

int32_t handleScrolling(int32_t cursor_offset);

int32_t getCursor()
{
    portByteOut(REG_SCREEN_CTRL, CURSOR_LOCATION_HIGH_OFFSET);
    int32_t offset = portByteIn(REG_SCREEN_DATA) << 8;
    portByteOut(REG_SCREEN_CTRL, CURSOR_LOCATION_LOW_OFFSET);
    offset += portByteIn(REG_SCREEN_DATA);
    return offset * BYTES_PER_CHAR;
}

void copyMemory(char *source, char *dest, int32_t no_bytes)
{
    int32_t i;
    for (i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void setCursor(int32_t offset)
{
    offset = offset / BYTES_PER_CHAR; 
    portByteOut(REG_SCREEN_CTRL, CURSOR_LOCATION_HIGH_OFFSET);
    portByteOut(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    portByteOut(REG_SCREEN_CTRL, CURSOR_LOCATION_LOW_OFFSET);
    portByteOut(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

int32_t getVideoMemoryOffset(int32_t col, int32_t row)
{
    int32_t offset;
    if (col >= 0 && row >= 0) {
        offset = (row * MAX_COLS + col) * BYTES_PER_CHAR;
    } else {
        offset = getCursor();
    }
    return offset;
}

// moves cursor to space before next line so new line with moveCursorAlong().
int32_t updateOffsetNewLine(int32_t offset)
{
    int32_t row = offset / (BYTES_PER_CHAR * MAX_COLS);
    return (row * MAX_COLS + MAX_COLS - 1) * BYTES_PER_CHAR;
}

void moveCursorAlong(int32_t offset)
{
    offset += BYTES_PER_CHAR;
    offset = handleScrolling(offset);
    setCursor(offset);
}

void printChar(char character, int32_t col, int32_t row, char attribute_byte)
{
    volatile unsigned char *vidmem = (volatile unsigned char *) VIDEO_ADDRESS;
    attribute_byte = attribute_byte ? attribute_byte : DEF_STYLE;
    int32_t offset = getVideoMemoryOffset(col, row);
    if (character == '\n') {
        offset = updateOffsetNewLine(offset);
    } else if (character == '\b') {
        vidmem[offset - 2] = ' ';
        vidmem[offset - 1] = attribute_byte;
        offset -= BYTES_PER_CHAR * 2;
    } else {
        vidmem[offset] = character;
        vidmem[offset + 1] = attribute_byte;
    }
    moveCursorAlong(offset);
}

int convertColRowToOffset(int32_t col, int32_t row)
{
    return (row * MAX_COLS + col) * BYTES_PER_CHAR;
}

int32_t handleScrolling(int32_t cursor_offset)
{
    // Nothing changes if no overflow
    if (cursor_offset < MAX_ROWS * MAX_COLS * BYTES_PER_CHAR) {
        return cursor_offset;
    }
    int32_t i;
    // Move each lines memory back a line (exc. header hence 2)
    for (i = 2; i < MAX_ROWS; i++) {
        copyMemory((char *) (convertColRowToOffset(0, i) + VIDEO_ADDRESS),
                    (char *) (convertColRowToOffset(0, i-1) + VIDEO_ADDRESS),
                    MAX_COLS * BYTES_PER_CHAR);
    }
    // Wipe former contents of last line (Don't replace with 0s else cursor won't show)
    char *last_line = (char *) (convertColRowToOffset(0, MAX_ROWS-1) + VIDEO_ADDRESS);
    for (i = 0; i < MAX_COLS*2; i=i+BYTES_PER_CHAR) {
        last_line[i] = ' ';
        last_line[i + 1] = DEF_STYLE;
    }
    // Put cursor at start of last line
    return convertColRowToOffset(0, MAX_ROWS - 1);
}

void printAt(char *message, int32_t col, int32_t row, char attribute_byte)
{
    int32_t i = 0;
    while (message[i] != 0) {
        printChar(message[i++], col, row, attribute_byte);
    }
}

void clearScreen() 
{
    int32_t row;
    int32_t col;
    for (row = 0; row < MAX_ROWS; row++) {
        for (col = 0; col < MAX_COLS; col++) {
            printChar(' ', col, row, 0);
        }
    }
    setCursor(convertColRowToOffset(0, 1));
}

void printHeader(char *message, int32_t col, char attribute_byte)
{
    col = (col > 0) ? col : 0;
    attribute_byte = attribute_byte ? attribute_byte : DEF_STYLE;
    volatile unsigned char *header_mem = (volatile unsigned char *) VIDEO_ADDRESS;
    int32_t i = 0;
    col = col * BYTES_PER_CHAR;
    while (message[i] != '\0' && message[i] != '\n') {
        header_mem[col] = message[i];
        header_mem[col + 1] = attribute_byte;
        i++;
        col = col + BYTES_PER_CHAR;
    }
}

static const char hex_ascii_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                         '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

// ascii must be length 9 array (or more) initialized to all zero.
void convertHexToAscii(uint32_t hex, char *ascii)
{
    int i, j;
    uint32_t remainder;
    for (i = 0; i < 8; i++) { // 32-bit hex is max 8 ascii chars
        remainder = hex % 16;
        hex = hex / 16;
        ascii[i] = hex_ascii_table[remainder];
        if (hex == 0) break;
    }
    char swap_value;
    for (j = 0; i > 0; i--, j++) {
        swap_value = ascii[j];
        ascii[j] = ascii[i];
        ascii[i] = swap_value;
    }
}

void printHex(uint32_t hex)
{
    char ascii[9] = {0};
    convertHexToAscii(hex, ascii);
    printAt(ascii, DEFAULT_COL, DEFAULT_ROW, DEFAULT_STYLE);
}

void printk(const char *fmt, ...)
{
    va_list valist;
    const char *iter_char;
    uint32_t uint_mod;
    va_start(valist, fmt);
    for (iter_char = fmt; *iter_char != '\0'; iter_char++) {
        if (*iter_char != '%') {
            printChar(*iter_char, DEFAULT_COL, DEFAULT_ROW, DEFAULT_STYLE);
            continue;
        }
        switch(*++iter_char)
        {
        case 'x':
            uint_mod = va_arg(valist, uint32_t);
            printHex(uint_mod);
            break;
        case '%':
            printChar('%', DEFAULT_COL, DEFAULT_ROW, DEFAULT_STYLE);
            break;
        default:
            printChar('%', DEFAULT_COL, DEFAULT_ROW, DEFAULT_STYLE);
            printChar(*iter_char, DEFAULT_COL, DEFAULT_ROW, DEFAULT_STYLE);
            break;
        }
    }
    va_end(valist);
}

