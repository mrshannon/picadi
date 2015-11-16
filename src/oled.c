///////////////////////////////////////////////////////////////////////
// File: oled.c
// Header: oled.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Sunday, November 15, 2015
// Device: PIC18F87K22
// Fosc: 10 MHz
// Instruction Clock: Fosc/4
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include "util.h"
#include "stdint.h"
#include "spilib.h"
#include "oled.h"


// Control bits for used pins.
#define RESET_TRIS TRISFbits.TRISF2
#define SELECT_TRIS TRISFbits.TRISF5
#define DC_TRIS TRISFbits.TRISF6


// OLED pin set/unset macros.
#define OLED_RESET_LOW() LATFbits.LATF2 = 0
#define OLED_RESET_HIGH() LATFbits.LATF2 = 1
#define OLED_SELECT_COMMAND() LATFbits.LATF6 = 0
#define OLED_SELECT_DATA() LATFbits.LATF6 = 1
#define OLED_SELECT() LATFbits.LATF5 = 0
#define OLED_DESELECT() LATFbits.LATF5 = 1


// Decorator for selection of the OLED display's chip-select.
#define OLED(code) do { \
        OLED_SELECT(); \
        code \
        OLED_DESELECT(); \
    } while (0)


// Decorator for selection of OLED chip-select and command bits.
#define OLED_COMMAND(code) do { \
        OLED_SELECT(); \
        OLED_SELECT_COMMAND(); \
        code \
        OLED_DESELECT(); \
    } while (0)


// Decorator for selection of OLED chip-select and data bits.
#define OLED_DATA(code) do { \
        OLED_SELECT(); \
        OLED_SELECT_DATA(); \
        code \
        OLED_DESELECT(); \
    } while (0)


// Description:
//      Send a command byte.
//
// Input:
//      uint8_t byte:
//          Byte to write as a command.
//
#define oledSendCommand(byte) OLED_COMMAND(spi2ExchangeByte(byte);)


// Description:
//      Send a data byte.
//
//      Don't use this function to write a lot of data as it has at
//      least 3 instructions of overhead beyond the spi2ExchangeByte
//      instruction.  If speed is really important then you should use
//      the SPI macros and not the high level functions.
//
// Input:
//      uint8_t byte:
//          Byte to write as data.
//
#define oledSendData(byte) OLED_DATA(spi2ExchangeByte(byte);)

////////////////////////////////////////////////////////////////////////
// NOTE: Below are some low level OLED commands, see Table 9-1 of the
//       SSD1306 OLED controller datasheet for details.
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Fundamental OLED Commands (low level)
////////////////////////////////////////////////////////////////////////

// Set contrast.
#define OLED_CONTRAST(contrast) OLED_COMMAND( \
        spi2ExchangeByte(0x81); \
        spi2ExchangeByte(contrast); \
    )

// Reset/resume display.
#define OLED_DISPLAY_RESUME() oledSendCommand(0xA4)
#define OLED_DISPLAY_RESET() oledSendCommand(0xA5)

// Normal/inverse mode.
#define OLED_DISPLAY_NORMAL() oledSendCommand(0xA6)
#define OLED_DISPLAY_INVERSE() oledSendCommand(0xA7)

// Display on/off.
#define OLED_DISPLAY_OFF() oledSendCommand(0xAE)
#define OLED_DISPLAY_ON() oledSendCommand(0xAF)

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Adressing OLED Commands (low level)
////////////////////////////////////////////////////////////////////////

// Only horizontal addressing mode commands are implemented here as the
// frame buffer is setup to be addressed in this way.

// Set addressing mode.
#define OLED_HORIZONTAL_MODE() OLED_COMMAND( \
        spi2ExchangeByte(0x20); \
        spi2ExchangeByte(0b00); \
    )
#define OLED_VERTICAL_MODE() OLED_COMMAND( \
        spi2ExchangeByte(0x20); \
        spi2ExchangeByte(0b01); \
    )
#define OLED_PAGE_MODE() OLED_COMMAND( \
        spi2ExchangeByte(0x20); \
        spi2ExchangeByte(0b10); \
    )

