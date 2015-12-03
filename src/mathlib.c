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
