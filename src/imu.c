///////////////////////////////////////////////////////////////////////
// File: imu.c
// Header: imu.h
// Author: Michael R. Shannon
// Written: Friday, December 04, 2015
// Updated: Sunday, December 06, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include "stdint.h"
#include "util.h"
#include "spilib.h"
#include "dac.h"
#include "eeprom.h"
#include "sdcard.h"
#include "pressure.h"
#include "imu.h"


// Inertial measurement unit buffers.
uint8_t imuAccIdx;
uint8_t imuMagIdx;
union bytes2 imuAccX[IMU_BUFFER_LENGTH];
union bytes2 imuAccY[IMU_BUFFER_LENGTH];
union bytes2 imuAccZ[IMU_BUFFER_LENGTH];
union bytes2 imuMagX[IMU_BUFFER_LENGTH];
union bytes2 imuMagY[IMU_BUFFER_LENGTH];
union bytes2 imuMagZ[IMU_BUFFER_LENGTH];


// Interrupt macros and defines.
#define INT_PIN_INPUT() (TRISBbits.TRISB2 = 1)
#define INT_RISING_EDGE() (INTCON2bits.INTEDG2 = 1)
#define INT_FALLING_EDGE() (INTCON2bits.INTEDG2 = 0)
#define INT_PRIORITY_LOW() (INTCON3bits.INT2IP = 0)
#define INT_PRIORITY_HIGH() (INTCON3bits.INT2IP = 1)
#define INT_ENABLE() (INTCON3bits.INT2IE = 1)
#define INT_DISABLE() (INTCON3bits.INT2IE = 0)


// Decorator for selection of the IMU's chip-select.
#define IMU(code) do { \
        IMU_SELECT(); \
        code \
        IMU_DESELECT(); \
    } while (0)


// Decorator for selection of IMU chip-select and setting up for
// writing.
#define IMU_READ(address, code) do { \
        IMU_SELECT(); \
        spi1ExchangeByte(0b11000000 | (0b00111111 & address)); \
        code \
        IMU_DESELECT(); \
    } while (0)


// Decorator for selection of IMU chip-select and settin up for
// reading.
#define IMU_WRITE(address, code) do { \
        IMU_SELECT(); \
        spi1ExchangeByte(0b01000000 | (0b00111111 & address)); \
        code \
        IMU_DESELECT(); \
    } while (0)


// IMU Registers
#define TEMP_OUT_L      0x05    //              r
#define TEMP_OUT_H      0x06    //              r
#define STATUS_M        0x07    //              r
#define OUT_X_L_M       0x08    //              r
#define OUT_X_H_M       0x09    //              r
#define OUT_Y_L_M       0x0A    //              r
#define OUT_Y_H_M       0x0B    //              r
#define OUT_Z_L_M       0x0C    //              r
#define OUT_Z_H_M       0x0D    //              r
#define WHO_AM_I        0x0F    // 01001001     r
#define INT_CTRL_M      0x12    // 11101000     rw
#define INT_SRC_M       0x13    //              r
#define INT_THS_L_M     0x14    // 00000000     rw
#define INT_THS_H_M     0x15    // 00000000     rw
#define OFFSET_X_L_M    0x16    // 00000000     rw
#define OFFSET_X_H_M    0x17    // 00000000     rw
#define OFFSET_Y_L_M    0x18    // 00000000     rw
#define OFFSET_Y_H_M    0x19    // 00000000     rw
#define OFFSET_Z_L_M    0x1A    // 00000000     rw
#define OFFSET_Z_H_M    0x1B    // 00000000     rw
#define REFERENCE_X     0x1C    // 00000000     rw
#define REFERENCE_Y     0x1D    // 00000000     rw
#define REFERENCE_Z     0x1E    // 00000000     rw
#define CTRL0           0x1F    // 00000000     rw
#define CTRL1           0x20    // 00000111     rw
#define CTRL2           0x21    // 00000000     rw
#define CTRL3           0x22    // 00000000     rw
#define CTRL4           0x23    // 00000000     rw
#define CTRL5           0x24    // 00011000     rw
#define CTRL6           0x25    // 00100000     rw
#define CTRL7           0x26    // 00000001     rw
#define STATUS_A        0x27    //              r
#define OUT_X_L_A       0x28    //              r
#define OUT_X_H_A       0x29    //              r
#define OUT_Y_L_A       0x2A    //              r
#define OUT_Y_H_A       0x2B    //              r
#define OUT_Z_L_A       0x2C    //              r
#define OUT_Z_H_A       0x2D    //              r
#define FIFO_CTRL       0x2E    // 00000000     rw
#define FIFO_SRC        0x2F    //              r
#define IG_CFG1         0x30    // 00000000     rw
#define IG_SRC1         0x31    //              r
#define IG_THS1         0x32    // 00000000     rw
#define IG_DUR1         0x33    // 00000000     rw
#define IG_CFG2         0x34    // 00000000     rw
#define IG_SRC2         0x35    //              r
#define IG_THS2         0x36    // 00000000     rw
#define IG_DUR2         0x37    // 00000000     rw
#define CLICK_CFG       0x38    // 00000000     rw
#define CLICK_SRC       0x39    //              r
#define CLICK_THS       0x3A    // 00000000     rw
#define TIME_LIMIT      0x3B    // 00000000     rw
#define TIME_LATENCY    0x3C    // 00000000     rw
#define TIME_WINDOW     0x3D    // 00000000     rw
#define ACT_THS         0x3E    // 00000000     rw
#define ACT_DUR         0x3F    // 00000000     rw


