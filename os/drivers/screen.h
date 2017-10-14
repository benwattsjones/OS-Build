#ifndef DRIVER_SCREEN_H
#define DRIVER_SCREEN_H

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

// Returns offset (in bytes NOT chars)
int getCursor();
// Sets cursor to arguement of offset (in bytes NOT chars)
void setCursor(int offset);

// Prints string (MUST be null-terminated) at specified row and column
// (in chars NOT bytes), with specified attribute byte. Setting col=-1
// and/or row=-1 will by default print from the current location of the
// cursor. Setting attribute_byte=0 will use the DEF_STYLE.
void printAt(char *message, int col, int row, char attribute_byte);
// prints message (MUST be null-terminated) from current cursor position
// in default styling (DEF_STYLE)
void print(char *message);

// Clears contents of screen and sets cursor to start. This function should
// be called upon kernel loading to set up for printing
void clearScreen();

#endif
