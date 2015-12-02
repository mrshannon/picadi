////////////////////////////////////////////////////////////////////////
// File: lcd.h
// Author: Michael R. Shannon
// Written: Tuesday, November 03, 2015
// Updated: Monday, November 30, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This is a library for the ST7066U LCD controller on the ASEN
//      5519 development board.  It treats the LCD screen as a dual
//      line display with 8 valid characters locations each.
//
////////////////////////////////////////////////////////////////////////

#include "stdint.h"

#ifndef LCD_H
#define LCD_H


// Description:
//      Writes the initialization string to the LCD controller.
//
void lcdInit(void);


// Description:
//      Writes the given string the given line of the LCD.  String must
//      be in RAM.
//
// Inputs:
//      uint8_t line:
//          Line number of LCD to write to.
//
//      uint8_t column:
//          Column to start writing to LCD on.
//
//      const char *str:
//          String to write into LCD.  Must be in RAM.  String can be
//          longer than remainder of LCD but it will not be used.
//          String must be null terminated if shorter than remainder of
//          LCD line.
//
void lcdWrite(uint8_t line, uint8_t column, const char *str);


// Description:
//      Writes the given string the given line of the LCD.  String must
//      be in ROM.
//
// Inputs:
//      uint8_t line:
//          Line number of LCD to write to.
//
//      uint8_t column:
//          Column to start writing to LCD on.
//
//      const char *str:
//          String to write into LCD.  Must be in ROM.  String can be
//          longer than remainder of LCD but it will not be used.
//          String must be null terminated if shorter than remainder of
//          LCD line.
//
void lcdWriteROM(uint8_t line, uint8_t column, const rom char *str);


// Description:
//      Sets the address of the LCD cursor and resets the LCD to data
//      mode.
//
//      NOTE: Invalid line or column
//
// Inputs:
//      uint8_t line:
//          Line number of LCD address.
//
//      uint8_t column:
//          Column of lcd address.
//
void lcdSetAddress(uint8_t line, uint8_t column);


// Description:
//      Clears LCD display.
//
void lcdClear(void);


// Description:
//      Writes a byte to the LCD.  User is responsible for code data
//      mode.
//
// Input:
//      byte:
//          An 8 bit literal or variable to write to the LCD.
//
void lcdWriteByte(uint8_t byte);


#endif // LCD_H
