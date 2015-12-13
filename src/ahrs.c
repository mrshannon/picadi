///////////////////////////////////////////////////////////////////////
// File: ahrs.c
// Header: ahrs.h
// Author: Michael R. Shannon
// Written: Sunday, December 06, 2015
// Updated: Saturday, December 12, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include "stdint.h"
#include "mathlib.h"
#include "imu.h"
#include "ahrs.h"


bool ahrsUpdate(int16_t *yaw, int16_t *pitch, int16_t *roll){

    int16_t axt, ayt, azt, mxt, myt, mzt;
    int32_t ax, ay, az, mx, my, mz;
    int32_t rollSin, rollCos, pitchSin, pitchCos;
    int32_t tmpA, tmpB, tmpC, tmpD, tmpE;

    // Read data from IMU and convert to plane based coordinate system.
    // X forward, Y starbord, and Z down, but invert gravity vector as
    // well so gravity is positive.
    ahrsReadAcc(&axt, &ayt, &azt);
    ahrsReadMag(&mxt, &myt, &mzt);
    ax = -axt; ay = ayt; az = azt;
    mx = mxt; my = -myt; mz = -mzt;

    // Calculate the roll angle.
    *roll = atan216(ay, az);

    // Limit roll angle to -180 to 180.
    if (*roll > TRIG16_CYCLE/2){
        *roll -= TRIG16_CYCLE;
    }

    // Compute trig functions of roll.
    rollSin = sin16(*roll);
    rollCos = cos16(*roll);

    // Calculate the pitch angle.
    tmpA = (ay*rollSin)/TRIG16_ONE;
    tmpB = (az*rollCos)/TRIG16_ONE;
    *pitch = atan216(-ax, tmpA + tmpB);

    // Limit pitch to -90 to +90
    if (*pitch > TRIG16_CYCLE/4){
        *pitch = TRIG16_CYCLE/2 - *pitch;
    }
    if (*pitch < -TRIG16_CYCLE/4){
        *pitch = -TRIG16_CYCLE/2 - *pitch;
    }

    // Compute trig functions of pitch.
    pitchSin = sin16(*pitch);
    pitchCos = cos16(*pitch);

    // Calculate the yaw angle.
    tmpA = (mx*pitchCos)/TRIG16_ONE;
    tmpB = (mz*pitchSin)/TRIG16_ONE;
    tmpC = (((mz*rollSin)/TRIG16_ONE)*pitchCos)/TRIG16_ONE;
    tmpD = (((mx*rollSin)/TRIG16_ONE)*pitchSin)/TRIG16_ONE;
    tmpE = (my*rollCos)/TRIG16_ONE;
    *yaw = atan216(tmpC - tmpD - tmpE, tmpA + tmpB);

    // Check for validity of solution.
    tmpA = (ax*ax)/IMU_ONE + (ay*ay)/IMU_ONE + (az*az)/IMU_ONE;
    if (tmpA < IMU_ACC_MAG_MIN || IMU_ACC_MAG_MAX < tmpA){
        return false; // possible errors
    }

    // Result is valid.
    return true;
}


void ahrsReadAcc(int16_t *x_ptr, int16_t *y_ptr, int16_t *z_ptr){

    uint8_t i;
    int32_t acc;

    // Compute average x acceleration over last half second.
    acc = 0;
    ATOMIC(
        for (i = 0; i < IMU_BUFFER_LENGTH; ++i){
            acc += (int32_t)imuAccX[i].int16;
        }
    );
    *x_ptr = acc/((int32_t)IMU_BUFFER_LENGTH);

    // Compute average y acceleration over last half second.
    acc = 0;
    ATOMIC(
        for (i = 0; i < IMU_BUFFER_LENGTH; ++i){
            acc += (int32_t)imuAccY[i].int16;
        }
    );
    *y_ptr = acc/((int32_t)IMU_BUFFER_LENGTH);

    // Compute average z acceleration over last half second.
    acc = 0;
    ATOMIC(
        for (i = 0; i < IMU_BUFFER_LENGTH; ++i){
            acc += (int32_t)imuAccZ[i].int16;
        }
    );
    *z_ptr = acc/((int32_t)IMU_BUFFER_LENGTH);
}


void ahrsReadMag(int16_t *x_ptr, int16_t *y_ptr, int16_t *z_ptr){

    uint8_t i;
    int32_t acc;    // accumulator

    // Compute average x acceleration over last half second.
    acc = 0;
    ATOMIC(
        for (i = 0; i < IMU_BUFFER_LENGTH; ++i){
            acc += (int32_t)imuMagX[i].int16;
        }
    );
    *x_ptr = acc/((int32_t)IMU_BUFFER_LENGTH);

    // Compute average y acceleration over last half second.
    acc = 0;
    ATOMIC(
        for (i = 0; i < IMU_BUFFER_LENGTH; ++i){
            acc += (int32_t)imuMagY[i].int16;
        }
    );
    *y_ptr = acc/((int32_t)IMU_BUFFER_LENGTH);

    // Compute average z acceleration over last half second.
    acc = 0;
    ATOMIC(
        for (i = 0; i < IMU_BUFFER_LENGTH; ++i){
            acc += (int32_t)imuMagZ[i].int16;
        }
    );
    *z_ptr = acc/((int32_t)IMU_BUFFER_LENGTH);
}
