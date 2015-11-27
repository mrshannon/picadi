////////////////////////////////////////////////////////////////////////
// File: stdint.h
// Author: Michael R. Shannon
// Written: Tuesday, November 03, 2015
// Updated: Tuesday, November 03, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This is a header only library that implements something similar
//      to the stdint.h library in C99.
//
////////////////////////////////////////////////////////////////////////

#ifndef STDINT_H
#define STDINT_H

// Signed integers.
typedef signed char int8_t;             // 8-bit signed integer
typedef int int16_t;                    // 16-bit signed integer
typedef short long int24_t;             // 24-bit signed integer
typedef long int32_t;                   // 32-bit signed integer
typedef long intmax_t;                  // maximum sized signed integer

// Unsigned integers.
typedef unsigned char uint8_t;          // 8-bit unsigned integer
typedef unsigned int uint16_t;          // 16-bit unsigned integer
typedef unsigned short long uint24_t;   // 24-bit unsigned integer
typedef unsigned long uint32_t;         // 32-bit unsigned integer
typedef unsigned long uintmax_t;        // maximum sized unsigned integer

// Singed integer limits.
#define INT8_MIN -128
#define INT8_MAX 127
#define INT16_MIN -32768
#define INT16_MAX 32767
#define INT24_MIN -8388608
#define INT24_MAX 8388607
#define INT32_MIN -2147483648
#define INT32_MAX 2147483647
#define INTMAX_MIN -2147483648
#define INTMAX_MAX 2147483647

// Unsigned integer limits.
#define UINT8_MAX 256
#define UINT16_MAX 65536
#define UINT24_MAX 16777216
#define UINT32_MAX 4294967296
#define UINTMAX_MAX 4294967296

#endif // STDINT_H
