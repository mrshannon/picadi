///////////////////////////////////////////////////////////////////////
// File: graphics.c
// Header: graphics.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Sunday, November 15, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include <string.h>
#include <math.h>
#include "stdint.h"
#include "util.h"
#include "graphics.h"


#define byteNumber(y) (7 - y/8)
#define bitNumber(y, byte) ((8-byte)*8 - (y + 1))


// Extern the frame buffer.
extern uint8_t frameBuffer[GL_FRAME_SIZE];


void glClear(void){
    memset(frameBuffer, 0x00, GL_FRAME_SIZE);
}


void glSet(void){
    memset(frameBuffer, 0xFF, GL_FRAME_SIZE);
}


void glInvert(void){
    uint16_t i;
    for (i = 0; i < GL_FRAME_SIZE; ++i){
        frameBuffer[i] = ~frameBuffer[i];
    }
}


void glPoint(int16_t x, int16_t y, uint8_t color){

    uint8_t brush;
    int16_t byte, idx;

    // Return immediately if pixel is off the screen.
    if (x < 0 || x >= GL_FRAME_WIDTH || y < 0 || y >= GL_FRAME_HEIGHT){
        return;
    }

    // Compute byte to draw point in.
    byte = byteNumber(y);

    // Select brush.
    switch (bitNumber(y, byte)){
        case 0: brush = 0b00000001; break;
        case 1: brush = 0b00000010; break;
        case 2: brush = 0b00000100; break;
        case 3: brush = 0b00001000; break;
        case 4: brush = 0b00010000; break;
        case 5: brush = 0b00100000; break;
        case 6: brush = 0b01000000; break;
        case 7: brush = 0b10000000; break;
    }

    // Calculate byte index.
    idx = byte + x*(GL_FRAME_HEIGHT/8);

    // Draw point with given color.
    switch (color){
        case GL_COLOR_WHITE:
            frameBuffer[idx] |= brush;
            break;
        case GL_COLOR_BLACK:
            frameBuffer[idx] &= ~brush;
            break;
        case GL_COLOR_INVERT:
            frameBuffer[idx] ^= brush;
            break;
    }
}


void glVLine(int16_t x, int16_t y0, int16_t y1, uint8_t color){

    int16_t tmp;

    // Shortcut 1 dot line.
    if (y0 == y1){
        glPoint(x, y0, color);
        return;
    }

    // Fix order of y0 and y1.
    if (y0 > y1){
        tmp = y1;
        y1 = y0;
        y0 = tmp;
    }

    // Return immediately if line is off the screen.
    if (x < 0 || x >= GL_FRAME_WIDTH){
        return;
    }
    if (y1 < 0 || y0 >= GL_FRAME_HEIGHT){
        return;
    }

    // Constrain line to the frame buffer.
    if (y1 >= GL_FRAME_HEIGHT){
        y1 = GL_FRAME_HEIGHT - 1;
    }
    if (y0 < 0){
        y0 = 0;
    }

    // Draw the line.
    glVLine_(x, y0, y1, color);
}


