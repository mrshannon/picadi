////////////////////////////////////////////////////////////////////////
// File: stdbool.h
// Author: Michael R. Shannon
// Written: Tuesday, December 01, 2015
// Updated: Friday, December 11, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This is a header only library that implements something similar
//      to the stdbool.h library in C99.
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"


#ifndef STDBOOL_H
#define STDBOOL_H


#define bool uint8_t

#define true 1

#define false 0

#define __bool_true_false_are_define 1


#endif // STDBOOL_H
