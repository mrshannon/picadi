////////////////////////////////////////////////////////////////////////
// File: util.h
// Author: Michael R. Shannon
// Written: Sunday, November 15, 2015
// Updated: Monday, November 30, 2015
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


#define PI 3.14159265f


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
//      Wrapper to fix type of literal strings.
//
// Input:
//      str:
//          A string literal.
//
#define STR(str) ((const rom far char *)str)


// Description:
//      Function to load a literal string into a global RAM buffer for
//      use in string functions.
//
// Input:
//      const rom char *src:
//          A Sting located in program memory.
//
// Output (char *):
//      A pointer to a global buffer where the source string has been
//      copied.
//
#define STR_MAX_LENGTH 256
char *str(const rom char *src);


#endif // UTIL_H
