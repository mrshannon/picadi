////////////////////////////////////////////////////////////////////////
// File: ahrs.h
// Author: Michael R. Shannon
// Written: Sunday, December 06, 2015
// Updated: Sunday, December 06, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library reads data from the IMU buffers and generates
//      attitude/heading solutions.
//
//      The solutions are based on:
//      https://www.freescale.com/files/sensors/doc/app_note/AN4248.pdf
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"
#include "stdbool.h"


#ifndef AHRS_H
#define AHRS_H


bool ahrsUpdate(int16_t *yaw, int16_t *pitch, int16_t *roll);


void ahrsReadAcc(int16_t *x_ptr, int16_t *y_ptr, int16_t *z_ptr);


void ahrsReadMag(int16_t *x_ptr, int16_t *y_ptr, int16_t *z_ptr);


#endif // AHRS_H