// Set column range (horizontal or vertical addressing mode).
#define OLED_COLUMN_RANGE(start, end) OLED_COMMAND( \
        spi2ExchangeByte(0x21); \
        spi2ExchangeByte(start); \
        spi2ExchangeByte(end); \
    )

// Set page range (horizontal or verical addressing mode).
#define OLED_PAGE_RANGE(start, end) OLED_COMMAND( \
        spi2ExchangeByte(0x22); \
        spi2ExchangeByte(start); \
        spi2ExchangeByte(end); \
    )

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Hardware Configuration OLED Commands (low level)
////////////////////////////////////////////////////////////////////////

// Set display RAM start line (0-63).
#define OLED_START_LINE(line) oledSendCommand(0b01000000 | (line & 0b00111111))

// Use normal/reverse column mapping.
#define OLED_NORMAL_COLUMN() oledSendCommand(0xA0)
#define OLED_REVERSE_COLUMN() oledSendCommand(0xA1)

// Set the multiplex ratio to (16 <= mux <= 64).
#define OLED_MULTIPLEX(mux) OLED_COMMAND( \
        spi2ExchangeByte(0xA8); \
        spi2ExchangeByte(mux-1); \
    )

// Set normal/reverse row mapping.
#define OLED_NORMAL_ROW() oledSendCommand(0xC0)
#define OLED_REVERSE_ROW() oledSendCommand(0xC8)

// Set verticel offset from 0 to 63.
#define OLED_VERTICAL_OFFSET(offset) OLED_COMMAND( \
        spi2ExchangeByte(0xD3); \
        spi2ExchangeByte(offset); \
    )

// Configure COM pins.
#define OLED_ENABLE_ROW_REMAP 0b00100010
#define OLED_DISABLE_ROW_REMAP 0b00000010
#define OLED_SEQ_COM 0b00000010
#define OLED_ALT_COM 0b00010010
#define OLED_PIN_CONFIG(config) OLED_COMMAND( \
        spi2ExchangeByte(0xDA); \
        spi2ExchangeByte(config); \
    )\

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Timing and Driving Scheme OLED Commands (low level)
////////////////////////////////////////////////////////////////////////

// Set clock frequency (0-15) and display clock divisor (1-16).
#define OLED_CLOCK(freq, div) OLED_COMMAND( \
        spi2ExchangeByte(0xD5); \
        spi2ExchangeByte(((uint8_t)freq) << 4 | ((uint8_t)div-1)); \
    )

// Set phase 1 and phase 2 pre-charge periods in 1 to 15 display clock
// ticks.
#define OLED_PRECHARGE_PERIOD(phase1, phase2) OLED_COMMAND( \
        spi2ExchangeByte(0xD9); \
        spi2ExchangeByte(((uint8_t)phase2) << 4 | ((uint8_t)phase1)); \
    )

// Choose 0.65*Vcc as Vcomh deselect.
#define OLED_VCOMH_064VCC() OLED_COMMAND( \
        spi2ExchangeByte(0xDB); \
        spi2ExchangeByte(0x00); \
    )
// Choose 0.77*Vcc as Vcomh deselect (default).
#define OLED_VCOMH_077VCC() OLED_COMMAND( \
        spi2ExchangeByte(0xDB); \
        spi2ExchangeByte(0x20); \
    )
// Choose 0.83*Vcc as Vcomh deselect (default).
#define OLED_VCOMH_083VCC() OLED_COMMAND( \
        spi2ExchangeByte(0xDB); \
        spi2ExchangeByte(0x30); \
    )

// No operation.
#define OLED_NOP() oledSendCommand(0xE3)

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Charge Pump OLED Commands (low level)
////////////////////////////////////////////////////////////////////////

// Enable/disable 7.5 V charge pump.
#define OLED_CHARGE_PUMP_ENABLE() OLED_COMMAND( \
        spi2ExchangeByte(0x8D); \
        spi2ExchangeByte(0x14); \
    )
#define OLED_CHARGE_PUMP_DISABLE() OLED_COMMAND( \
        spi2ExchangeByte(0x8D); \
        spi2ExchangeByte(0x14); \
    )

////////////////////////////////////////////////////////////////////////


