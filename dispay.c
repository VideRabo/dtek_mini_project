/*
   parts from 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

/* Declare a helper function which is local to this file */
static void num32asc( char * s, int );

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)
static uint8_t mapPixels[128][4];
char textbuffer[4][16];
/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc)
{
    int i;
		for(i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data)
{
		//skicka in pixlar
	  while(!(SPI2STAT & 0x08));	//Väntar tills alla element har skickats från transmit buffer så att buffern kan skrivas över med 'data'. SPI-TBE
		SPI2BUF = data;	//writear alla elemnt
		while(!(SPI2STAT & 1)); //Medan read buffer inte är full gör inget (SPI-RBF)
		return SPI2BUF;	//Läs av read buffer o sätt cleara SPI-RBF;
}

void display_init(void)
{
    DISPLAY_CHANGE_TO_COMMAND_MODE;
		quicksleep(10);
		DISPLAY_ACTIVATE_VDD;
		quicksleep(1000000);

		spi_send_recv(0xAE);
		DISPLAY_ACTIVATE_RESET;
		quicksleep(10);
		DISPLAY_DO_NOT_RESET;
		quicksleep(10);

  	spi_send_recv(0x8D);
  	spi_send_recv(0x14);

  	spi_send_recv(0xD9);
    spi_send_recv(0xF1);

  	DISPLAY_ACTIVATE_VBAT;
  	quicksleep(10000000);

  	spi_send_recv(0xA1);
  	spi_send_recv(0xC8);

  	spi_send_recv(0xDA);
  	spi_send_recv(0x20);

    spi_send_recv(0xAF);
}


/* Vektorn mapPixels används för att mappa pixlarna på skärmen. mapPixels är en vektor med storlek
   128x4. 128 pixlar i x-led, och de accessas enligt ordning 0-127 (med origo i vänster hörn, högst upp, på displayen).
   y-led är indelat i 4 rader (0 - 3). Således är en kolumn indelad i 4 lika stora delar, vardera håller 8 pixlar.

   Hur vi specifikt väljer pixel i y-led: (skicka in pixel som vanligt)
   Funktionen drawPixel tar emot y-värdet och listar ut vilken rad pixeln skall läggas på. (y / 8, vilket max kan ge 4, då ymax är 31).
   Resten används sedan för att bestämma offset ner i y-led, från den raden vi står på. Låt säg att vi skickar in y = 31:
   -->
   int y/8 = 3
   y % 8 = 7

   Tolkning av resultat: rad 3, pixeloffset 7. Alltså pixeln längst ner på skärmen (beroende på var vi ligger i x-led.
*/
void drawPixel(uint8_t x, uint8_t y, int activatePixel)
{

    if(x < 0 || x > 127 || y < 0 || y > 31)
			  return;

    /* Välj page. Max y = 31 (annars ritar vi ej på skärmen), ymax/8 ger 3 */
	  uint8_t column = (uint8_t) (y / 8);


    //say pixel was 7, this means bit 7 will be 1 because of the shift, im using OR so that previously
		//written pixels in the same column dont get wiped out when activating new pixel.
		if(activatePixel == 1)
		    mapPixels[x][column] |= (0x1 << (y % 8)); //y%8 ger remainder, alltså hur långt ner i y-led om vi sätter offset från vår page till 0 högst upp

		if(activatePixel == 0)
		    mapPixels[x][column] &= ~(0x1 << (y % 8));

		return;
}


/* Function drawRectangle draws or erases a filled in rectangle from x0 --> x1 (width)
   and y0 --> y1 (height).
   activatePixel == 1 (draw)
   activatePixel == 0 (erase)
*/
void drawRectangle(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1, int activatePixel)
{
    uint8_t y = y0;
		uint8_t x;

		for(x = x0; x < x1; x++)
		{
		    for(y = y0; y < y1; y++)
				    drawPixel(x,y, activatePixel);
	  }
}

/* Function clearScreen clears all pixels on the screen */
void clearScreen()
{
    uint8_t x;
		uint8_t y;

		for(x = 0; x < 128; x++)
		{
		    for(y = 0; y < 32; y++)
				    drawPixel(x,y, 0);
	  }
}

void display_string(int line, char *s)
{
    int i;

  	if(line < 0 || line >= 4)
    		return;
  	if(!s)
    		return;

  	for(i = 0; i < 16; i++)
  		if(*s)
      {
    			textbuffer[line][i] = *s;
    			s++;
  		}
      else
    			textbuffer[line][i] = ' ';
}

/* Function display_update updates the display */
/* select == 1 --> update pixels
   select == 0 --> update text */
void display_update(int select)
{
  	int i, j, k;
  	int c;
  	for(i = 0; i < 4; i++)
    {
    		DISPLAY_CHANGE_TO_COMMAND_MODE;
    		spi_send_recv(0x22); //set page address
    		spi_send_recv(i); // pick page

    		spi_send_recv(0x0);	//set lower column adress
    		spi_send_recv(0x10); //set upper column adress

    		DISPLAY_CHANGE_TO_DATA_MODE;

        if(select == 1)
        {
            for(j = 0; j < 128; j++)
            {
                c = mapPixels[j][i];
                spi_send_recv(c);
            }
        }

        if(select == 0)
        {
            for(j = 0; j < 16; j++)
            {
                c = textbuffer[i][j];
                if(c & 0x80)
                    continue;

                for(k = 0; k < 8; k++)
                    spi_send_recv(font[c*8 + k]);
            }
        }
  	}
}


/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, int n )
{
    int i;
    for( i = 28; i >= 0; i -= 4 )
        *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}

