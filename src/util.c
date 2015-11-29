////////////////////////////////////////////////////////////////////////
// File: util.c
// Header: util.h
// Author: Michael R. Shannon
// Written: Sunday, November 15, 2015
// Updated: Sunday, November 15, 2015
// Device: PIC18F87K22
// Compiler: C18
// Fosc: 10 MHz
// Instruction Clock: Fosc/4
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include <delays.h>
#include "stdint.h"
#include "util.h"


void delayxms(uint8_t ms){

    uint8_t ms4 = ms/4;

    // Delay multiples of 4 milliseconds.
    Delay10KTCYx(ms4*PLLMUL);

    // Delay for remainder of milliseconds.
    // NOTE: This is done with a switch statement to encourage the
    //       compiler to implement it as a jump table.  Also, notice the
    //       lack of break statements.
    switch (ms - ms4*4){
        case 3:
            delay1ms();
        case 2:
            delay1ms();
        case 1:
            delay1ms();
    }
}


void delayxs(uint8_t s){

    uint16_t i;

    for (i = ((uint16_t)s)*PLLMUL; i > 0; --i){
        Delay10KTCYx(250);  // 1 second delay
    }

}


int8_t abs8(int8_t n){
    if (n < 0){
        return -n;
    }
    return n;
}


int16_t abs16(int16_t n){
    if (n < 0){
        return -n;
    }
    return n;
}


int24_t abs24(int24_t n){
    if (n < 0){
        return -n;
    }
    return n;
}


int32_t abs32(int32_t n){
    if (n < 0){
        return -n;
    }
    return n;
}
