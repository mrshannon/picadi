////////////////////////////////////////////////////////////////////////
// File: efis.h
// Author: Michael R. Shannon
// Written: Monday, November 30, 2015
// Updated: Thursday, December 03, 2015
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


#ifndef EFIS_H
#define EFIS_H


void efisDraw(int16_t yaw, int16_t pitch, int16_t roll);


void efisAI(float pitch, float roll);


bool efisHorizon(int16_t pitch, int16_t roll,
                 int16_t *x0_ptr, int16_t *y0_ptr,
                 int16_t *x1_ptr, int16_t *y1_ptr);


int8_t efisPoints(uint8_t xa[], uint8_t ya[],
                  int16_t x0, int16_t y0,
                  int16_t x1, int16_t y1);

void efisDrawHorizon(int16_t pitch, int16_t roll);


void efisDrawHorizonAsPolygon(uint8_t xa[4], uint8_t ya[4], uint8_t color);


void efisPitch(float pitch, float roll);


void efisRoll(float roll);


void efisPlane();


void efisCompass(float yaw);


#endif // EFIS_H
