#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

#define PORTD		PIC32_R (0x860D0)

/* Möjligtvis onödigt, men ha temporärt */
void inputinit()
{
    volatile int* ptrise = (volatile int*) 0xbf886100;
    *ptrise &= 0xffffff00;
    TRISD |= 0xfe0; //0000 .. 1111 1110 0000
}

/* Function getsw returns the status of the toggle switches on the board.
   The four least significant bits of the return value contain current data from switches.
*/
int getsw(void)
{
    /* Bits 11-8 correpond to SW4 - SW1. */
    return (PORTD >> 8) & 0xf;
}

/* Function getbtns returns the current status of push buttons BTN4 - BTN2.
   The three least significant bits of the return value contain current data from push buttons.
*/
int getbtns(void)
{
    /* Bits 7-5 correspond to BTN4-BTN2 */
    return (PORTD >> 5) & 0x7;
}
