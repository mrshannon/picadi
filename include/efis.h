////////////////////////////////////////////////////////////////////////
// File: efis.h
// Author: Michael R. Shannon
// Written: Monday, November 30, 2015
// Updated: Tuesday, December 08, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library draws an EFIS to the frame buffer using the
//      graphics library.  It does not compute the attitude solution,
//      only displays it.
//
////////////////////////////////////////////////////////////////////////


#include "graphics.h"
#include "stdint.h"
#include "stdbool.h"


#ifndef EFIS_H
#define EFIS_H


void efisDraw(int16_t yaw, int16_t pitch, int16_t roll, bool valid);


void efisDrawInvalid(bool valid);


void efisDrawCompass(int16_t yaw);


void efisDrawAI(int16_t pitch, int16_t roll);


void efisDrawPlane(void);


void efisDrawPitch(int16_t pitch, int16_t rollSin, int16_t rollCos);


void efisDrawRoll(int16_t roll);


void efisDrawRollLine(int16_t tehta, uint8_t length);


void efisDrawHorizon(int16_t pitch, int16_t rollSin, int16_t rollCos);


bool efisHorizon(int16_t pitch, int16_t rollSin, int16_t rollCos,
                 int16_t *x0_ptr, int16_t *y0_ptr,
                 int16_t *x1_ptr, int16_t *y1_ptr);


int8_t efisPoints(uint8_t xa[], uint8_t ya[],
                  int16_t x0, int16_t y0,
                  int16_t x1, int16_t y1);


void efisDrawHorizonAsPolygon(uint8_t xa[4], uint8_t ya[4], uint8_t color);


#endif // EFIS_H
