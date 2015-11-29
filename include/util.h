////////////////////////////////////////////////////////////////////////
// File: util.h
// Author: Michael R. Shannon
// Written: Sunday, November 15, 2015
// Updated: Sunday, November 15, 2015
// Device: PIC18F87K22
// Compiler: C18
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


// Description:
//      Delay by a given number of seconds.
//
// Input:
//      uint_t ms:
//          Number of seconds to busy wait for.
//
void delayxs(uint8_t s);


// Description:
//      Functions to compute the absolute value of each signed integer
//      type.
//
// Input:
//      intX_t n:
//          Number to take absolute value of.
// 
// Output (intX_t):
//      Absolute value of n.
//
int8_t abs8(int8_t n);
int16_t abs16(int16_t n);
int24_t abs24(int24_t n);
int32_t abs32(int32_t n);


#endif // UTIL_H
