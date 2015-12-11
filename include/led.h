////////////////////////////////////////////////////////////////////////
// File: led.h
// Author: Michael R. Shannon
// Written: Tuesday, November 10, 2015
// Updated: Friday, December 11, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      A library to handle initialization and timing of the LEDs.
//
////////////////////////////////////////////////////////////////////////


#ifndef LED_H
#define LED_H


// LED control macros.
#define LED_ALIVE_ON() LATBbits.LATB4 = 0
#define LED_ALIVE_OFF() LATBbits.LATB4 = 1
#define LED_D2_ON() LATBbits.LATB5 = 1
#define LED_D2_OFF() LATBbits.LATB5 = 0
#define LED_D3_ON() LATBbits.LATB6 = 1
#define LED_D3_OFF() LATBbits.LATB6 = 0
#define LED_D4_ON() LATBbits.LATB7 = 1
#define LED_D4_OFF() LATBbits.LATB7 = 0


// Description:
//      Setup LED pins as outputs and run LED startup sequence.
//
void ledInit(void);


#endif // LED_H