// STATUS_M Flags (magnetometer)
// x, y, z axis data overrun has occurred.
#define ZYXMOR      0b10000000
// z-axis data overrun
#define ZMOR        0b01000000
// y-axis data overrun
#define YMOR        0b00100000
// x-axis data overrun
#define XMOR        0b00010000
// x, y, z-axis, new data available
#define ZYXMDA      0b00001000
// z-axis new data available
#define ZMDA        0b00000100
// y-axis new data available
#define YMDA        0b00000010
// z-axis new data available
#define XMDA        0b00000001


// INT_CTRL_M Flags
// enable interrupt recognition on X-axis magnetic data
#define XMIEN       0b10000000
// enable interrupt recognition on Y-axis magnetic data
#define YMIEN       0b01000000
// enable interrupt recognition on Z-axis magnetic data
#define ZMIEN       0b00100000
// interrupt pin configuration
#define PP          0b00000000  // push-pull
#define OD          0b00010000  // open drain
// interrupt polarity
#define IEA_LOW     0b00000000  // active low
#define IEA_HIGH    0b00001000  // active high
// latch magnetic interrupt request on INT_SRC_M register
#define MIEL        0b00000100
// enable 4D
#define _4D         0b00000010
// enable interrupt generator for magnetic data
#define MIEN        0b00000001


// CTRL1 Flags
// acceleration data rate
#define AODR_3      0b00010000  //    3.125 Hz
#define AODR_6      0b00100000  //    6.26 Hz
#define AODR_12     0b00110000  //   12.5 Hz
#define AODR_25     0b01000000  //   25 Hz
#define AODR_50     0b01010000  //   50 Hz
#define AODR_100    0b01100000  //  100 Hz
#define AODR_200    0b01110000  //  200 Hz
#define AODR_400    0b10000000  //  400 Hz
#define AODR_800    0b10010000  //  800 Hz
#define AODR_1600   0b10100000  // 1600 Hz
// block update until read
#define BDU         0b00001000
// enable z-axis of accelerometer
#define AZEN        0b00000100
// enable y-axis of accelerometer
#define AYEN        0b00000010
// enable x-axis of accelerometer
#define AXEN        0b00000001


// CTRL2 Flags
// acceleration anti-alias filter bandwidth
#define ABW_773     0b00000000  // 773 Hz
#define ABW_362     0b10000000  // 362 Hz
#define ABW_194     0b01000000  // 194 Hz
#define ABW_50      0b11000000  //  50 Hz
// acceleration scale
#define AFS_2       0b00000000  // +-2g
#define AFS_4       0b00001000  // +-4g
#define AFS_6       0b00010000  // +-6g
#define AFS_8       0b00011000  // +-8g
#define AFS_16      0b00100000  // +-16g
// enable acceleration self-test enable
#define AST         0b00000010
// use 3-wire SPI instead of 4-wire SPI
#define SIM         0b00000001


