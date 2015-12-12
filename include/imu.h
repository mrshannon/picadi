////////////////////////////////////////////////////////////////////////
// File: imu.h
// Author: Michael R. Shannon
// Written: Friday, December 04, 2015
// Updated: Friday, December 11, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library is used to talk to the LSM303D accelerometer and
//      compass sensor.  It uses low priority interrupts to keep a 0.1
//      second buffer of accelerometer and magnetometer values full of
//      the most recent data.  The frequency of this data is 100 Hz.
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"
#include "util.h"
#include "spilib.h"


#ifndef IMU_H
#define IMU_H


#define IMU_ONE 16383L // integer value treated as 1
#define IMU_ACC_MAG_MIN 14745L // IMU_ONE*0.90
#define IMU_ACC_MAG_MAX 18021L // IMU_ONE*1.10


// IMU pin select/unselect macros.
#define IMU_SELECT_PIN_OUTPUT() (TRISHbits.TRISH5 = 0)
#define IMU_SELECT() (LATHbits.LATH5 = 0)
#define IMU_DESELECT() (LATHbits.LATH5 = 1)


// IMU interrupt flag.
#define IMU_INT_FLAG (INTCON3bits.INT2IF)


// IMU buffers.
#define IMU_BUFFER_LENGTH 25
extern uint8_t imuAccIdx;   // last written to buffer location (acceleration)
extern uint8_t imuMagIdx;   // last written to buffer location (magnetic field)
extern union bytes2 imuAccX[IMU_BUFFER_LENGTH];
extern union bytes2 imuAccY[IMU_BUFFER_LENGTH];
extern union bytes2 imuAccZ[IMU_BUFFER_LENGTH];
extern union bytes2 imuMagX[IMU_BUFFER_LENGTH];
extern union bytes2 imuMagY[IMU_BUFFER_LENGTH];
extern union bytes2 imuMagZ[IMU_BUFFER_LENGTH];


// Description:
//      Initialize accelerometer/magnetometer by setting up SPI module
//      1, writing LSM303D initialization bytes, and setting up RB2 as
//      the interrupt pin for new accelerometer/magnetometer data.
//
void imuInit(void);


// Description:
//      Finish initialization, must be called after interrupts are
//      enabled.  This fixes the interrupt lock problem and preloads the
//      buffers.
//
void imuSpinup(void);


// Description:
//      Low priority IMU interrupt service routine.  Handles reading
//      acceleration and magnetic field data from the LSM303D when data
//      is available and loading it into the buffers.
//
void imuISR(void);


#endif
