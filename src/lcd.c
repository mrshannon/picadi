///////////////////////////////////////////////////////////////////////
// File: lcd.c
// Header: lcd.h
// Author: Michael R. Shannon
// Written: Tuesday, November 03, 2015
// Updated: Monday, November 30, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <delays.h>
#include <string.h>
#include "util.h"
#include "config.h"
#include "lcd.h"


// LCD control bits.
#define LCD_RS LATHbits.LATH1           // register select bit
#define LCD_RS_TRIS TRISHbits.TRISH1    // and I/O setup
#define LCD_SET_CODE() LCD_RS = 0       // select instruction register
#define LCD_SET_DATA() LCD_RS = 1       // select data register
#define LCD_E LATHbits.LATH2            // enable bit
#define LCD_E_TRIS TRISHbits.TRISH2      // and I/O setup
#define LCD_PRE_WRITE() LCD_E = 1
#define LCD_POST_WRITE() LCD_E = 0


// LCD data bits.
#define LCD_DATA LATJ       // data register for LCD (only high 4 bits used)
#define LCD_DATA_TRIS TRISJ // control register for LCD data
// Macro to setup data bits as outputs.
#define LCD_SET_DATA_TRIS() LCD_DATA_TRIS &= 0b00001111


// LCD initialization string.
static const rom far char LCD_INIT_STR[] = "\x33\x32\x28\x01\x0C\x06\x00";


// Initialize LCD controller.
void lcdInit(void){

    uint8_t ii = 0;
    char c;

    LCD_SET_CODE();
        LCD_POST_WRITE();

    // Setup LCD pins as outputs.
    LCD_RS_TRIS = 0;
    LCD_E_TRIS = 0;
    LCD_SET_DATA_TRIS();

    // Delay for 0.1 seconds.
    delayxms(100);

    // Select instruction register of LCD.
    LCD_SET_CODE();

    // Write each byte of initialization string.
    while (LCD_INIT_STR[ii]) {

        // Write upper nibble of byte.
        LCD_PRE_WRITE();
        LCD_DATA = LCD_INIT_STR[ii];
        LCD_POST_WRITE();
        delayxms(10);


        // Write lower nibble of byte.
        LCD_PRE_WRITE();
        LCD_DATA = LCD_INIT_STR[ii] << 4;
        LCD_POST_WRITE();
        delayxms(10);

        // Increment initialization string index.
        ++ii;
    }

    // Clear screen.
    lcdClear();
}


// Writes up to 8 characters from RAM string to LCD at given location.
void lcdWrite(uint8_t line, uint8_t column, const char *str){

    uint8_t ii;
    uint8_t max_chars;

    // Return immediately if location is invalid.
    if (line >= 2U){
        return;
    }
    if (column >= 8U){
        return;
    }

    // Calculate maximum number of characters to write.
    max_chars = 8 - column;

    // Set the LCD address.
    lcdSetAddress(line, column);

    // Write characters to the LCD.
    for (ii = 0; (ii < max_chars) && (str[ii] != (char)'\0'); ++ii){
        lcdWriteByte(str[ii]);
    }
}


// Writes up to 8 characters from ROM string to LCD at given location.
void lcdWriteROM(uint8_t line, uint8_t column, const rom char *str){

    char buffer[9];

    // Copy ROM str to RAM buffer.
    strncpypgm2ram(buffer, (const rom far char *)str, 8U);

    // Make sure buffer is null terminated.
    buffer[8] = '\0';

    // Call lcdWrite to do the actual writing.
    lcdWrite(line, column, buffer);
}


// Set address of LCD cursor.
void lcdSetAddress(uint8_t line, uint8_t column){

    // Limit address to the screen.
    if (column >= 8U){
        column = 7;
    }

    // Set the address.
    LCD_SET_CODE();
    lcdWriteByte((line > 0U ? 0xC0 : 0x80) + column);
    LCD_SET_DATA();
}


// Clear both lines of LCD.
void lcdClear(){
    uint8_t ii;

    // Clear 1st line.
    lcdSetAddress(0, 0);
    for (ii = 0; ii < 8U; ++ii){
        lcdWriteByte(' ');
    }

    // Clear 2nd line.
    lcdSetAddress(1, 0);
    for (ii = 0; ii < 8U; ++ii){
        lcdWriteByte(' ');
    }
}


// Write single byte to LCD.
void lcdWriteByte(uint8_t byte){

    // Write high nibble.
    LCD_PRE_WRITE();
    LCD_DATA = (uint8_t)(byte);
    LCD_POST_WRITE();

    // Write low nibble.
    LCD_PRE_WRITE();
    LCD_DATA = (uint8_t)(byte) << 4;
    LCD_POST_WRITE();

    // Add 40 us delay so we don't write to the LCD to fast.
    Delay10TCYx(10*PLLMUL);
}
