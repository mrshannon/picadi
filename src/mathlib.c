///////////////////////////////////////////////////////////////////////
// File: graphics.c
// Header: graphics.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Sunday, November 15, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include "stdint.h"
#include "stdbool.h"
#include "mathlib.h"


static const int16_t sin16_table[TRIG16_TABLE_SIZE+1] = {
        0,  1608,  3212,  4808,  6393,  7962,  9512, 11039,
    12539, 14010, 15446, 16846, 18204, 19519, 20787, 22005,
    23170, 24279, 25329, 26319, 27245, 28105, 28898, 29621,
    30273, 30852, 31356, 31785, 32137, 32412, 32609, 32728,
    32767
};


// 16384 equals 45 degrees, divide by 8 to get actual angle
static const int16_t atan16_table[TRIG16_TABLE_SIZE+1] = {
        0,   652,  1302,  1950,  2594,  3233,  3866,  4493,
     5110,  5719,  6318,  6907,  7484,  8050,  8603,  9144,
     9672, 10187, 10689, 11177, 11653, 12115, 12564, 13000,
    13424, 13835, 14234, 14620, 14995, 15359, 15711, 16053,
    16384
};


int8_t abs8(int8_t n){
    if (n < 0){
        return -n;
    }
    return n;
}


int16_t abs16(int16_t n){
    if (n < 0){
        return -n;
    }
    return n;
}


int24_t abs24(int24_t n){
    if (n < 0){
        return -n;
    }
    return n;
}


int32_t abs32(int32_t n){
    if (n < 0){
        return -n;
    }
    return n;
}


int16_t sin16(int16_t theta){

    uint8_t idx;
    int16_t result;
    bool negative, inverse;

    // Fix range of theta.
    theta &= 0x3FFF;

    // Determine the quadrant.
    negative = (theta & 0b0010000000000000) > 0;
    inverse  = (theta & 0b0001000000000000) > 0;

    // Limit theta to first quadrant and invert if necessary.
    theta &= 0xFFF;
    if (inverse){
        theta = 0x1000 - theta;
    }

    // Get indices into the lookup table.
    idx = theta/128;

    // Linearly interpolate table.
    result = yIntercept(idx*128, sin16_table[idx],
                        (idx+1)*128, sin16_table[idx+1], theta);

    // Negate result if in quadrant 3 or 4.
    if (negative){
        result = -result;
    }

    return result;
}


int16_t cos16(int16_t theta){
    /* if (theta <= -12288 #<{(| -1.5*TRIG16_CYCLE |)}>#){ */
    /*     theta += TRIG16_CYCLE; */
    /* } */
    return sin16(TRIG16_CYCLE/4 - theta);
}


int32_t tan16(int16_t theta){
    return (int32_t)(sin16(theta) * 32768) / (int32_t)(cos16(theta));
}


// Description:
//      Internal only version of arctangent.
//
// Input:
//      int16_t xy:
//          y/x scaled from 0 to 1 to 0 to 32767.  All inputs are
//          in first octant.
//
// Output (int16_t):
//      TRIG16 scaled angle in the 1st octant corresponding to yx. 
//
int16_t _atan16(int16_t yx){

    uint8_t idx;
    int16_t result;

    // Get indices into the lookup table.
    idx = yx/1024;

    // Linearly interpolate table.
    result = yIntercept(idx*1024, atan16_table[idx],
                        (idx+1)*1024, atan16_table[idx+1], yx);

    return result/8;
}


// Algorithm from: http://www.coranac.com/documents/arctangent/
int16_t atan216(int16_t y, int16_t x){

    int16_t offset = 0;
    int32_t _x, _y, t;

    // Shortcut for 0 or 180 degrees.
    if (y == 0){
        return x >= 0 ? 0 : TRIG16_CYCLE/2;
    }

    _x = x;
    _y = y;

    // Shift angle into first octant.
    // rotate by -180 degrees
    if (_y < 0){
        _x = -_x;
        _y = -_y;
        offset += TRIG16_CYCLE/2;
    }
    // rotate by -90 degrees
    if (_x <= 0){
        t = _x;
        _x = _y;
        _y = -t;
        offset += TRIG16_CYCLE/4;
    }
    // rotate by -45 degrees
    if (_x <= _y){
        t = _y - _x;
        _x = _x + _y;
        _y = t;
        offset += TRIG16_CYCLE/8;
    }

    return _atan16((_y*TRIG16_ONE)/_x) + offset;
}


void rotate16_(int16_t *xPtr, int16_t *yPtr, int16_t s, int16_t c){

    int32_t x, y;

    // Store local copy.
    x = *xPtr;
    y = *yPtr;

    // Rotate the points.
    *xPtr = (int16_t)((x*(int32_t)c - y*(int32_t)s)/(int32_t)INT16_MAX);
    *yPtr = (int16_t)((x*(int32_t)s + y*(int32_t)c)/(int32_t)INT16_MAX);
}


int16_t yIntercept(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x){
    int32_t num, dom;
    num = ((int32_t)(x - x0)) * ((int32_t)(y1 - y0));
    dom = (int32_t)(x1 - x0);
    return num/dom + y0;
}