// CTRL4 Flags
// click generator interrupt on INT2
#define INT2_Click      0b10000000
// inertial interrupt generator 1 on INT2
#define INT2_IG1        0b01000000
// inertial interrupt generator 2 on INT2
#define INT2_IG2        0b00100000
// magnetic interrupt generator INT2
#define INT2_IGM        0b00010000
// accelerometer data-ready signal on INT2
#define INT2_DRDY_A     0b00001000
// magnetometer data-ready signal on INT2
#define INT2_DRDY_M     0b00000100
// FIFO overrun interrupt on INT2
#define INT2_Overrun    0b00000010
// FIFO threshold interrupt on INT2
#define INT2_FTH        0b00000001


// CTRL5 Flags
// enable temperature sensor
#define TEMP_EN         0b10000000
// magnetometer resolution
#define M_RES_HIGH      0b01100000
#define M_RES_LOW       0b00000000
// magnetometer data rate
#define M_ODR_3         0b00000000  //   3.125 Hz
#define M_ODR_6         0b00000100  //   6.25 Hz
#define M_ODR_12        0b00001000  //  12.5 Hz
#define M_ODR_25        0b00001100  //  25 Hz
#define M_ODR_50        0b00010000  //  50 Hz
#define M_ODR_100       0b00010100  // 100 Hz
// latch interrupt request on INT2_SRC register
#define LIR2            0b00000010
// latch interrupt request on INT1_SRC register
#define LIR1            0b00000001


// CTRL6 Flags
// magnetometer scale
#define MFS_2   0b00000000  //  +-2 gauss
#define MFS_4   0b00100000  //  +-4 gauss
#define MFS_8   0b01000000  //  +-8 gauss
#define MFS_12  0b01100000  // +-12 guass


// CTRL7
// acceleration high pass filter mode
#define AHPM_NORMAL_RESET   0b00000000  // normal mode (reset x, y, z)
#define AHPM_REFERENCE      0b01000000  // reference signal for filtering
#define AHPM_NORMAL         0b10000000  // normal mode
#define AHPM_AUTO_RESET     0b11000000  // auto reset on interrupt
// use internal filter for acceleration data
#define AFDS                0b00100000
// only use temperature sensor
#define T_ONLY              0b00010000
// magnetometer low power mode
#define MLP                 0b00000100
// magnetometer sensor mode
#define MD_CONT             0b00000000  // continuous conversion mode
#define MD_SINGLE           0b00000001  // single conversion mode
#define MD_POWER_DOWN       0b00000010  // power down mode
#define MD_POWER_DOWN0      0b00000010  // power down mode


// STATUS_A Flags (accelerometer)
// x, y, z axis data overrun has occurred.
#define ZYXAOR      0b10000000
// z-axis data overrun
#define ZAOR        0b01000000
// y-axis data overrun
#define YAOR        0b00100000
// x-axis data overrun
#define XAOR        0b00010000
// x, y, z-axis, new data available
#define ZYXADA      0b00001000
// z-axis new data available
#define ZADA        0b00000100
// y-axis new data available
#define YADA        0b00000010
// z-axis new data available
#define XADA        0b00000001


