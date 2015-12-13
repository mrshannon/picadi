////////////////////////////////////////////////////////////////////////
// File: ahrs.h
// Author: Michael R. Shannon
// Written: Sunday, December 06, 2015
// Updated: Saturday, December 12, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library reads data from the IMU buffers and generates
//      attitude/heading solutions.
//
//      The solutions are based on:
//
//      https://www.freescale.com/files/sensors/doc/app_note/AN4248.pdf
//
//      and
//
//      https://www.pololu.com/file/0J434/LSM303DLH-compass-app-note.pdf
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"
#include "stdbool.h"


#ifndef AHRS_H
#define AHRS_H


// Description:
//      Compute a new attitude and heading solution from the IMU buffers.
//
//      NOTE: The accelerometer and magnetometer data used are backward
//            looking boxcar filtered over the past 0.25 seconds.
//
// Input:
//      int16_t *yaw:
//          Pointer to store heading result, from 0 to 360 degrees in
//          TRIG16 units.
//
//      int16_t *pitch:
//          Pointer to store pitch result, from -90 to +90 degrees in
//          TRIG16 units.
//
//      int16_t roll:
//          Pointer to store roll result (positive clockwise) from -180
//          to 180 degrees in TRIG16 units.
//
bool ahrsUpdate(int16_t *yaw, int16_t *pitch, int16_t *roll);


// Description:
//      Read accelerometer data and filter with backwards looking
//      boxcar filter over entire buffer (0.25 seconds).
//
// Input:
//      int16_t *x_ptr:
//          Point to store x-component of acceleration.
//
//      int16_t *y_ptr:
//          Point to store y-component of acceleration.
//
//      int16_t *z_ptr:
//          Point to store z-component of acceleration.
//
void ahrsReadAcc(int16_t *x_ptr, int16_t *y_ptr, int16_t *z_ptr);


// Description:
//      Read magnetometer data and filter with backwards looking
//      boxcar filter over entire buffer (0.25 seconds).
//
// Input:
//      int16_t *x_ptr:
//          Point to store x-component of magnetic field.
//
//      int16_t *y_ptr:
//          Point to store y-component of magnetic field.
//
//      int16_t *z_ptr:
//          Point to store z-component of magnetic field.
//
void ahrsReadMag(int16_t *x_ptr, int16_t *y_ptr, int16_t *z_ptr);


#endif // AHRS_H