void glVLine_(uint8_t x, uint8_t y0, uint8_t y1, uint8_t color){

    uint8_t startByte, startBit, startBrush, endByte, endBit, endBrush;
    uint16_t idx, endIdx;

    // Calculate start and end bytes.
    startByte = byteNumber(y1);
    startBit  = bitNumber(y1, startByte);
    endByte   = byteNumber(y0);
    endBit    = bitNumber(y0, endByte);

    // Find start brush.
    switch (startBit){
        case 0: startBrush = 0b11111111; break;
        case 1: startBrush = 0b11111110; break;
        case 2: startBrush = 0b11111100; break;
        case 3: startBrush = 0b11111000; break;
        case 4: startBrush = 0b11110000; break;
        case 5: startBrush = 0b11100000; break;
        case 6: startBrush = 0b11000000; break;
        case 7: startBrush = 0b10000000; break;
    }

    // Find end brush.
    switch (endBit){
        case 0: endBrush = 0b00000001; break;
        case 1: endBrush = 0b00000011; break;
        case 2: endBrush = 0b00000111; break;
        case 3: endBrush = 0b00001111; break;
        case 4: endBrush = 0b00011111; break;
        case 5: endBrush = 0b00111111; break;
        case 6: endBrush = 0b01111111; break;
        case 7: endBrush = 0b11111111; break;
    }

    // Calculate starting byte index.
    idx = (uint16_t)startByte + (uint16_t)x*(GL_FRAME_HEIGHT/8);

    // Shortcut when line is contained within a single byte.
    if (startByte == endByte){
        switch (color){
            case GL_COLOR_WHITE:
                frameBuffer[idx] |= (startBrush & endBrush);
                break;
            case GL_COLOR_BLACK:
                frameBuffer[idx] &= ~(startBrush & endBrush);
                break;
            case GL_COLOR_INVERT:
                frameBuffer[idx] ^= (startBrush & endBrush);
                break;
        }
        return;
    }

    // Calculate ending index.
    endIdx = idx + (uint16_t)(endByte - startByte);

    // Choose color and draw line.
    switch (color){
        case GL_COLOR_WHITE:
            frameBuffer[idx++] |= startBrush;
            while (idx < endIdx){
                frameBuffer[idx++] = 0xFF;
            }
            frameBuffer[idx] |= endBrush;
            break;
        case GL_COLOR_BLACK:
            frameBuffer[idx++] &= ~startBrush;
            while (idx < endIdx){
                frameBuffer[idx++] = 0x00;
            }
            frameBuffer[idx] &= ~endBrush;
            break;
        case GL_COLOR_INVERT:
            frameBuffer[idx++] ^= startBrush;
            while (idx < endIdx){
                frameBuffer[idx++] ^= 0xFF;
            }
            frameBuffer[idx] ^= endBrush;
            break;
    }
}


void glHLine(int16_t x0, int16_t x1, int16_t y, uint8_t color){

    int16_t tmp;

    // Shortcut 1 dot line.
    if (x0 == x1){
        glPoint(x0, y, color);
        return;
    }

    // Fix order of x0 and x1.
    if (x0 > x1){
        tmp = x1;
        x1 = x0;
        x0 = tmp;
    }

    // Return immediately if line is off the screen.
    if (y < 0 || y >= GL_FRAME_HEIGHT){
        return;
    }
    if (x1 < 0 || x0 >= GL_FRAME_WIDTH){
        return;
    }

    // Constrain line to the frame buffer.
    if (x1 >= GL_FRAME_WIDTH){
        x1 = GL_FRAME_WIDTH - 1;
    }
    if (x0 < 0){
        x0 = 0;
    }

    // Draw the line.
    glHLine_(x0, x1, y, color);
}


void glHLine_(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color){

    uint8_t byte, brush, tmp, i;
    uint16_t idx;

    // Compute byte to draw point in.
    byte = byteNumber(y);

    // Select brush.
    switch (bitNumber(y, byte)){
        case 0: brush = 0b00000001; break;
        case 1: brush = 0b00000010; break;
        case 2: brush = 0b00000100; break;
        case 3: brush = 0b00001000; break;
        case 4: brush = 0b00010000; break;
        case 5: brush = 0b00100000; break;
        case 6: brush = 0b01000000; break;
        case 7: brush = 0b10000000; break;
    }

    // Calculate start byte index.
    idx = (uint16_t)byte + (uint16_t)x0*(GL_FRAME_HEIGHT/8);

    // Draw line with given color.
    tmp = x1 - x0; // length of line - 1
    switch (color){
        case GL_COLOR_WHITE:
            for (i = 0; i <= tmp; ++i){
                frameBuffer[idx] |= brush;
                idx += GL_FRAME_HEIGHT/8;
            }
            break;
        case GL_COLOR_BLACK:
            for (i = 0; i <= tmp; ++i){
                frameBuffer[idx] &= ~brush;
                idx += GL_FRAME_HEIGHT/8;
            }
            break;
        case GL_COLOR_INVERT:
            for (i = 0; i <= tmp; ++i){
                frameBuffer[idx] ^= brush;
                idx += GL_FRAME_HEIGHT/8;
            }
            break;
    }
}


