////////////////////////////////////////////////////////////////////////
// File: spilib.h
// Author: Michael R. Shannon
// Written: Friday, November 13, 2015
// Updated: Friday, December 11, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      SPI master mode library.  It does not currently support slave
//      mode.
//
////////////////////////////////////////////////////////////////////////


#ifndef SPILIB_H
#define SPILIB_H


// bit aliases for SPI1
#define SPI1_INT_ENABLE PIE1bits.SSP1IE
#define SPI1_INT_FLAG PIR1bits.SSP1IF
#define SPI1_INT_PRIORITY IPR1bits.SSP1IP
#define SPI1_BUFFER SSP1BUF
#define SPI1_RECEIVE_DONE SSP1STATbits.BF
#define SPI1_WRITE_COLLISION SSP1CON1bits.WCOL
#define SPI1_RECEIVE_OVERFLOW SSP1CON1bits.SSPOV
#define SPI1_ENABLE SSP1CON1bits.SSPEN


// Bit aliases for SPI2
#define SPI2_INT_ENABLE PIE2bits.SSP2IE
#define SPI2_INT_FLAG PIR2bits.SSP2IF
#define SPI2_INT_PRIORITY IPR2bits.SSP2IP
#define SPI2_BUFFER SSP2BUF
#define SPI2_RECEIVE_DONE SSP2STATbits.BF
#define SPI2_WRITE_COLLISION SSP2CON1bits.WCOL
#define SPI2_RECEIVE_OVERFLOW SSP2CON1bits.SSPOV
#define SPI2_ENABLE SSP2CON1bits.SSPEN


////////////////////////////////////////////////////////////////////////
// spixInit config "or" flags
////////////////////////////////////////////////////////////////////////

// Sample input at middle or end of output.
#define SPI_SAMPLE_MIDDLE       0b00000000  // sample at middle of frame
#define SPI_SAMPLE_END          0b10000000  // sample at end of frame

// Valid edge (uses non PIC style).
#define SPI_VALID_1ST_EDGE      0b01000000  // data is valid on 1st edge
#define SPI_VALID_2ND_EDGE      0b00000000  // data is valid on 2nd edge

// On/off.
#define SPI_ENABLE              0b00100000  // enable SPI
#define SPI_DISABLE             0b00000000  // disable SPI


// Idle select.
#define SPI_IDLE_LOW            0b00000000
#define SPI_IDLE_HIGH           0b00010000

// Frequency select
#define SPI_MASTER_FOSC_4       0b00000000  // Fosc/4
#define SPI_MASTER_FOSC_8       0b00001010  // Fosc/8
#define SPI_MASTER_FOSC_16      0b00000001  // Fosc/16
#define SPI_MASTER_FOSC_64      0b00000010  // Fosc/64
#define SPI_MASTER_FOSC_TMR2    0b00000011  // (Timer2 output)/2


////////////////////////////////////////////////////////////////////////
// spixInit interrupt "or" flags
////////////////////////////////////////////////////////////////////////

// SPI interrupt on/off
#define SPI_INT_ENABLE          0b00000001  // enable SPI interrupt
#define SPI_INT_DISABLE         0b00000000  // disable SPI interrupt


// Priority of SPI interrupt.
#define SPI_INT_HIGH            0b00000010  // high priority interrupt
#define SPI_INT_LOW             0b00000000  // low priority interrupt
////////////////////////////////////////////////////////////////////////


// Description:
//      Configure SPI module.
//
// Input:
//      uint8_t config:
//          Bitmask documented above.
//
//      uint8_t interrupt:
//          Bitmask documented above.
//
void spi1Init(uint8_t config, uint8_t interupt);
void spi2Init(uint8_t config, uint8_t interupt);


// Description:
//      Turn off the SPI module.
//
#define spi1Close() SPI1_ENABLE = 0
#define spi2Close() SPI2_ENABLE = 0


// Description:
//      Perform SPI byte exchange.  Transmits the given byte and returns
//      the byte it receives at the same time.
//
// Input:
//      uint8_t out:
//          Byte to send out.
//
// Output (uint8_t):
//      Byte received during exchange.
//
uint8_t spi1ExchangeByte(uint8_t out);
uint8_t spi2ExchangeByte(uint8_t out);
// same as above but use seperate tmpdata sections so they are safe to
// use inside of interupts.
uint8_t spi1ExchangeByte_ISRH(uint8_t out);
uint8_t spi1ExchangeByte_ISRL(uint8_t out);


// Description:
//      Write a byte string to the SPI module and store the received
//      bytes.
//
// Input:
//      uint8_t len:
//          Length of <in> and <out> buffers.
//
//      uint8_t *outPtr:
//          Pointer to buffer to transmit bytes from.  Use null if every
//          output byte should be 0.
//
//      uint8_t *inPtr
//          Pointer to buffer to write received bytes into.  Use null to
//          throw away received bytes.
//
//          NOTE: It is safe for this to point to the same location as
//                <outPtr>.  This effectively swaps the byte to transmit
//                with the received byte.
//
void spi1Exchange(uint8_t len, uint8_t *outPtr, uint8_t *inPtr);
void spi2Exchange(uint8_t len, uint8_t *outPtr, uint8_t *inPtr);


#endif // SPILIB_H