// Description:
//      Setup communication with the OLED controller via SPI using SPI2.
//
static void __oledInitCom(void){


    // Initialize SPI2 for communication with OLED display.
    spi2Init(
        SPI_VALID_1ST_EDGE |    // data capture on rising clock edge
        SPI_ENABLE |            // enable SPI2 module
        SPI_IDLE_LOW |          // idle clock is low
        SPI_MASTER_FOSC_4,      // Fosc/4
        SPI_INT_DISABLE         // no interrupts
    );

    // Setup non SPI pins as outputs.
    RESET_TRIS = 0;
    SELECT_TRIS = 0;
    DC_TRIS = 0;
}


// Description:
//      Send intilization/configuration bytes to the OLED controller.
//
static void __oledWriteInit(void){


    // Initialize the OLED display.
    OLED_RESET_HIGH();  // tie reset pin high
    delay1ms();         // wait for Vdd to become stable
    OLED_RESET_LOW();   // tie reset pin low
    delayxms(10);       // wait for reset
    OLED_RESET_HIGH();  // tie reset pin high

    // Send initialization commands (most of these are taken from the
    // Adafruit SSD1306 library).
    OLED_DISPLAY_OFF();         // turn off display
    OLED_CLOCK(8, 1);           // Fosc = speed 8, display clock = Fosc
    OLED_MULTIPLEX(64);         // MUX = 32
    OLED_VERTICAL_OFFSET(0);    // no vertical offset
    OLED_START_LINE(0);         // start at line 0 of RAM
    OLED_CHARGE_PUMP_ENABLE();  // enable 7.5V charge pump
    OLED_HORIZONTAL_MODE();     // use horizontal addressing mode
    OLED_REVERSE_COLUMN();      // don't reverse column addressing
    OLED_REVERSE_ROW();         // reverse row addressing
    // Configure COM pins.
    OLED_PIN_CONFIG(OLED_DISABLE_ROW_REMAP | OLED_ALT_COM);
    OLED_CONTRAST(127);         // set contrast to 0.5
    // Period 1 = 15 DCLK, period 2 = 1 DCLK.
    OLED_PRECHARGE_PERIOD(1, 15);
    OLED_VCOMH_077VCC();        // Vcomh deselect = 0.77 * Vcc
    OLED_DISPLAY_RESUME();      // resume display
    OLED_DISPLAY_NORMAL();      // normal display mode
    OLED_DISPLAY_ON();          // turn on OLED panel
}


void oledInit(void){

    // Initialize SPI communication.
    __oledInitCom();

    // Write initialization string to OLED.
    __oledWriteInit();
}


void oledWriteBuffer(void){

    uint8_t i;

    // Write frame buffer to OLED RAM.
    OLED_DATA(

        // Write page 0.
        for (i = 0; i < 128; ++i){
            // NOTE: This uses the SPI functions.  If speed becomes a
            //       problem then manually write to the SPI buffer.
            spi2ExchangeByte(oledBuffer_PAGE0[i]);
        }

        // Write page 1.
        for (i = 0; i < 128; ++i){
            spi2ExchangeByte(oledBuffer_PAGE1[i]);
        }

        // Write page 2.
        for (i = 0; i < 128; ++i){
            spi2ExchangeByte(oledBuffer_PAGE2[i]);
        }

        // Write page 3.
        for (i = 0; i < 128; ++i){
            spi2ExchangeByte(oledBuffer_PAGE3[i]);
        }

        // Write page 4.
        for (i = 0; i < 128; ++i){
            spi2ExchangeByte(oledBuffer_PAGE4[i]);
        }

        // Write page 5.
        for (i = 0; i < 128; ++i){
            spi2ExchangeByte(oledBuffer_PAGE5[i]);
        }

        // Write page 6.
        for (i = 0; i < 128; ++i){
            spi2ExchangeByte(oledBuffer_PAGE6[i]);
        }

        // Write page 7.
        for (i = 0; i < 128; ++i){
            spi2ExchangeByte(oledBuffer_PAGE7[i]);
        }
    );
}


void oledNormal(void){
    OLED_DISPLAY_NORMAL();
}


void oledInverse(void){
    OLED_DISPLAY_INVERSE();
}


// Include initialize frame buffer.
#include "oled_picadi.h"
