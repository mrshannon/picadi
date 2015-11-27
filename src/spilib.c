////////////////////////////////////////////////////////////////////////
// File: spi.c
// Header: spi.h
// Author: Michael R. Shannon
// Written: Friday, November 13, 2015
// Updated: Friday, November 13, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include "stdint.h"
#include "spilib.h"


void spi1Init(uint8_t config, uint8_t interupt){

    // Turn off SPI1 for configuration.
    SPI1_ENABLE = 0;

    // Configure SPI1 module.
    TRISCbits.TRISC5 = 0;   // set SDO1 as output
    TRISCbits.TRISC4 = 1;   // set SDI1 as input
    TRISCbits.TRISC3 = 0;   // set SCK1 as output;
    // Configure sample location and valid edge.
    SSP1STAT = config & 0b11000000;
    // Turn on and configure clock polarity and speed.
    SSP1CON1 = config & 0b00111111;

    // Turn on SPI module if desired.
    SPI1_ENABLE = (config & 0b00100000) != 0;

    // Configure interrupts.
    SPI1_INT_PRIORITY = (interupt & 0b00000010) > 0;
    SPI1_INT_ENABLE = (interupt & 0b00000001) > 0;
}


void spi2Init(uint8_t config, uint8_t interupt){

    // Turn off SPI2 for configuration.
    SPI2_ENABLE = 0;

    // Configure SPI2 module.
    TRISDbits.TRISD4 = 0;   // set SDO2 as output
    TRISDbits.TRISD5 = 1;   // set SDI2 as input
    TRISDbits.TRISD6 = 0;   // set SCK2 as output;
    // Configure sample location and valid edge.
    SSP2STAT = config & 0b11000000;
    // Configure clock polarity and speed.
    SSP2CON1 = config & 0b00011111;

    // Turn on SPI module if desired.
    SPI2_ENABLE = (config & 0b00100000) != 0;

    // Configure interrupts.
    SPI2_INT_PRIORITY = (interupt & 0b00000010) > 0;
    SPI2_INT_ENABLE = (interupt & 0b00000001) > 0;
}


uint8_t spi1ExchangeByte(uint8_t out){

    uint8_t tmp;

    // Transmit byte over SPI.
    SPI1_BUFFER = out;

    // If this cause a write collision wait for previous exchange to
    // finish and throw away the received byte.
    if (SPI1_WRITE_COLLISION){
        while (SPI1_RECEIVE_DONE == 0); // wait for receive to finish
        SPI1_WRITE_COLLISION = 0;       // clear write collision bit
        tmp = SPI1_BUFFER;              // empty buffer
        SPI1_BUFFER = out;             // write byte to SPI
    }

    // Wait for transmit/receive operation to complete.
    while (SPI1_RECEIVE_DONE == 0);

    // Return received byte.  It is done this way in an attempt to keep
    // the compiler from optimizing out the read.
    tmp = SPI1_BUFFER;
    return tmp;
}


uint8_t spi2ExchangeByte(uint8_t out){

    uint8_t tmp;

    // Transmit byte over SPI.
    SPI2_BUFFER = out;

    // If this cause a write collision wait for previous exchange to
    // finish and throw away the received byte.
    if (SPI2_WRITE_COLLISION){
        while (SPI2_RECEIVE_DONE == 0); // wait for receive to finish
        SPI2_WRITE_COLLISION = 0;       // clear write collision bit
        tmp = SPI2_BUFFER;              // empty buffer
        SPI2_BUFFER = out;              // write byte to SPI
    }

    // Wait for transmit/receive operation to complete.
    while (SPI2_RECEIVE_DONE == 0);

    // Return received byte.  It is done this way in an attempt to keep
    // the compiler from optimizing out the read.
    tmp = SPI2_BUFFER;
    return SPI2_BUFFER;
}


uint8_t spi1ExchangeByte_(uint8_t out){

    uint8_t tmp;

    // Wait for transmit/receive operation to complete.
    while (SPI1_RECEIVE_DONE == 0);

    // Read received byte.
    tmp = SPI1_BUFFER;

    // Send byte to SPI.
    SPI1_BUFFER = out;

    // Return the received byte.
    return tmp;
}


uint8_t spi2ExchangeByte_(uint8_t out){

    uint8_t tmp;

    // Wait for transmit/receive operation to complete.
    while (SPI2_RECEIVE_DONE == 0);

    // Read received byte.
    tmp = SPI2_BUFFER;

    // Send byte using SPI.
    SPI2_BUFFER = out;

    // Return the received byte.
    return tmp;
}


////////////////////////////////////////////////////////////////////////
// NOTE: Many of the functions below are using default implementations
//       that are not very efficient because the C18 compiler cannot
//       inline functions.  Therefore, it may be desirable to write
//       specific implementations that can be more efficient.
////////////////////////////////////////////////////////////////////////


uint8_t spi1ReadByte(){
    return spi1ExchangeByte(0);
}


uint8_t spi2ReadByte(){
    return spi2ExchangeByte(0);
}


uint8_t spi1ReadByte_(){
    return spi1ExchangeByte_(0);
}


uint8_t spi2ReadByte_(){
    return spi2ExchangeByte_(0);
}


void spi1WriteByte(uint8_t out){
    spi1ExchangeByte(out);
}


void spi2WriteByte(uint8_t out){
    spi2ExchangeByte(out);
}


void spi1WriteByte_(uint8_t out){
    spi1ExchangeByte_(out);
}


void spi2WriteByte_(uint8_t out){
    spi2ExchangeByte_(out);
}


void spi1Exchange(uint8_t len, uint8_t *outPtr, uint8_t *inPtr){

    uint8_t i;

    // Both pointers are valid.
    if (outPtr && inPtr){
        for (i = 0; i < len; ++i){
            inPtr[i] = spi1ExchangeByte(outPtr[i]);
        }
    // inPtr == null
    } else if (outPtr){
        for (i = 0; i < len; ++i){
            spi1WriteByte(outPtr[i]);
        }
    // outPtr == null
    } else if (inPtr){
        for (i = 0; i < len; ++i){
            inPtr[i] = spi1ReadByte();
        }
    }
}


void spi2Exchange(uint8_t len, uint8_t *outPtr, uint8_t *inPtr){

    uint8_t i;

    // Both pointers are valid.
    if (outPtr && inPtr){
        for (i = 0; i < len; ++i){
            inPtr[i] = spi2ExchangeByte(outPtr[i]);
        }
    // inPtr == null
    } else if (outPtr){
        for (i = 0; i < len; ++i){
            spi2WriteByte(outPtr[i]);
        }
    // outPtr == null
    } else if (inPtr){
        for (i = 0; i < len; ++i){
            inPtr[i] = spi2ReadByte();
        }
    }
}
