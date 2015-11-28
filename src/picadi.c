///////////////////////////////////////////////////////////////////////
// PIC ADI (Attitude Direction Indicator)
////////////////////////////////////////////////////////////////////////
// File: pacadi.c
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Thursday, November 12, 2015
// Device: PIC18F87K22
// Compiler: C18
// Fosc: 10 MHz
// Instruction Clock: Fosc/4
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include "config.h"
#include "led.h"
#include "oled.h"
#include "util.h"
#include "graphics.h"
#pragma config FOSC=HS1, PWRTEN=ON, BOREN=ON, BORV=2, PLLCFG=ON
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

    uint8_t i;
    int16_t x, y;

    ////////////////////////////////////////////////////////////////////
    //// INITIALIZATION
    ////////////////////////////////////////////////////////////////////
    ledInit();
    LED_D2_ON();
    oledInit();
    LED_D3_ON();
    oledWriteBuffer();
    LED_D4_ON();

    delayxs(2);
    LED_D4_OFF();

    /* for (i = 0; i < 100; ++i){ */
    /*     glPoint(i, 48, GL_COLOR_INVERT); */
    /* } */
    /* for (i = 0; i < 100; ++i){ */
    /*     glPoint(i, 24, GL_COLOR_WHITE); */
    /* } */
    /* for (i = 0; i < 100; ++i){ */
    /*     glPoint(i, 56, GL_COLOR_BLACK); */
    /* } */

    /* glClear(); */
    /* oledWriteBuffer(); */
    for (i = 0; i < 64; ++i){
        glVLine(i+25, 63, i, GL_COLOR_INVERT);
    }
    oledWriteBuffer();
    /* glHLine(0, 127, 63, GL_COLOR_WHITE); */
    /* glHLine(127, 0, 25, GL_COLOR_BLACK); */
    /* glHLine(0, 127, 24, GL_COLOR_INVERT); */
    glClear();
    /* glHLine(0, 64, 0, GL_COLOR_WHITE); */
    /* glPoint(0, 0, GL_COLOR_BLACK); */
    /* glPoint(1, 0, GL_COLOR_WHITE); */
    /* glPoint(2, 0, GL_COLOR_WHITE); */
    /* glPoint(3, 0, GL_COLOR_WHITE); */
    /* glPoint(4, 0, GL_COLOR_WHITE); */
    /* glPoint(0, 1, GL_COLOR_WHITE); */
    /* glPoint(0, 2, GL_COLOR_WHITE); */
    /* glPoint(0, 3, GL_COLOR_WHITE); */
    /* glPoint(0, 4, GL_COLOR_WHITE); */
    /* glLine(0, 0, 127, 200, GL_COLOR_WHITE); */
    /* glPoint(127, 20, GL_COLOR_WHITE); */
    for (x = -10; x < GL_FRAME_WIDTH + 10; ++x){
        glClear();
        glHLine(x-3, x+3, x/3, GL_COLOR_WHITE);
        glVLine(x, x/4-3, x/4+3, GL_COLOR_WHITE);
        oledWriteBuffer();
        /* delayxms(10); */
    }



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
