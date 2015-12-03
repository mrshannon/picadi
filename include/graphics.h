////////////////////////////////////////////////////////////////////////
// File: graphics.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Wednesday, December 02, 2015
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


#include "stdint.h"
#include "stdbool.h"


#ifndef GRAPHICS_H
#define GRAPHICS_H


#define GL_COLOR_WHITE 0
#define GL_COLOR_BLACK 1
#define GL_COLOR_INVERT 2
#define GL_COLOR_OVERWRITE 3


#define GL_FRAME_WIDTH 128
#define GL_FRAME_HEIGHT 64
#define GL_FRAME_SIZE (GL_FRAME_WIDTH * GL_FRAME_HEIGHT / 8)
#define GL_MIN_X 0
#define GL_MIN_Y 0
#define GL_MAX_X (GL_FRAME_WIDTH-1)
#define GL_MAX_Y (GL_FRAME_HEIGHT-1)


// Clip codes.
#define GL_CCT 0b00001000   // top
#define GL_CCB 0b00000100   // bottom
#define GL_CCR 0b00000010   // right
#define GL_CCL 0b00000001   // left


#define GL_NUM_CHARS 96
#define GL_CHAR_WIDTH 6
#define GL_CHAR_HEIGHT 8


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


bool glClipLine(int16_t *x0_ptr, int16_t *y0_ptr,
                int16_t *x1_ptr, int16_t *y1_ptr);


uint8_t glClipCode(int16_t x, int16_t y);


void glRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);


void glRectFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);


void glRectFill_(uint8_t x0, uint8_t y0,
                 uint8_t x1, uint8_t y1,
                 uint8_t color);


void glEllipse(int16_t xc, int16_t yc, uint8_t xr, uint8_t yr, uint8_t color);


void glCircle(int16_t xc, int16_t yc, uint8_t r, uint8_t color);


void glTriangle(int16_t x0, int16_t y0,
                int16_t x1, int16_t y1,
                int16_t x2, int16_t y2,
                uint8_t color);

void glTriangleFill(int16_t x0, int16_t y0,
                    int16_t x1, int16_t y1,
                    int16_t x2, int16_t y2,
                    uint8_t color);

void glTriangleFill_(int16_t x0, int16_t y0,
                     int16_t x1, int16_t y1,
                     int16_t xp, int16_t yp,
                     uint8_t color);

void glTriangleFillFVS(int16_t xs, int16_t ys0, int16_t ys1,
                       int16_t xp, int16_t yp,
                       uint8_t color);


void glChar(uint8_t line, uint8_t column, char ch, uint8_t color);


void glString(uint8_t line, uint8_t column, uint8_t color, const char *str);


#endif // GRAPHICS_H
