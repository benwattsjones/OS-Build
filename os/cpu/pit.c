#include <stdint.h>

#include "ioports.h"

/* He we setup the PIT chip on the motherboard to send an interrupt for each
 * clock tick of the timer.
 * First we set the Control Word Register to 0x36 (00110110b). This sets the
 * mode of the timer and allows us to then send the frequency.
 *
 * 00110110b
 *        +- The binary counter will be in binary not binary coded decimal
 *     +++-- Set PIT to mode 3 - square wave generator. In this mode, the OUT
 *           pin of the PIT is high for half the cycle and low the other half
 *   ++----- Read/ load least significant byte of counter to counter register,
 *           then most significant byte
 * ++------- Select counter zero to use as the counter register
 *
 * The clock speed/ frequency of the PIT is 1193180Hz. We divide this value by
 * the frequency of the system timer we want, the result being the value of
 * the counter. This works because the PIT counts down from the counter value
 * by one for each clock cycle, untill it gets to one, then restarts. In 
 * square wave mode, it will fire an interrupt each time it gets to one.
 * Thus an interrupt is fired at our desired frequency.
 * We send the counter value to the counter register as defined in the 
 * control word register above.
 */

#define PIT_CONTROL_WORD_REGISTER 0x43
#define PIT_COUNTER_0_REGISTER 0x40
#define PIT_FREQUENCY 1193180

void timer_init()
{
    portByteOut(PIT_CONTROL_WORD_REGISTER, 0x36);
    uint16_t count = PIT_FREQUENCY / 20;
    portByteOut(PIT_COUNTER_0_REGISTER, count & 0x0f);
    portByteOut(PIT_COUNTER_0_REGISTER, count >> 8);
}

