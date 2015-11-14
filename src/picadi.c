///////////////////////////////////////////////////////////////////////
// PIC ADI (Attitude Direction Indicator)
////////////////////////////////////////////////////////////////////////
// File: pacadi.c
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Thursday, November 12, 2015
// Device: PIC18F87K22
// Fosc: 10 MHz
// Instruction Clock: Fosc/4
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include "oled.h"
#pragma config FOSC=HS1, PWRTEN=ON, BOREN=ON, BORV=2, PLLCFG=OFF
#pragma config WDTEN=OFF, CCP2MX=PORTC, XINST=OFF


// Declaration of ISRs.
void isrh(void);
void isrl(void);



// High priority interrupt redirect.
#pragma code highVector=0x08
void highVector(void){
    _asm GOTO isrh _endasm;
}
#pragma code


// Low priority interrupt redirect.
#pragma code lowVector=0x18
void lowVector(void){
    _asm GOTO isrl _endasm;
}
#pragma code


void main(){

    ////////////////////////////////////////////////////////////////////
    //// INITIALIZATION
    ////////////////////////////////////////////////////////////////////
    oledInit();


    ////////////////////////////////////////////////////////////////////
    //// MAIN LOOP
    ////////////////////////////////////////////////////////////////////
    while (1){

    }
}


// Description
//      The high priority interrupt handler.
//
#pragma interrupt isrh nosave=TBLPTR,TABLAT,PCLATH,PCLATU,\
                              section(".tmpdata"),section("MATH_DATA")
void isrh(void){
    // high priority interrupt code
}


// Description:
//      The low priority interrupt handler.
//
#pragma interruptlow isrl nosave=TBLPTR,TABLAT,PCLATH,PCLATU,\
                                 section(".tmpdata"),section("MATH_DATA")
void isrl(void){
    // low priority interrupt code
}