// Bresenham algorithm from:
// http://members.chello.at/~easyfilter/bresenham.html
//
// This function does not check for out of bounds lines and lets the
// underling glPoint function sort it out.  If more performance is
// needed consider limiting the line to the frame buffer.
void glLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color){

    int16_t dx, dy, sx, sy, err, e2;

    // Shortcut vertical line.
    if (x0 == x1){
        glVLine(x0, y0, y1, color);
        return;
    }

    // Shortcut horizontal line line.
    if (y0 == y1){
        glHLine(x0, x1, y0, color);
        return;
    }

    // Compute delta values in each coordinate.
    dx =  abs16(x1 - x0);
    dy = -abs16(y1 - y0);

    // Calculate steps in each dimension.
    sx = x0 < x1 ? 1 : -1;
    sy = y0 < y1 ? 1 : -1;

    // Calculate initial error.
    err = dx + dy;

    // Draw the line.
    while(1){
        glPoint(x0, y0, color);
        e2 = 2*err;
        // Step x dimension.
        if (e2 >= dy){
            // Check if done.
            if (x0 == x1){
                break;
            }
            // Update error and step x.
            err += dy;
            x0 += sx;
        }
        // Step y dimension.
        if (e2 <= dx){
            // Check if done.
            if (y0 == y1){
                break;
            }
            // Update error and step x.
            err += dx;
            y0 += sy;
        }
    }
}


void glRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color){

    // Draw rectangle.
    glVLine(x0, y0, y1, color);
    glVLine(x1, y0, y1, color);
    glHLine(x0, x1, y0, color);
    glHLine(x0, x1, y1, color);

    // If the color is invert we need to draw the corners an odd number
    // of times.
    if (color == GL_COLOR_INVERT){
        glPoint(x0, y0, color);
        glPoint(x0, y1, color);
        glPoint(x1, y1, color);
        glPoint(x1, y0, color);
    }
}


void glRectFill(
        int16_t x0, int16_t y0,
        int16_t x1, int16_t y1,
        uint8_t color){

    int16_t tmp;

    // Shortcut to vertical line.
    if (x0 == x1){
        glVLine(x0, y0, y1, color);
    }

    // Shortcut to horizontal line.
    if (y0 == y1){
        glHLine(x0, x1, y0, color);
    }

    // Fix order of x0 and x1.
    if (x0 > x1){
        tmp = x1;
        x1 = x0;
        x0 = tmp;
    }

    // Fix order of y0 and y1.
    if (y0 > y1){
        tmp = y1;
        y1 = y0;
        y0 = tmp;
    }

    // Return immediately if rectangle is off the screen.
    if (x1 < 0 || x0 >= GL_FRAME_WIDTH){
        return;
    }
    if (y1 < 0 || y0 >= GL_FRAME_HEIGHT){
        return;
    }

    // Constrain rectangle to frame buffer.
    if (x1 >= GL_FRAME_WIDTH){
        x1 = GL_FRAME_WIDTH - 1;
    }
    if (x0 < 0){
        x0 = 0;
    }
    if (y1 >= GL_FRAME_HEIGHT){
        y1 = GL_FRAME_HEIGHT - 1;
    }
    if (y0 < 0){
        y0 = 0;
    }

    // Draw rectangle.
    glRectFill_(x0, y0, x1, y1, color);
}


