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
#include "stdint.h"
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


void glPoint(uint8_t x, uint8_t y, uint8_t color){

    uint8_t byte;
    uint8_t brush;
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

    // Calculate byte index.
    idx = (uint16_t)byte + (uint16_t)x*(GL_FRAME_HEIGHT/8);

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


void glVLine(uint8_t x, uint8_t y0, uint8_t y1, uint8_t color){

    uint8_t startByte, startBit, startBrush, endByte, endBit, endBrush;
    uint16_t idx, endIdx;

    // Calculate start and end bytes.
    if (y1 > y0){
        startByte = byteNumber(y1);
        startBit  = bitNumber(y1, startByte);
        endByte   = byteNumber(y0);
        endBit    = bitNumber(y0, endByte);
    } else if (y0 > y1){
        startByte = byteNumber(y0);
        startBit  = bitNumber(y0, startByte);
        endByte   = byteNumber(y1);
        endBit    = bitNumber(y1, endByte);
    } else {
        glPoint(x, y0, color);
        return;
    }

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
                frameBuffer[idx] = ~frameBuffer[idx];
                ++idx;
            }
            frameBuffer[idx] ^= endBrush;
            break;
    }
}


void glHLine(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color){

    uint8_t byte, brush, tmp, i;
    uint16_t idx;

    // Fix order of x0, x1.
    if (x0 > x1){
        tmp = x1;
        x1 = x0;
        x0 = tmp;
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

    // Calculate start byte index.
    idx = (uint16_t)byte + (uint16_t)x0*(GL_FRAME_HEIGHT/8);

    // Draw point with given color.
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


void glLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color){
}


void glRectFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color){
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
