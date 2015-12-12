////////////////////////////////////////////////////////////////////////
// File: graphics.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Friday, December 11, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This a graphics library designed to work with a 128x64 column
//      major frame buffer of the global name frameBuffer.  The first
//      address is the top left and the last address is the bottom
//      right.
//
//      While the origin of the frame buffer is the upper left corner
//      the GL function treats the lower left corner as the origin and
//      all coordinates are in pixels.
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"
#include "stdbool.h"


#ifndef GRAPHICS_H
#define GRAPHICS_H


// Graphics library colors.
#define GL_COLOR_WHITE 0        // white
#define GL_COLOR_BLACK 1        // black
#define GL_COLOR_INVERT 2       // invert color
#define GL_COLOR_OVERWRITE 3    // overwrite background, only for text


// Frame buffer size.
#define GL_FRAME_WIDTH 128
#define GL_FRAME_HEIGHT 64
#define GL_FRAME_SIZE (GL_FRAME_WIDTH * GL_FRAME_HEIGHT / 8) // in bytes


// Minimum and maximum frame buffer.
#define GL_MIN_X 0
#define GL_MIN_Y 0
#define GL_MAX_X (GL_FRAME_WIDTH-1)
#define GL_MAX_Y (GL_FRAME_HEIGHT-1)


// Clip codes.
#define GL_CCT 0b00001000   // top
#define GL_CCB 0b00000100   // bottom
#define GL_CCR 0b00000010   // right
#define GL_CCL 0b00000001   // left


// ASCII character constants.
#define GL_NUM_CHARS 96     // characters in ASCII table
#define GL_CHAR_WIDTH 6     // pixel width of each character
#define GL_CHAR_HEIGHT 8    // pixel height of each character


// Description:
//      Set frame buffer to all white.
//
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
//      int16_t x:
//          Horizontal pixel coordinate of point.
//
//      int16_t y:
//          Vertical pixel coordinate of point.
//
//      uint8_t color:
//          Color (defined only).
//
void glPoint(int16_t x, int16_t y, uint8_t color);


// Description:
//      Draws a vertical line at a given location and a given color.
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
//          Vertical pixel coordinate of end of line.
//
//      uint8_t color:
//          Color (defined only).
//
void glVLine(int16_t x, int16_t y0, int16_t y1, uint8_t color);
// Same as glVLine but requires y0 < y1 and all coordinates be one the
// frameBuffer.
void glVLine_(uint8_t x, uint8_t y0, uint8_t y1, uint8_t color);


// Description:
//      Draws a horizontal line at a given location and a given color.
//
//      NOTE: This is more efficient than generic lines but not as
//            efficient as glVline.
//
// Input:
//      uint8_t x0:
//          Horizontal pixel coordinate of start of line.
//
//      uint8_t x1:
//          Horizontal pixel coordinate of end of line.
//
//      uint8_t y:
//          Vertical pixel coordinate of line.
//
//      uint8_t color:
//          Color (defined only).
//
void glHLine(int16_t x0, int16_t x1, int16_t y, uint8_t color);
// Same as glHLine but requires x0 < x1 and all coordinates be one the
// frameBuffer.
void glHLine_(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color);


// Description:
//      Draws an arbitrary 1-pixel wide line from point 0 to point 1.
//
// Input:
//      int16_t x0:
//          Horizontal coordinate of start point.
//
//      int16_t y0:
//          Vertical coordinate of start point.
//
//      int16_t x1:
//          Horizontal coordinate of end point.
//
//      int16_t y1:
//          Vertical coordinate of end point.
//
//      uint8_t color:
//          Color (defined only).
//
void glLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);


// Description:
//      Clip line to the frame buffer using the Cohen-Sutherland method.
//      Coordinates are adjusted in place.
//
//      Based on implementation at:
//      https://en.wikipedia.org/wiki/Cohen–Sutherland
//
// Input:
//      int16_t *x0_ptr:
//          Pointer to horizontal coordinate of start point.
//
//      int16_t *y0_ptr:
//          Pointer to vertical coordinate of start point.
//
//      int16_t *x1_ptr:
//          Pointer to horizontal coordinate of end point.
//
//      int16_t *y1_ptr:
//          Pointer to vertical coordinate of end point.
//
bool glClipLine(int16_t *x0_ptr, int16_t *y0_ptr,
                int16_t *x1_ptr, int16_t *y1_ptr);


// Description:
//      Calculate Cohen-Sutherland clip code for a given point.
//
// Input:
//      int16_t x:
//          Horizontal coordinate of point.
//
//      int16_t y:
//          Vertical coordinate of point.
//
// Output (uint8_t):
//      Cohen-Sutherland clip code.
//
uint8_t glClipCode(int16_t x, int16_t y);


// Description:
//      Draw a non-filled rectangle to the frame buffer.
//
// Input:
//      int16_t x0:
//          Horizontal coordinate of corner 0.
//
//      int16_t y0:
//          Vertical coordinate of corner 0.
//
//      int16_t x1:
//          Horizontal coordinate of corner 1.
//
//      int16_t y1:
//          Vertical coordinate of corner 1.
//
//      uint8_t color:
//          Color (defined only).
//
void glRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);


// Description:
//      Draw a filled rectangle to the frame buffer.
//
// Input:
//      int16_t x0:
//          Horizontal coordinate of corner 0.
//
//      int16_t y0:
//          Vertical coordinate of corner 0.
//
//      int16_t x1:
//          Horizontal coordinate of corner 1.
//
//      int16_t y1:
//          Vertical coordinate of corner 1.
//
//      uint8_t color:
//          Color (defined only).
//
void glRectFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
// Same as glRectFill but all coordinates must be on the frame buffer,
// corner 0 must be the lower left, and corner 1 must be the top right.
void glRectFill_(uint8_t x0, uint8_t y0,
                 uint8_t x1, uint8_t y1,
                 uint8_t color);