void glRectFill_(
        uint8_t x0, uint8_t y0,
        uint8_t x1, uint8_t y1,
        uint8_t color){

    uint8_t startByte, startBit, startBrush, endByte, endBit, endBrush;
    uint16_t idx, startIdx, endIdx;

    // Calculate start and end bytes.
    startByte = byteNumber(y1);
    startBit  = bitNumber(y1, startByte);
    endByte   = byteNumber(y0);
    endBit    = bitNumber(y0, endByte);

    // Find start brush.
    switch (startBit){
        case 0: startBrush = 0b11111111; break;
        case 1: startBrush = 0b11111110; break;
        case 2: startBrush = 0b11111100; break;
        case 3: startBrush = 0b11111000; break;
        case 4: startBrush = 0b11110000; break;
        case 5: startBrush = 0b11100000; break;
        case 6: startBrush = 0b11000000; break;
        case 7: startBrush = 0b10000000; break;
    }

    // Find end brush.
    switch (endBit){
        case 0: endBrush = 0b00000001; break;
        case 1: endBrush = 0b00000011; break;
        case 2: endBrush = 0b00000111; break;
        case 3: endBrush = 0b00001111; break;
        case 4: endBrush = 0b00011111; break;
        case 5: endBrush = 0b00111111; break;
        case 6: endBrush = 0b01111111; break;
        case 7: endBrush = 0b11111111; break;
    }

    // Calculate starting byte index.
    startIdx = (uint16_t)startByte + (uint16_t)x0*(GL_FRAME_HEIGHT/8);

    // Shortcut when rectangle is contained within a single byte row.
    if (startByte == endByte){
        idx = startIdx;
        switch (color){
            case GL_COLOR_WHITE:
                for (; x0 <= x1; ++x0){
                    frameBuffer[idx] |= (startBrush & endBrush);
                    idx += GL_FRAME_HEIGHT/8;
                }
                break;
            case GL_COLOR_BLACK:
                for (; x0 <= x1; ++x0){
                    frameBuffer[idx] &= ~(startBrush & endBrush);
                    idx += GL_FRAME_HEIGHT/8;
                }
                break;
            case GL_COLOR_INVERT:
                for (; x0 <= x1; ++x0){
                    frameBuffer[idx] ^= (startBrush & endBrush);
                    idx += GL_FRAME_HEIGHT/8;
                }
                break;
        }
        return;
    }

    // Calculate ending index.
    endIdx = startIdx + (uint16_t)(endByte - startByte);

    // Choose color and draw rectangle.
    switch (color){
        case GL_COLOR_WHITE:
            for (; x0 <= x1; ++x0){
                idx = startIdx;

                // Draw vertical line.
                frameBuffer[idx++] |= startBrush;
                while (idx < endIdx){
                    frameBuffer[idx++] = 0xFF;
                }
                frameBuffer[idx] |= endBrush;

                // Increment indices.
                startIdx += GL_FRAME_HEIGHT/8;
                endIdx += GL_FRAME_HEIGHT/8;
            }
            break;
        case GL_COLOR_BLACK:
            for (; x0 <= x1; ++x0){
                idx = startIdx;

                // Draw vertical line.
                frameBuffer[idx++] &= ~startBrush;
                while (idx < endIdx){
                    frameBuffer[idx++] = 0x00;
                }
                frameBuffer[idx] &= ~endBrush;

                // Increment indices.
                startIdx += GL_FRAME_HEIGHT/8;
                endIdx += GL_FRAME_HEIGHT/8;
            }
            break;
        case GL_COLOR_INVERT:
            for (; x0 <= x1; ++x0){
                idx = startIdx;

                // Draw vertical line.
                frameBuffer[idx++] ^= startBrush;
                while (idx < endIdx){
                    frameBuffer[idx++] ^= 0xFF;
                }
                frameBuffer[idx] ^= endBrush;

                // Increment indices.
                startIdx += GL_FRAME_HEIGHT/8;
                endIdx += GL_FRAME_HEIGHT/8;
            }
            break;
    }
}


