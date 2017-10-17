#include <stdint.h>

#include "screen.h"
/*char *scan_codes[0x40] = {"?\0", "?\0", "1\0", "2\0", "3\0", "4\0", "5\0", "6\0",
                     "7\0", "8\0", "9\0", "0\0", "-\0", "=\0", "?\0", "?\0",
                     "q\0", "w\0", "e\0", "r\0", "t\0", "y\0", "u\0", "i\0",
                     "o\0", "p\0", "[\0", "]\0", "\n\0", "?\0", "a\0", "s\0", 
                     "d\0", "f\0", "g\0", "h\0", "j\0", "k\0", "l\0", ";\0",
                     "'\0", "#\0", "?\0", "\\\0", "z\0", "x\0", "c\0", "v\0",
                     "b\0", "n\0", "m\0", ",\0", ".\0", "/\0", "?\0", "*\0",
                     "?\0", " \0"};
*/

// covers 0x0 - 0x39 of original XT scan codes.
// Called by keyboard IRQ1 handler in interrupts.c in os/cpu/
void printKeyInput(uint8_t scan_code)
{
    char *scan_codes = "?\0 ?\0 1\0 2\0 3\0 4\0 5\0 6\0 7\0 8\0 9\0 0\0 -\0 =\0 ?\0 ?\0 q\0 w\0 e\0 r\0 t\0 y\0 u\0 i\0 o\0 p\0 [\0 ]\0 \n\0 ?\0 a\0 s\0 d\0 f\0 g\0 h\0 j\0 k\0 l\0 ;\0 '\0 #\0 ?\0 \\\0 z\0 x\0 c\0 v\0 b\0 n\0 m\0 ,\0 .\0 /\0 ?\0 *\0 ?\0  \0";
    if (scan_code <= 0x39) {
        char *str = scan_codes + scan_code * 3;
        print(str);
    } else if (scan_code < 0x80) { // Key up codes at 0x80+
        print("?\0");
    }
}
