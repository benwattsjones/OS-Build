#include <stdint.h>
#include "screen.h"

#define DEF_STYLE 0

struct time_elapsed {
    uint16_t milisecs;
    uint8_t secs;
    uint8_t mins;
    uint8_t hours;
};

static struct time_elapsed TimeElapsed = {0, 0, 0, 0};

void updateTimeElapsed()
{
    TimeElapsed.milisecs += 50;
    if (TimeElapsed.milisecs >= 1000){
        TimeElapsed.secs += 1;
        TimeElapsed.milisecs = 0;
    }
    if (TimeElapsed.secs >= 60) {
        TimeElapsed.mins += 1;
        TimeElapsed.secs = 0;
    } 
    if (TimeElapsed.mins >= 60) {
        TimeElapsed.hours += 1;
        TimeElapsed.mins = 0;
    }
    if (TimeElapsed.hours >= 99) {
        TimeElapsed.hours = 0;
    }
}

void convertTwoDigitAscii(uint8_t input_num, char *output_ascii) 
{
    char tens_num = input_num / 10;
    output_ascii[0] = tens_num + '0';
    char units_num = input_num - tens_num * 10;
    output_ascii[1] = units_num + '0';
}

void printTimeElapsed()
{
    updateTimeElapsed();
    char time_element[3] = {0, 0, 0}; // need extra for null ptr
    convertTwoDigitAscii(TimeElapsed.hours, time_element);
    printHeader(time_element, 30, DEF_STYLE);
    convertTwoDigitAscii(TimeElapsed.mins, time_element);
    printHeader(time_element, 33, DEF_STYLE);
    convertTwoDigitAscii(TimeElapsed.secs, time_element);
    printHeader(time_element, 36, DEF_STYLE);
}