// Bresenham algorithm for Ellipses from:
// http://members.chello.at/~easyfilter/bresenham.html
void glEllipse(int16_t xc, int16_t yc, uint8_t xr, uint8_t yr, uint8_t color){

    int16_t x, y;
    int32_t dx, dy, e2, err, a2, b2;

    // Precomputed constants.
    a2 = 2*(int32_t)xr*(int32_t)xr;
    b2 = 2*(int32_t)yr*(int32_t)yr;

    // Quadrant 2.
    x = -(int16_t)xr;
    y = 0;
    e2 = yr;

    // Error increments.
    dx = (1 + 2*x) * e2*e2;
    dy = x*x;

    // Error of first step.
    err = dx + dy;

    // Draw ellipse.
    do {
        // Draw pixel in each quadrant.
        if (y == 0){
        glPoint(xc+x, yc+y, color);
        glPoint(xc-x, yc+y, color);
        } else if (x == 0){
        glPoint(xc+x, yc+y, color);
        glPoint(xc+x, yc-y, color);
        } else {
        glPoint(xc+x, yc+y, color);
        glPoint(xc+x, yc-y, color);
        glPoint(xc-x, yc-y, color);
        glPoint(xc-x, yc+y, color);
        }

        // Update pixel coordinate.
        e2 = 2*err;
        if (e2 >= dx){
            x++;
            err += dx += b2;
        }
        if (e2 <= dy){
            y++;
            err += dy += a2;
        }
    } while (x <= 0);

    // Finish ellipse where xr == 1;
    while (y++ < (int16_t)yr){
        glPoint(xc, yc+y, color);
        glPoint(xc, yc-y, color);
    }
}


void glCircle(int16_t xc, int16_t yc, uint8_t r, uint8_t color){
    glEllipse(xc, yc, r, r, color);
}


void glTriangle(int16_t x0, int16_t y0,
                int16_t x1, int16_t y1,
                int16_t x2, int16_t y2,
                uint8_t color){

    // Draw triangle
    glLine(x0, y0, x1, y1, color);
    glLine(x1, y1, x2, y2, color);
    glLine(x2, y2, x0, y0, color);

    // If the color is invert we need to draw the corners an odd number
    // of times.
    if (color == GL_COLOR_INVERT){
        glPoint(x0, y0, color);
        glPoint(x1, y1, color);
        glPoint(x2, y2, color);
    }
}


// Triangle rasterization algorithm from:
// http://www.sunshine2k.de/coding/java/TriangleRasterization/
//      TriangleRasterization.html#algo3
void glTriangleFill_(int16_t x0, int16_t y0,
                     int16_t x1, int16_t y1,
                     int16_t xm, int16_t ym,
                     uint8_t color){

    int16_t yi, yi01, yi10;

    // Calculate the two possible y intercepts.
    yi01 = glYIntercept(x0, y0, x1, y1, xm);
    yi10 = glYIntercept(x1, y1, x0, y0, xm);

    // Choose which y intercept to use.
    if (abs16(ym - yi01) >= abs16(ym - yi10)){
        yi = yi01;
    } else {
        yi = yi10;
    }

    // Draw each half of the triangle.
    glTriangleFillFS(xm, ym, yi, x0, y0, color);
    glTriangleFillFS(xm, ym, yi, x1, y1, color);

    // Fix for invert color.
    if (color == GL_COLOR_INVERT){
        glVLine(xm, ym, yi, color);
    }
}


void glTriangleFill(int16_t x0, int16_t y0,
                    int16_t x1, int16_t y1,
                    int16_t x2, int16_t y2,
                    uint8_t color){

    // Rearange points and call low level traingle fill function.
    if (x1 <= x2){
        if (x0 <= x1){
            // x1 is middle
            glTriangleFill_(x0, y0, x2, y2, x1, y1, color);
        } else if (x0 >= x2){
            // x2 is middle
            glTriangleFill_(x0, y0, x1, y1, x2, y2, color);
        } else {
            // x0 is middle
            glTriangleFill_(x1, y1, x2, y2, x0, y0, color);
        }
    } else { // x1 > x2
        if (x0 <= x2){
            // x2 is middle
            glTriangleFill_(x0, y0, x1, y1, x2, y2, color);
        } else if (x0 >= x1){
            // x1 is middle
            glTriangleFill_(x0, y0, x2, y2, x1, y1, color);
        } else {
            // x0 is middle
            glTriangleFill_(x1, y1, x2, y2, x0, y0, color);
        }
    }
}


