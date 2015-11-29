////////////////////////////////////////////////////////////////////////
// File: graphics.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Friday, November 13, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This a graphics library designed to work with a 128x64 column
//      major frame buffer of the global name frameBuffer.  The first
//      address is the top left and the last address is the bottom
//      right.
//
//      While the origin of the frame buffer is the upper left corner
//      the GL function treat the lower left corner as the origin and
//      all coordinates are in pixels.
//
////////////////////////////////////////////////////////////////////////


#ifndef GRAPHICS_H
#define GRAPHICS_H


#define GL_COLOR_WHITE 0
#define GL_COLOR_BLACK 1
#define GL_COLOR_INVERT 2


#define GL_FRAME_WIDTH 128
#define GL_FRAME_HEIGHT 64
#define GL_FRAME_SIZE (GL_FRAME_WIDTH * GL_FRAME_HEIGHT / 8)


// Description:
//      Set frame buffer to all white.
void glSet(void);


// Description:
//      Set frame buffer to all black.
//
void glClear(void);


// Description:
//      Flip all the bits of the frame buffer.
//
void glInvert(void);


// Description:
//      Draws a single pixel point of the given color at the given
//      location.
//
// Input:
//      iint16_t x:
//          Horizontal pixel coordinate of point.
//
//      int16_t y:
//          Vertical pixel coordinate of point.
//
//      uint8_t color:
//          Color of point (defined only).
//
void glPoint(int16_t x, int16_t y, uint8_t color);


// Description:
//      Draws a vertical at a given location and a given color.
//
//      NOTE: This is the most efficient line.
//
// Input:
//      uint8_t x:
//          Horizontal pixel coordinate of line.
//
//      uint8_t y0:
//          Vertical pixel coordinate of start of line.
//
//      uint8_t y1:
//          Vertical pixel coordinate of point.
//
//      uint8_t color:
//          Color of line (defined only).
//
void glVLine(int16_t x, int16_t y0, int16_t y1, uint8_t color);


void glVLine_(uint8_t x, uint8_t y0, uint8_t y1, uint8_t color);


void glHLine(int16_t x0, int16_t x1, int16_t y, uint8_t color);


void glHLine_(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color);


void glLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);


void glRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);


void glRectFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);


void glRectFill_(uint8_t x0, uint8_t y0,
                 uint8_t x1, uint8_t y1,
                 uint8_t color);


void glCircle(uint8_t xc, uint8_t r, uint8_t color);


void glTriangleFBFill(uint8_t x0, uint8_t x1, uint8_t yb,
                      uint8_t xp, uint8_t yp, uint8_t color);


void glRotate(uint8_t *xPtr, uint8_t *yPtr, float theta);


void glChar8x6LP(char ch);


void glString8x6LP(uint8_t line, uint8_t x, char *str);


#endif // GRAPHICS_H
