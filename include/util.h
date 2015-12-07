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


union bytes1 {
    int8_t int8;
    uint8_t uint8;
    struct {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
    };
};


union bytes2 {
    int16_t int16;
    uint16_t uint16;
    struct {
        int8_t int8A;
        int8_t int8B;
    };
    struct {
        uint8_t uint8A;
        uint8_t uint8B;
    };
    struct {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
        uint8_t bit8:1;
        uint8_t bit9:1;
        uint8_t bit10:1;
        uint8_t bit11:1;
        uint8_t bit12:1;
        uint8_t bit13:1;
        uint8_t bit14:1;
        uint8_t bit15:1;
    };
};


union bytes3 {
    int24_t int24;
    uint24_t uint24;
    struct {
        int8_t int8A;
        int8_t int8B;
        int8_t int8C;
    };
    struct {
        uint8_t uint8A;
        uint8_t uint8B;
        uint8_t uint8C;
    };
    struct {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
        uint8_t bit8:1;
        uint8_t bit9:1;
        uint8_t bit10:1;
        uint8_t bit11:1;
        uint8_t bit12:1;
        uint8_t bit13:1;
        uint8_t bit14:1;
        uint8_t bit15:1;
        uint8_t bit16:1;
        uint8_t bit17:1;
        uint8_t bit18:1;
        uint8_t bit19:1;
        uint8_t bit20:1;
        uint8_t bit21:1;
        uint8_t bit22:1;
        uint8_t bit23:1;
    };
};


union bytes4 {
    float float32;
    int32_t int32;
    uint32_t uint32;
    struct {
        int16_t int16A;
        int16_t int16B;
    };
    struct {
        uint16_t uint16A;
        uint16_t uint16B;
    };
    struct {
        int8_t int8A;
        int8_t int8B;
        int8_t int8C;
        int8_t int8D;
    };
    struct {
        uint8_t uint8A;
        uint8_t uint8B;
        uint8_t uint8C;
        uint8_t uint8D;
    };
    struct {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
        uint8_t bit8:1;
        uint8_t bit9:1;
        uint8_t bit10:1;
        uint8_t bit11:1;
        uint8_t bit12:1;
        uint8_t bit13:1;
        uint8_t bit14:1;
        uint8_t bit15:1;
        uint8_t bit16:1;
        uint8_t bit17:1;
        uint8_t bit18:1;
        uint8_t bit19:1;
        uint8_t bit20:1;
        uint8_t bit21:1;
        uint8_t bit22:1;
        uint8_t bit23:1;
        uint8_t bit24:1;
        uint8_t bit25:1;
        uint8_t bit26:1;
        uint8_t bit27:1;
        uint8_t bit28:1;
        uint8_t bit29:1;
        uint8_t bit30:1;
        uint8_t bit31:1;
    };
};


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
