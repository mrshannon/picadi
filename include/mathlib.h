////////////////////////////////////////////////////////////////////////
// File: mathlib.h
// Author: Michael R. Shannon
// Written: Sunday, November 29, 2015
// Updated: Sunday, November 29, 2015
// Device: PIC18F87K22
// Compiler: C18
// Fosc: 10 MHz
// Instruction Clock: Fosc/4
// Description:
//      A library for utility functions.
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"


#ifndef MATHLIB_H
#define MATHLIB_H


#define PI 3.14159265f


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


#endif // MATHLIB_H