void imuInit(void){

    // Initialize buffer indices.
    imuAccIdx = 0;
    imuMagIdx = 0;

    // Set all chip select pins to outputs.
    IMU_SELECT_PIN_OUTPUT();
    PRS_SELECT_PIN_OUTPUT();
    SDC_SELECT_PIN_OUTPUT();
    DAC_SELECT_PIN_OUTPUT();
    EPM_SELECT_PIN_OUTPUT();

    // Deselect all SPI1 chips.
    IMU_DESELECT();
    PRS_DESELECT();
    SDC_DESELECT();
    DAC_DESELECT();
    EPM_DESELECT();

    // Setup SPI module 1 to talk to the IMU.
    spi1Init(
        SPI_VALID_2ND_EDGE |    // data capture on rising clock edge
        SPI_ENABLE |            // enable SPI2 module
        SPI_IDLE_HIGH |          // idle clock is low
        SPI_MASTER_FOSC_4,      // Fosc/4
        SPI_INT_DISABLE         // no interrupts
    );

    // Enable magnetometer interrupts.  Use push pull, and active high.
    IMU_WRITE(INT_CTRL_M, 
        spi1ExchangeByte(XMIEN | YMIEN | ZMIEN | PP | IEA_HIGH);
    );

    // Enable accelerometer at 50 Hz and block updates until reads.
    IMU_WRITE(CTRL1, spi1ExchangeByte(AODR_50 | BDU | AZEN | AYEN | AXEN););

    // Set accelerometer scale to +-2g.
    IMU_WRITE(CTRL2, spi1ExchangeByte(AFS_2););

    // Use INT2 pin as the data ready interrupt pin.
    IMU_WRITE(CTRL4, spi1ExchangeByte(INT2_DRDY_A | INT2_DRDY_M););

    // Set magnetometer to low resolution and data rate to 50 Hz.
    IMU_WRITE(CTRL5, spi1ExchangeByte(M_RES_LOW | M_ODR_50););

    // Set magnetometer to +-2 gauss.
    IMU_WRITE(CTRL6, spi1ExchangeByte(MFS_2););

    // Set magnetometer to continuous mode.
    IMU_WRITE(CTRL7, spi1ExchangeByte(MD_CONT););

    // Setup interrupt pin.
    IMU_INT_FLAG = 0;
    INT_PIN_INPUT();
    INT_PRIORITY_LOW();
    INT_RISING_EDGE();
    INT_ENABLE();
}


void imuSpinup(void){

    // One manual call is needed because the PIC triggers it's interrupt
    // on a rising edge.  If the IMU interrupt pin is tripped before
    // interrupts are enabled it will be stuck.
    imuISR();

    // Wait for 1 second while the buffers fill up.
    delayxs(1);
}


// Change temporary data section for library interrupts.
#pragma tmpdata imu_tmpdata
void imuISR(void){

    uint8_t byte;

    // Check for new magnetometer data.
    IMU_READ(STATUS_M, byte = spi1ExchangeByte_ISRL(0););
    if (byte & ZYXMDA){
        // Update index.
        if (imuMagIdx == IMU_BUFFER_LENGTH){
            imuMagIdx = 0;
        }
        // Read magnetometer data into globals.
        IMU_READ(OUT_X_L_M, 
            imuMagX[imuMagIdx].uint8A = spi1ExchangeByte_ISRL(0);
            imuMagX[imuMagIdx].uint8B = spi1ExchangeByte_ISRL(0);
            imuMagY[imuMagIdx].uint8A = spi1ExchangeByte_ISRL(0);
            imuMagY[imuMagIdx].uint8B = spi1ExchangeByte_ISRL(0);
            imuMagZ[imuMagIdx].uint8A = spi1ExchangeByte_ISRL(0);
            imuMagZ[imuMagIdx].uint8B = spi1ExchangeByte_ISRL(0);
        );
    }

    // Check for new accelerometer data.
    IMU_READ(STATUS_A, byte = spi1ExchangeByte_ISRL(0););
    if (byte & ZYXADA){
        // Update index.
        if (imuAccIdx == IMU_BUFFER_LENGTH){
            imuAccIdx = 0;
        }
        // Read accelerometer data.
        IMU_READ(OUT_X_L_A, 
            imuAccX[imuAccIdx].uint8A = spi1ExchangeByte_ISRL(0);
            imuAccX[imuAccIdx].uint8B = spi1ExchangeByte_ISRL(0);
            imuAccY[imuAccIdx].uint8A = spi1ExchangeByte_ISRL(0);
            imuAccY[imuAccIdx].uint8B = spi1ExchangeByte_ISRL(0);
            imuAccZ[imuAccIdx].uint8A = spi1ExchangeByte_ISRL(0);
            imuAccZ[imuAccIdx].uint8B = spi1ExchangeByte_ISRL(0);
        );
    }

    IMU_INT_FLAG = 0;
}
#pragma tmpdata
