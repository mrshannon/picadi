///////////////////////////////////////////////////////////////////////
// PIC ADI (Attitude Direction Indicator)
////////////////////////////////////////////////////////////////////////
// File: pacadi.c
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Tuesday, December 08, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// NOTE: The LED and LCD libraries are not actually used.  There where
// included in this project to aid with debugging.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include "config.h"
#include "util.h"
#include "stdint.h"
#include "stdbool.h"
#include "mathlib.h"
#include "oled.h"
#include "efis.h"
#include "imu.h"
#include "ahrs.h"


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


uint16_t tcounter = 0;


void main(){

    int16_t yaw, pitch, roll;
    bool valid;

    uint8_t invalidCooldown = 0;

    // Initialize all subsystems.
    oledInit();
    oledWriteBuffer();
    imuInit();

    // Enable interrupts.
    RCONbits.IPEN = 1;      // Enable priority levels
    INTCONbits.GIEL = 1;    // Enable low-priority interrupts to CPU.
    INTCONbits.GIEH = 1;    // Enable all interrupts.

    // Spin-up IMU and delay on splash screen.
    imuSpinup();
    delayxs(2);

    // Main loop.
    while (true){

        // Update AHRS solution.
        valid = ahrsUpdate(&yaw, &pitch, &roll);

        // Clear the frame buffer.
        glClear();

        // Write the EFIS to the frame buffer.
        efisDraw(yaw, pitch, roll, valid);

        // Write the frame buffer.
        oledWriteBuffer();

        /* delayxs(1); */
    }
}


// Description
//      The high priority interrupt handler.
//
#pragma interrupt isrh
void isrh(void){
    // high priority interrupt code
}


// Description:
//      The low priority interrupt handler.
//
#pragma interruptlow isrl nosave=TBLPTR,TABLAT,PCLATH,PCLATU,\
                          section(".tmpdata"),section("MATH_DATA")
void isrl(void){
    while(IMU_INT_FLAG) {
        imuISR();
    }
}
