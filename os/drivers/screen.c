#include "screen.h"
#include "../cpu/ioports.h"

#define VIDEO_ADDRESS 0xb8000  
#define MAX_ROWS 24 // only use 24 of 25 rows as cursor cannot be seen on 25th
#define MAX_COLS 80
#define DEF_STYLE 0x07

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

#define CURSOR_LOCATION_HIGH_OFFSET 0xe
#define CURSOR_LOCATION_LOW_OFFSET 0xf


// Useful reference: http://www.brokenthorn.com/Resources/OSDev10.html
// This file codes for a screen driver for text-based display

// Monitors can be controlled by the motherboards VGA (video graphics array)
// controller. For reasons of backwards compatability, this emulates a CRT
// (cathode ray tube) microcontroller. Computers boot in VGA color text
// mode with dimensions 80x25 characters. A font is already defined in the
// internal memory of the VGA display device. The character display of 
// the screens internal memory buffer is memory-mapped from address
// 0xb8000 (VIDEO_MEMORY) (for 7 pages) in RAM. The registers of device 
// controllers are mapped into an I/O address space in RAM. These mappings are
// known as ports. The CPU issues commands to the device controllers via
// buses.

// VIDEO_MEMORY is interpreted sequentially, with 2-bytes per character.
// The first byte is the ascii code for the character. The second byte
// is the attribute byte, which defines the color of the character. The '\n'
// character will not start a new line - This must be implemented manually.

// Attribute byte definition:
//   Bits 0 - 2: Foreground color
//     Bit 0: Blue
//     Bit 1: Green
//     Bit 2: Red
//   Bit 3: Foreground intensity
//   Bits 4 - 6: Background color
//     Bit 4: Blue
//     Bit 5: Green
//     Bit 6: Red
//   Bit 7: Background intensity
// Regarding combinations of the color bits:
//   000 = Black (but set to default, DEF_STYLE, if attribute byte is 0x0)
//   001 = Blue
//   010 = Green
//   011 = Cyan
//   100 = Red (orange if relevant intensity bit is 1)
//   101 = Magenta
//   110 = Brown
//   111 = Light Grey (white if relevant intensity bit is one)
// Thus, DEF_STYLE is light grey foreground on black background

// CPU interactions with device microcontrollers are implemented in 
// assembly (see ioports.asm). The CRT microcontroller contains an
// Index Register (memory-mapped to 0x3d5, REG_SCREEN_CTRL), and a
// Data Register (memory-mapped to 0x3d4, REG_SCREEN_DATA) [and others].
// In order to determine a propery, or set a write a value to a property,
// in the data register, we must make the data register point to the 
// correct location. This is done by writing an index offset value
// to the index register.

// The properties pointed to by the data register upon writing to the index
// register with different index offsets are shown below:
//  0x0    Horizontal total
//  0x1    Horizontal display enable end
//  0x2    Start horizontal blanking
//  0x3    End horizontal blanking
//  0x4    Start horizontal retrace pulse
//  0x5    End horizontal retrace
//  0x6    Vertical total
//  0x7    Overflow
//  0x8    Preset row scan
//  0x9    Maximum scan line
//  0xA    Cursor start
//  0xB    Cursor end
//  0xC    Start address high
//  0xD    Start address low
//  0xE    Cursor location high
//  0xF    Cursor location low
//  0x10   Vertical retrace start
//  0x11   Vertical retrace end
//  0x12   Vertica display enable end
//  0x13   Offset
//  0x14   Underline location
//  0x15   Start vertical blanking
//  0x16   End vertical blanking
//  0x17   CRT mode control
//  0x18   Line compare

// Thus, using 0xE (CURSOR_LOCATION_HIGH_OFFSET) and 0xF
// (CURSOR_LOCATION_LOW_OFFSET), I/O ports can be used to read and write
// values for the cursor location.

int handle_scrolling(int cursor_offset);

