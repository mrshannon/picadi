// File: led.c
// Author: Michael R. Shannon
// Written: Tuesday, November 10, 2015
// Updated: Sunday, November 15, 2015
// Header: led.h
// Instruction Clock: Fosc/4
// 
// See header file for documentation.


#include <p18cxxx.h>
#include <delays.h>
#include "stdint.h"
#include "led.h"


// LED configuration bits.
#define TRISBBITS 0b00001111


void ledInit(void){

    // Setup LED pins and turn them all off.
    TRISB = TRISBBITS;
    LED_ALIVE_OFF();
    LED_D2_OFF();
    LED_D3_OFF();
    LED_D4_OFF();

    // Run startup sequence.
    LED_D2_ON();                // turn on D2
    Delay10KTCYx(125);          // 0.5 second delay
    LED_D2_OFF();               // turn off D2
    LED_D3_ON();                // turn on D2
    Delay10KTCYx(125);          // 0.5 second delay
    LED_D3_OFF();               // turn off D2
    LED_D4_ON();                // turn on D2
    Delay10KTCYx(125);          // 0.5 second delay
    LED_D4_OFF();               // turn off D2
}
