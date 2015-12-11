////////////////////////////////////////////////////////////////////////
// File: util.c
// Header: util.h
// Author: Michael R. Shannon
// Written: Sunday, November 15, 2015
// Updated: Friday, December 11, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include <delays.h>
#include <string.h>
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


static char stringBuffer[STR_MAX_LENGTH];
char *str(const rom char *src){
    stringBuffer[STR_MAX_LENGTH-1] = '\0';
    strncpypgm2ram(stringBuffer, STR(src), STR_MAX_LENGTH-1);
    return stringBuffer;
}