// Description:
//      Draw a non-filled ellipse to the frame buffer using the
//      Bresenham algorithm.
//
//      Algorithm:
//      http://members.chello.at/~easyfilter/bresenham.html
//
// Input:
//      int16_t xc:
//          Horizontal coordinate of center.
//
//      int16_t yc:
//          Vertical coordinate of center.
//
//      uint8_t xr:
//          Horizontal radius in pixels.
//
//      uint8_t yr:
//          Vertical radius in pixels.
//
//      uint8_t color:
//          Color (defined only).
//
void glEllipse(int16_t xc, int16_t yc, uint8_t xr, uint8_t yr, uint8_t color);


// Description:
//      Draw a non-filled circle to the frame buffer using the
//      Bresenham algorithm.
//
//      Algorithm:
//      http://members.chello.at/~easyfilter/bresenham.html
//
// Input:
//      int16_t xc:
//          Horizontal coordinate of center.
//
//      int16_t yc:
//          Vertical coordinate of center.
//
//      uint8_t r:
//          Radius in pixels.
//
//      uint8_t color:
//          Color (defined only).
//
void glCircle(int16_t xc, int16_t yc, uint8_t r, uint8_t color);


// Description:
//      Draw a non-filled triangle to the frame buffer.
//
// Input:
//      int16_t x0:
//          Horizontal coordinate of point 0.
//
//      int16_t y0:
//          Vertical coordinate of point 0.
//
//      int16_t x1:
//          Horizontal coordinate of point 1.
//
//      int16_t y1:
//          Vertical coordinate of point 1.
//
//      int16_t x2:
//          Horizontal coordinate of point 2.
//
//      int16_t y2:
//          Vertical coordinate of point 2.
//
//      uint8_t color:
//          Color (defined only).
//
void glTriangle(int16_t x0, int16_t y0,
                int16_t x1, int16_t y1,
                int16_t x2, int16_t y2,
                uint8_t color);


// Description:
//      Draw a filled triangle to the frame buffer.
//
//      Triangle rasterization algorithm from:
//      http://www.sunshine2k.de/coding/java/TriangleRasterization/
//          TriangleRasterization.html#algo3
//
// Input:
//      int16_t x0:
//          Horizontal coordinate of point 0.
//
//      int16_t y0:
//          Vertical coordinate of point 0.
//
//      int16_t x1:
//          Horizontal coordinate of point 1.
//
//      int16_t y1:
//          Vertical coordinate of point 1.
//
//      int16_t x2:
//          Horizontal coordinate of point 2.
//
//      int16_t y2:
//          Vertical coordinate of point 2.
//
//      uint8_t color:
//          Color (defined only).
//
void glTriangleFill(int16_t x0, int16_t y0,
                    int16_t x1, int16_t y1,
                    int16_t x2, int16_t y2,
                    uint8_t color);
// Same as glTriangleFill but cannot be used when a side of triangle is
// vertical.  It also requires that the middle point (horizontally be
// given last).
void glTriangleFill_(int16_t x0, int16_t y0,
                     int16_t x1, int16_t y1,
                     int16_t xm, int16_t ym,
                     uint8_t color);


// Description:
//      Draw a flat sided filled triangle to the frame buffer.
//
//      Triangle rasterization algorithm from:
//      http://www.sunshine2k.de/coding/java/TriangleRasterization/
//          TriangleRasterization.html#algo3
//
// Input:
//      int16_t xa:
//          Horizontal coordinate of vertical side.
//
//      int16_t ys0:
//          Vertical coordinate of start of vertical side.
//
//      int16_t ys1:
//          Vertical coordinate of end of vertical side.
//
//      int16_t xp:
//          Horizontal coordinate of peak.
//
//      int16_t xp:
//          Vertical coordinate of peak.
//
//      uint8_t color:
//          Color (defined only).
//
void glTriangleFillFVS(int16_t xs, int16_t ys0, int16_t ys1,
                       int16_t xp, int16_t yp,
                       uint8_t color);


// Description:
//      Write single ASCII character to frame buffer.  At given line and
//      pixel offset.
//
//      NOTE: Characters are 6x8 pixels.
//
// Input:
//      uint8_t line:
//          Line (0 to 7) to draw character at.
//
//      uint8_t column:
//          Column (per pixel -> 0 to 127) to draw character at.
//
//      char ch:
//          ASCII code of the character to write.
//
//      uint8_t color:
//          Color (defined only).
//
void glChar(uint8_t line, uint8_t column, char ch, uint8_t color);


// Description:
//      Write a null terminated RAM char string to the frame buffer.
//      Strings will wrap between lines.  Strings also support the '\n'
//      and '\r' control characters.  The first shifts down a line and
//      the second rests the cursor to the beginning of the line.
//
//      NOTE: Characters are 6x8 pixels.
//
// Input:
//      uint8_t line:
//          Line (0 to 7) to start string on.
//
//      uint8_t column:
//          Column (per pixel -> 0 to 127) to start character at.
//
//      uint8_t color:
//          Color (defined only).
//
//      const char *str:
//          Null terminated string to write to the frame buffer.
//
void glString(uint8_t line, uint8_t column, uint8_t color, const char *str);


#endif // GRAPHICS_H
