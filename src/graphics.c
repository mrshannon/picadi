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
#include "graphics.h"

#include "led.h"


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


// Algorithm from: http://joshbeam.com/articles/simple_line_drawing/
void glLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color){

    uint8_t min, max, y, x;
    float slope;

    // Shortcut 1 dot line.
    if (x0 == x1 && y0 == y1){
        glPoint(x0, y0, color);
        return;
    }

    // Calculate slope of the line.
    slope = (float)(y1 - y0)/(float)(x1 - x0);

    // Treat x as independent variable.
    if (slope <= 1.0f){

        // Find minimum and maximum x.
        if (x1 > x0){
            min = x0;
            max = x1;
        } else {
            min = x1;
            max = x0;
        }

        // Shortcut if line is not in the frame buffer.
        if (min >= GL_FRAME_WIDTH){
            return;
        }

        // Limit range to framebuffer.
        // TODO: This could be better since the aspect ratio of the
        //       screen is 2:1.
        if (max >= GL_FRAME_WIDTH){
            max = GL_FRAME_WIDTH - 1;
        }

        // Draw line.
        for (x = min; x <= max; ++x){
            y = y0 + (uint8_t)((float)(x - x0) * slope + 0.5);
            glPoint(x, y, color);
        }

    // Treat y as independent variable.
    } else {

        // Find minimum and maximum y.
        if (y1 > y0){
            min = y0;
            max = y1;
        } else {
            min = y1;
            max = y0;
        }

        // Shortcut if line is not in the frame buffer.
        if (min >= GL_FRAME_WIDTH){
            return;
        }

        // Limit range to framebuffer.
        if (max >= GL_FRAME_HEIGHT){
            max = GL_FRAME_HEIGHT - 1;
        }

        // Invert the slope.
        slope = 1/slope;

        // Draw line.
        for (y = min; y <= max; ++y){
            x = x0 + (uint8_t)((float)(y - y0) * slope + 0.5);
            glPoint(x, y, color);
        }
    }
}


void glRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color){
    glVLine(x0, y0, y1, color);
    glVLine(x1, y0, y1, color);
    glHLine(x0, x1, y0, color);
    glHLine(x0, x1, y1, color);
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


void glCircle(uint8_t xc, uint8_t r, uint8_t color){
}


void glTriangleFBFill(uint8_t x0, uint8_t x1, uint8_t yb,
                      uint8_t xp, uint8_t yp, uint8_t color){
}


void glRotate(uint8_t *xPtr, uint8_t *yPtr, float theta){
}


void glChar8x6LP(char ch){
}


void glString8x6LP(uint8_t line, uint8_t x, char *str){
}
