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
#include "mathlib.h"


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
