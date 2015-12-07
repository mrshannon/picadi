////////////////////////////////////////////////////////////////////////
// File: imu.h
// Author: Michael R. Shannon
// Written: Friday, December 04, 2015
// Updated: Sunday, December 06, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library is used to talk to the LSM303D acceleromerter and
//      compass sensor.
//
////////////////////////////////////////////////////////////////////////


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


extern union bytes2 imuAccX;
extern union bytes2 imuAccY;
extern union bytes2 imuAccZ;
extern union bytes2 imuMagX;
extern union bytes2 imuMagY;
extern union bytes2 imuMagZ;


void imuInit(void);


void imuISR(void);


#endif
