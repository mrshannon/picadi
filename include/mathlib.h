////////////////////////////////////////////////////////////////////////
// File: mathlib.h
// Author: Michael R. Shannon
// Written: Sunday, November 29, 2015
// Updated: Tuesday, December 01, 2015
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


// Definitions for trig functions.
#define TRIG16_TABLE_SIZE 32
#define TRIG16_CYCLE 0x4000 // 16384
#define TRIG16_MAX_ANGLE 0x3FFF // 16383
#define TRIG16_MIN_ANGLE 0
#define TRIG16_ONE 32767L


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


// Description:
//      Integer based sine function.  There are 16384 units per
//      revolution.  360 deg == 2*PI == 16384.
//
// Input:
//      int16_t theta:
//          Angle to compute sine for, 16384 per cycle.
//
// Output (int16_t):
//      Sine of theta where -1 to +1 is scaled from -32767 to +32767.
//
int16_t sin16(int16_t theta);


// Description:
//      Integer based cosine function.  There are 16384 units per
//      revolution.  360 deg == 2*PI == 16384.
//
// Input:
//      int16_t theta:
//          Angle to compute cosine for, 16384 per cycle.
//
// Output (int16_t):
//      Cosine of theta where -1 to +1 is scaled from -32767 to +32767.
//
int16_t cos16(int16_t theta);


// Description:
//      Integer based tangent function.  There are 16384 units per
//      revolution.  360 deg == 2*PI == 16384.
//
// Input:
//      int16_t theta:
//          Angle to compute tangent for, 16384 per cycle.
//
// Output (int32_t):
//      Tangent of theta where -1 to +1 is scaled from -32767 to +32767.
//
int32_t tan16(int16_t theta);


// Description:
//      Integer based atan2 function.  There are 16384 units per
//      revolution.
//
//      Algorithm from: http://www.coranac.com/documents/arctangent/
//
// Input:
//      int16_t y:
//          y (vertical) value
//      int16_t x:
//          x (horizontal) value
//
int16_t atan216(int16_t y, int16_t x);


#define fromDeg(theta) \
    ((int16_t)(((int32_t)TRIG16_CYCLE)*((int32_t)theta)/360L))
#define toDeg(theta) (((int32_t)theta)*360L/TRIG16_CYCLE)
#define rotate16(xPtr, yPtr, theta) \
    (rotate_(xPtr, yPtr, sin16(theta), cos16(theta)))
void rotate16_(int16_t *xPtr, int16_t *yPtr,  int16_t s, int16_t c);


#define xIntercept(x0, y0, x1, y1, y) yIntercept(y0, x0, y1, x1, y)
int16_t yIntercept(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x);


#endif // MATHLIB_H
