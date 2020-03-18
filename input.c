#include <stdint.h>
#include <pic32mx.h>
#include "input.h"
#include "config.h"

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
*/1
int getbtns(void)
{
    /* Bits 7-5 correspond to BTN4-BTN2 */
    return (PORTD >> 5) & 0x7;
}

void update_inputs() {
    int switches = getsw();
    input_status.sw1 = switches & 0x1;
    input_status.sw2 = switches & 0x2;
    input_status.sw3 = switches & 0x4;
    input_status.sw4 = switches & 0x8;

    int buttons = getbtns();
    input_status.btn1 = buttons & 0x1;
    input_status.btn2 = buttons & 0x2;
    input_status.btn3 = buttons & 0x4;
}

void init_timer() {
	volatile int* ptrise = (volatile int*) 0xbf886100;

    //We initialize PORTE by using TRISE
	*ptrise &= 0xffffff00;
    TRISD |= 0xfe0; //111111100000. ora med 1or. Rör ej resten

    //timer - 2
    TMR2 = 0;
    T2CON = 0x70;
    PR2 = TMR2PERIOD; //periodregister
    T2CONSET = 0x8000; //bit 15 set to 1 (timer on)
}

short timer_ready() {
    if(IFS(0) & 0x100 )
    {
        IFSCLR(0) = 0x100;
        return 1;
    }
	return 0;
}

void setup_hardware() {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	// init timer
	init_timer();
}

