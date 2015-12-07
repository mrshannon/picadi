////////////////////////////////////////////////////////////////////////
// File: imu.h
// Author: Michael R. Shannon
// Written: Friday, December 04, 2015
// Updated: Sunday, December 06, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library is used to talk to the LSM303D accelerometer and
//      compass sensor.  It uses low priority interrupts to keep a half
//      second buffer of accelerometer and magnetometer values full of
//      the most recent data.  The frequency of this data is 50 Hz.
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"
#include "util.h"
#include "spilib.h"


#ifndef IMU_H
#define IMU_H


// IMU pin select/unselect macros.
#define IMU_SELECT_PIN_OUTPUT() (TRISHbits.TRISH5 = 0)
#define IMU_SELECT() (LATHbits.LATH5 = 0)
#define IMU_DESELECT() (LATHbits.LATH5 = 1)


// IMU interupt flag.
#define IMU_INT_FLAG (INTCON3bits.INT2IF)


// IMU buffers.
#define IMU_BUFFER_LENGTH 25
extern uint8_t imuAccIdx;
extern uint8_t imuMagIdx;
extern union bytes2 imuAccX[IMU_BUFFER_LENGTH];
extern union bytes2 imuAccY[IMU_BUFFER_LENGTH];
extern union bytes2 imuAccZ[IMU_BUFFER_LENGTH];
extern union bytes2 imuMagX[IMU_BUFFER_LENGTH];
extern union bytes2 imuMagY[IMU_BUFFER_LENGTH];
extern union bytes2 imuMagZ[IMU_BUFFER_LENGTH];


// Initialize IMU.
void imuInit(void);


// Finish initialization, must be called after interrupts are enabled.
void imuSpinup(void);


// Low priority IMU interrupt service routine.
void imuISR(void);


#endif
