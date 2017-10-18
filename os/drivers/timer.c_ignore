#include <stdint.h>

#define MILISECONDS_PER_SECOND 1000
#define TICKS_PER_SECOND 20
#define SECONDS_PER_MINUTE 60

// Note will revert to zero after 65535 ticks (~55 mins)
static uint16_t ticks_elapsed = 0;

uint64_t ascii_time = 0;

void updateTimeElapsed()
{
    time_elapsed++;
}

void formatTimeElapsed()
{
    uint16_t num_ticks = ticks_elapsed;
    uint8_t mins_elapsed = num_ticks / (TICKS_PER_SECOND * SECONDS_PER_MINUTE);
    num_ticks = num_ticks - mins_elapsed;
    uint8_t secs_elapsed = num_ticks / TICKS_PER_SECOND;
    num_ticks = num_ticks - secs_elapsed;
    uint8_t milisecs_elapsed = num_ticks * MILISECONDS_PER_SECOND;

    updateAsciiTime(mins_elapsed, secs_elapsed, milisecs_elapsed);
}

void updateAsciiTime(uint8_t mins_elapsed, uint8_t secs_elapsed, uint8_t milisecs_elapsed)
{
    char 

}