void tick( unsigned int * timep )
{
    /* Get current value, store locally */
    register unsigned int t = * timep;
    t += 1; /* Increment local copy */

    /* If result was not a valid BCD-coded time, adjust now */

    if( (t & 0x0000000f) >= 0x0000000a ) t += 0x00000006;
    if( (t & 0x000000f0) >= 0x00000060 ) t += 0x000000a0;
    /* Seconds are now OK */

    if( (t & 0x00000f00) >= 0x00000a00 ) t += 0x00000600;
    if( (t & 0x0000f000) >= 0x00006000 ) t += 0x0000a000;
    /* Minutes are now OK */

    if( (t & 0x000f0000) >= 0x000a0000 ) t += 0x00060000;
    if( (t & 0x00ff0000) >= 0x00240000 ) t += 0x00dc0000;
    /* Hours are now OK */

    if( (t & 0x0f000000) >= 0x0a000000 ) t += 0x06000000;
    if( (t & 0xf0000000) >= 0xa0000000 ) t = 0;
    /* Days are now OK */

    * timep = t; /* Store new value */
}

#define ITOA_BUFSIZ ( 24 )
char * itoaconv( int num )
{
    register int i, sign;
    static char itoa_buffer[ ITOA_BUFSIZ ];
    static const char maxneg[] = "-2147483648";

    itoa_buffer[ ITOA_BUFSIZ - 1 ] = 0;   /* Insert the end-of-string marker. */
    sign = num;                           /* Save sign. */
    if( num < 0 && num - 1 > 0 )          /* Check for most negative integer */
    {
      for( i = 0; i < sizeof( maxneg ); i += 1 )
      itoa_buffer[ i + 1 ] = maxneg[ i ];
      i = 0;
    }
    else
    {
      if( num < 0 ) num = -num;           /* Make number positive. */
      i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
      do {
        itoa_buffer[ i ] = num % 10 + '0';/* Insert next digit. */
        num = num / 10;                   /* Remove digit from number. */
        i -= 1;                           /* Move index to next empty position. */
      } while( num > 0 );
      if( sign < 0 )
      {
        itoa_buffer[ i ] = '-';
        i -= 1;
      }
    }
    /* Since the loop always sets the index i to the next empty position,
     * we must add 1 in order to return a pointer to the first occupied position. */
    return( &itoa_buffer[ i + 1 ] );
}
