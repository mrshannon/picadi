////////////////////////////////////////////////////////////////////////
// File: oled.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Friday, November 13, 2015
// Device: PIC18F87K22
// Compiler: C18
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
#define OLED_SIZE (OLED_WIDTH * OLED_HEIGHT / 8)


// Description:
//      Initilize SPI2 and OLED display.
//
void oledInit(void);


// Description:
//      Write the frame buffer to the OLED display.
//
void oledWriteBuffer(void);


// Description:
//      Switch display to normal mode.
//
void oledNormal(void);


// Description;
//      Switch display to inverted mode.
//
void oledInverse(void);


#endif // OLED_H