// Triangle rasterization algorithm from:
// http://www.sunshine2k.de/coding/java/TriangleRasterization/
//      TriangleRasterization.html#algo3
//
// Bresenham algorithm from:
// http://members.chello.at/~easyfilter/bresenham.html
void glTriangleFillFS(int16_t xs, int16_t ys0, int16_t ys1,
        int16_t xp, int16_t yp,
        uint8_t color){

    int16_t y0, y1, dx, dy0, dy1, sx, sy0, sy1, err0, err1, e2;

    // Compute delta values in each coordinate.
    dx  =  abs16(xs - xp);
    dy0 = -abs16(ys0 - yp);
    dy1 = -abs16(ys1 - yp);

    // Calculate steps in each dimension.
    sx  = xp < xs ? 1 : -1;
    sy0 = yp < ys0 ? 1 : -1;
    sy1 = yp < ys1 ? 1 : -1;

    // Calculate initial error.
    err0 = dx + dy0;
    err1 = dx + dy1;

    // Separate y's for incrementing.
    y0 = yp;
    y1 = yp;

    // Draw point of triangle.
    glPoint(xp, yp, color);

    // Draw last slice.
    glVLine(xs, ys0, ys1, color);
    xs  -= sx;

    // Step one bounding line.
    while(1){
        e2 = 2*err0;
        // Step y dimension.
        if (e2 <= dx){
            // Check if done.
            if (y0 == ys0){
                break;
            }
            // Update error and step x.
            err0 += dx;
            y0 += sy0;
        }
        // Step x dimension.
        if (e2 >= dy0){
            // Check if done.
            if (xp == xs){
                break;
            }
            // Update error and step x.
            err0 += dy0;
            xp += sx;

            // Step other bounding line.
            while(1){
                e2 = 2*err1;
                // Step y dimension.
                if (e2 <= dx){
                    // Update error and step x.
                    err1 += dx;
                    y1 += sy1;
                }
                // Step x dimension.
                if (e2 >= dy1){
                    // Update error.
                    err1 += dy1;
                    // Draw vertical line to fill triangle.
                    glVLine(xp, y0, y1, color);
                    break;
                }
            }
        }
    }
}


void glRotate(uint8_t *xPtr, uint8_t *yPtr, float theta){
}


// x must be between x0 and x1.
int16_t glYIntercept(int16_t x0, int16_t y0,
                     int16_t x1, int16_t y1,
                     int16_t x){

    int16_t dx, dy, sx, sy, err, e2, tmp;

    // Shortcut if slope is 0.
    if (y0 == y1){
        return y0;
    }

    // Compute delta values in each coordinate.
    dx =  abs16(x1 - x0);
    dy = -abs16(y1 - y0);

    // Calculate steps in each dimension.
    sx = x0 < x1 ? 1 : -1;
    sy = y0 < y1 ? 1 : -1;

    // Calculate initial error.
    err = dx + dy;

    // Draw the line.
    while(1){
        e2 = 2*err;
        // Step x dimension.
        if (e2 >= dy){
            // Update error and step x.
            err += dy;
            x0 += sx;
        }
        // Step y dimension.
        if (e2 <= dx){
            // Check if done.
            // Update error and step x.
            err += dx;
            y0 += sy;
        }
        // Check for intercept.
        if (x0 == x){
            return y0;
        }
    }
}


void glChar8x6LP(char ch){
}


void glString8x6LP(uint8_t line, uint8_t x, char *str){
}
