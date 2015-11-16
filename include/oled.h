////////////////////////////////////////////////////////////////////////
// File: oled.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Friday, November 13, 2015
// Device: PIC18F87K22
// Fosc: 10 MHz
// Instruction Clock: Fosc/4
// Description:
//      This is a library for the SSD1306 OLED controller with a 128x64
//      pixel monochrome display.
//
//      Some of this library is a modification of the Adafuit_SSD1306
//      library written is C++ for the Arduino.
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"


#ifndef OLED_H
#define OLED_H


// NOTE: See page 25 of the SSD1306 datasheet for details of the OLED
//       memory layout.


#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_PAGE_WIDTH 128
#define OLED_PAGE_HEIGHT 8


// Frame buffer.
extern uint8_t oledBuffer_PAGE0[OLED_PAGE_WIDTH];
extern uint8_t oledBuffer_PAGE1[OLED_PAGE_WIDTH];
extern uint8_t oledBuffer_PAGE2[OLED_PAGE_WIDTH];
extern uint8_t oledBuffer_PAGE3[OLED_PAGE_WIDTH];
extern uint8_t oledBuffer_PAGE4[OLED_PAGE_WIDTH];
extern uint8_t oledBuffer_PAGE5[OLED_PAGE_WIDTH];
extern uint8_t oledBuffer_PAGE6[OLED_PAGE_WIDTH];
extern uint8_t oledBuffer_PAGE7[OLED_PAGE_WIDTH];


void oledInit(void);

void oledWriteBuffer(void);

void oledNormal(void);

void oledInverse(void);


#endif // OLED_H
