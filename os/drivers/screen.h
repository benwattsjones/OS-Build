#ifndef DRIVER_SCREEN_H
#define DRIVER_SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 24 // only use 24 of 25 rows as cursor cannot be seen on 25th
#define MAX_COLS 80
#define DEF_STYLE 0x07

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

#define CURSOR_LOCATION_HIGH_OFFSET 0xe
#define CURSOR_LOCATION_LOW_OFFSET 0xf

// Returns offset (in bytes NOT chars)
int get_cursor();
// Sets cursor to arguement of offset (in bytes NOT chars)
void set_cursor(int offset);

// Prints string (MUST be null-terminated) at specified row and column
// (in chars NOT bytes), with specified attribute byte. Setting col=-1
// and/or row=-1 will by default print from the current location of the
// cursor. Setting attribute_byte=0 will use the DEF_STYLE.
void print_at(char *message, int col, int row, char attribute_byte);
// prints message (MUST be null-terminated) from current cursor position
// in default styling (DEF_STYLE)
void print(char *message);

// Clears contents of screen and sets cursor to start. This function should
// be called upon kernel loading to set up for printing
void clear_screen();

#endif