int getCursor()
{
    portByteOut(REG_SCREEN_CTRL, CURSOR_LOCATION_HIGH_OFFSET);
    int offset = portByteIn(REG_SCREEN_DATA) << 8;
    portByteOut(REG_SCREEN_CTRL, CURSOR_LOCATION_LOW_OFFSET);
    offset += portByteIn(REG_SCREEN_DATA);
    return offset * 2;
}

void memory_copy(char *source, char *dest, int no_bytes)
{
    int i;
    for (i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void setCursor(int offset)
{
    offset = offset / 2; 
    portByteOut(REG_SCREEN_CTRL, CURSOR_LOCATION_HIGH_OFFSET);
    portByteOut(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    portByteOut(REG_SCREEN_CTRL, CURSOR_LOCATION_LOW_OFFSET);
    portByteOut(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

int get_screen_offset(int cols, int rows)
{
    return (rows*MAX_COLS + cols) * 2; 
}

void print_char(char character, int col, int row, char attribute_byte)
{
    volatile unsigned char *vidmem = (volatile unsigned char *) VIDEO_ADDRESS;
    // if attribute byte is zero, assume default style
    if (!attribute_byte) {
        attribute_byte = DEF_STYLE;
    }
    // Get the video memory offset for the screen location
    int offset = 0;
    if (col >= 0 && row >= 0) {
        offset = get_screen_offset(col, row);
    } else {
        offset = getCursor();
    }
    // If newline, set offset to end of current row, so it will be advanced to
    // the first col of the next row
    if (character == '\n') {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(79, rows);
    } else {
        // Otherwise, write the character and its attribute byte to video
        // memory at our calculated offset
        vidmem[offset] = character;
        vidmem[offset + 1] = attribute_byte;
    }
    // Update the offset to the next character cell, which is two bytes 
    // ahead of the current cell.
    offset += 2;
    // Make scrolling adjustment, for when we reach bottom of screen.
    offset = handle_scrolling(offset);
    // Update the cursor position on the screen device
    setCursor(offset);
}

int handle_scrolling(int cursor_offset)
{
    // Nothing changes if no overflow
    if (cursor_offset < MAX_ROWS*MAX_COLS*2) {
        return cursor_offset;
    }
    int i;
    // Move each lines memory back a line - Note first line now lost forever
    for (i = 2; i < MAX_ROWS; i++) {
        memory_copy((char *) (get_screen_offset(0, i) + VIDEO_ADDRESS),
                    (char *) (get_screen_offset(0, i-1) + VIDEO_ADDRESS),
                    MAX_COLS * 2);
    }
    // Wipe former contents of last line
    char *last_line = (char *) (get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS);
    for (i = 0; i < MAX_COLS*2; i++) {
        last_line[i] = 0;
    }
    // Put cursor at start of last line (requires char at position)
    cursor_offset = get_screen_offset(0, MAX_ROWS - 1);
    print_char(' ', 0, MAX_ROWS-1, 0);
//    set_cursor(cursor_offset);
    return cursor_offset;
}

void printAt(char *message, int col, int row, char attribute_byte)
{
    int i = 0;
    while (message[i] != 0) {
        print_char(message[i++], col, row, attribute_byte);
    }
}

void print(char *message)
{
    printAt(message, -1, -1, 0);
}

void clearScreen() 
{
    int row;
    int col;
    for (row = 0; row < MAX_ROWS; row++) {
        for (col = 0; col < MAX_COLS; col++) {
            print_char(' ', col, row, 0);
        }
    }
    setCursor(get_screen_offset(0, 1));
}

void printHeader(char *message, int col, char attribute_byte)
{
    if (!attribute_byte)
        attribute_byte = DEF_STYLE;
    volatile unsigned char *header_mem = (volatile unsigned char *) VIDEO_ADDRESS;
    int i = 0;
    col = col * 2;
    while (message[i] != '\0' && message[i] != '\n') {
        header_mem[col] = message[i];
        header_mem[col + 1] = attribute_byte;
        i++;
        col = col + 2;
    }
}

