////////////////////////////////////////////////////////////////////////
// File: util.h
// Author: Michael R. Shannon
// Written: Sunday, November 15, 2015
// Updated: Sunday, November 15, 2015
// Device: PIC18F87K22
// Fosc: 10 MHz
// Instruction Clock: Fosc/4
// Description:
//      A library for utility functions.
//
////////////////////////////////////////////////////////////////////////


#include <delays.h>
#include "config.h"
#include "stdint.h"


#ifndef UTIL_H
#define UTIL_H


// Description:
//      Busy wait for 1 millisecond.
//
#define delay1ms() Delay100TCYx(25*PLLMUL)


// Description:
//      Delay by a given number of milliseconds between 0 and 255.
//
// Input:
//      uint_t ms:
//          Number of milliseconds to busy wait for.
//
void delayxms(uint8_t ms);


#endif // UTIL_H
