////////////////////////////////////////////////////////////////////////
// File: efis.c
// Header: efis.h
// Author: Michael R. Shannon
// Written: Monday, November 30, 2015
// Updated: Friday, December 11, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include <stdio.h>
#include "stdint.h"
#include "stdbool.h"
#include "mathlib.h"
#include "util.h"
#include "graphics.h"
#include "efis.h"


#define CENTER_X GL_FRAME_WIDTH/2   // center of frame buffer - x
#define CENTER_Y GL_FRAME_HEIGHT/2  // center of frame buffer - y
#define PLANE_RADIUS 3  // radius of plane symbol
#define PIX_PER_DEG 2   // pixels per degree of pitch


void efisDraw(int16_t yaw, int16_t pitch, int16_t roll, bool valid){
    efisDrawAI(pitch, roll);
    efisDrawCompass(yaw);
    efisDrawInvalid(valid);
}


void efisDrawInvalid(bool valid){

    static uint8_t invalidCounter = 0;

    // If data not valid start invalid counter (15 frames).
    if (!valid){
        invalidCounter = 15;
    }

    if (invalidCounter > 0){

        // Left side warning block.
        glRectFill(5, 5, 25, GL_MAX_Y-5, GL_COLOR_INVERT);
        glLine(5, 5, 25, GL_MAX_Y-5, GL_COLOR_INVERT);
        glLine(25, 5, 5, GL_MAX_Y-5, GL_COLOR_INVERT);

        // Right side warning block.
        glRectFill(GL_MAX_X-25, 5, GL_MAX_X-5, GL_MAX_Y-5, GL_COLOR_INVERT);
        glLine(GL_MAX_X-25, 5, GL_MAX_X-5, GL_MAX_Y-5, GL_COLOR_INVERT);
        glLine(GL_MAX_X-5, 5, GL_MAX_X-25, GL_MAX_Y-5, GL_COLOR_INVERT);

        --invalidCounter;
    }
}


void efisDrawCompass(int16_t yaw){

    char buffer[4];

    // Convert yaw to degrees and confine to 0 to 360 degrees.
    yaw = toDeg(yaw);
    while (yaw < 0){
        yaw += 360;
    }
    while (yaw > 360){
        yaw -= 360;
    }

    // Print yaw angle (heading) to buffer.
    sprintf(buffer, STR("%03d"), yaw);
    buffer[3] = '\0';

    // Write string buffer to the frame buffer (with background
    // rectangle).
    glRectFill_(CENTER_X-GL_CHAR_WIDTH-2, 0,
                CENTER_X+2*GL_CHAR_WIDTH+2, GL_CHAR_HEIGHT+2,
                GL_COLOR_BLACK);
    glString(7, CENTER_X-GL_CHAR_WIDTH, GL_COLOR_WHITE, buffer);
}


void efisDrawAI(int16_t pitch, int16_t roll){

    int16_t rollSin, rollCos;
    rollSin = sin16(roll);
    rollCos = cos16(roll);

    efisDrawHorizon(pitch, rollSin, rollCos);
    efisDrawPlane();
    efisDrawPitch(pitch, rollSin, rollCos);
    efisDrawRoll(roll);
}


void efisDrawPlane(){
    // The circle for the center.
    glCircle(CENTER_X, CENTER_Y, PLANE_RADIUS, GL_COLOR_INVERT);

    // The vertical up line.
    glVLine_(CENTER_X, CENTER_Y+PLANE_RADIUS+1, 
             CENTER_Y+PLANE_RADIUS+1+5, GL_COLOR_INVERT);

    // The two horizontal lines representing the wings.
    glHLine_(CENTER_X-PLANE_RADIUS-1-5, CENTER_X-PLANE_RADIUS-1,
             CENTER_Y, GL_COLOR_INVERT);
    glHLine_(CENTER_X+PLANE_RADIUS+1, CENTER_X+PLANE_RADIUS+1+5,
             CENTER_Y, GL_COLOR_INVERT);
}


void efisDrawPitch(int16_t pitch, int16_t rollSin, int16_t rollCos){

    int16_t min, max;
    int16_t x0, y0, x1, y1;

    // Prepare degrees.
    pitch = toDeg(pitch*PIX_PER_DEG);
    max = ((pitch + 25)/10)*10;
    min = ((pitch - 25)/10)*10;

    // Loop through each pitch line.
    while (max >= min){

        // Don't plot horizon again.
        if (max != 0){

            // Figure out if tick or sub-tick.
            if (max % 20){
                x0 = -10;
                x1 = +10;
            } else {
                x0 = -30;
                x1 = +30;
            }

            // Set vertical location.
            y0 = (max - pitch);
            y1 = (max - pitch);

            // Rotate pitch line to be parallel with horizon.
            rotate16_(&x0, &y0, rollSin, rollCos);
            rotate16_(&x1, &y1, rollSin, rollCos);

            // Shift to center.
            x0 += CENTER_X;
            x1 += CENTER_X;
            y0 += CENTER_Y;
            y1 += CENTER_Y;

            // Draw the pitch line.
            glLine(x0, y0, x1, y1, GL_COLOR_INVERT);
        }

        // Decrement pitch line by 10/PIX_PER_DEG degrees.
        max -= 10;
    }
}


void efisDrawRoll(int16_t roll){

    // Draw pointing triangle.
    glTriangleFill(CENTER_X-3, CENTER_Y+16,
                   CENTER_X+3, CENTER_Y+16,
                   CENTER_X, CENTER_Y+20, 
                   GL_COLOR_INVERT);

    // Draw each tick mark.
    efisDrawRollLine(fromDeg(-90) + roll, 10);
    efisDrawRollLine(fromDeg(-60) + roll, 10);
    efisDrawRollLine(fromDeg(-45) + roll,  5);
    efisDrawRollLine(fromDeg(-30) + roll, 10);
    efisDrawRollLine(fromDeg(-20) + roll,  5);
    efisDrawRollLine(fromDeg(-10) + roll,  5);
    efisDrawRollLine(fromDeg(  0) + roll, 10);
    efisDrawRollLine(fromDeg(+10) + roll,  5);
    efisDrawRollLine(fromDeg(+20) + roll,  5);
    efisDrawRollLine(fromDeg(+30) + roll, 10);
    efisDrawRollLine(fromDeg(+45) + roll,  5);
    efisDrawRollLine(fromDeg(+60) + roll, 10);
    efisDrawRollLine(fromDeg(+90) + roll, 10);
}


void efisDrawRollLine(int16_t theta, uint8_t length){

    int16_t x0, y0, x1, y1, s, c;

    // Generate non rotated tick mark coordinates.
    x0 = 0;
    x1 = 0;
    y0 = CENTER_Y - 10;
    y1 = CENTER_Y - 10 + (int16_t)(length-1);

    // Rotate the tick mark.
    s = sin16(theta);
    c = cos16(theta);
    rotate16_(&x0, &y0, s, c);
    rotate16_(&x1, &y1, s, c);

    // Shift to proper location.
    x0 += CENTER_X;
    x1 += CENTER_X;
    y0 += CENTER_Y;
    y1 += CENTER_Y;

    // Plot the tick mark.
    glLine(x0, y0, x1, y1, GL_COLOR_INVERT);
}


void efisDrawHorizon(int16_t pitch, int16_t rollSin, int16_t rollCos){

    bool outside, onePoint;
    int16_t x0, y0, x1, y1;
    int8_t n;
    uint8_t color;
    uint8_t xa[4];
    uint8_t ya[4];

    // Calculate horizon line.
    outside = efisHorizon(pitch, rollSin, rollCos, &x0, &y0, &x1, &y1);
    onePoint = x0 == x1 && y0 == y1;

    // Handle special cases of a 1 pixel horizon line and no horizon
    // line.
    if (outside || onePoint){
        if (pitch >= 0){
            glSet();
            color = GL_COLOR_BLACK;
        } else {
            glClear();
            color = GL_COLOR_WHITE;
        }
        if (onePoint){
            glPoint(x0, y0, color);
        }
        return;
    }

    // Calculate point of drawn entities.
    n = efisPoints(xa, ya, x0, y0, x1, y1);

    // Sky is drawn entity.
    if (n > 0){
        glClear();
        color = GL_COLOR_WHITE;

    // Ground is drawn entity.
    } else {
        glSet();
        color = GL_COLOR_BLACK;
        n = -n;
    }

    // Handle the 3 different scenarios.
    switch (n){
        case 2:     // draw as a line
            glLine(xa[0], ya[0], xa[1], ya[1], color);
            break;
        case 3:     // draw as a triangle
            glTriangleFill(xa[0], ya[0], xa[1], ya[1], xa[2], ya[2], color);
            break;
        case 4:     // draw as a rectangle + triangle
            efisDrawHorizonAsPolygon(xa, ya, color);
            break;
    }
}



// Calculate horizon line.
bool efisHorizon(int16_t pitch, int16_t rollSin, int16_t rollCos,
                 int16_t *x0_ptr, int16_t *y0_ptr,
                 int16_t *x1_ptr, int16_t *y1_ptr){

    // Initialize horizon line to correct pitch.
    *x0_ptr = -128;
    *x1_ptr =  128;
    /* *y0_ptr = -((int32_t)pitch*360L)/16384L; */
    *y0_ptr = -toDeg(pitch*PIX_PER_DEG);
    *y1_ptr = *y0_ptr;

    // Rotate horizon line by pitch angle.
    rotate16_(x0_ptr, y0_ptr, rollSin, rollCos);
    rotate16_(x1_ptr, y1_ptr, rollSin, rollCos);

    // Shift horizon line into center of frame buffer.
    *x0_ptr += GL_FRAME_WIDTH/2;
    *x1_ptr += GL_FRAME_WIDTH/2;
    *y0_ptr += GL_FRAME_HEIGHT/2;
    *y1_ptr += GL_FRAME_HEIGHT/2;

    // Clip horizon line to frame buffer.
    return glClipLine(x0_ptr, y0_ptr, x1_ptr, y1_ptr);
}


// Inverted from side of line algorithm at:
//      http://math.stackexchange.com/a/274728
#define EFIS_SIDE_OF_LINE(x, y) ((y - y0)*(x1 - x0) - (x - x0)*(y1 - y0))
int8_t efisPoints(uint8_t xa[], uint8_t ya[],
                  int16_t x0, int16_t y0,
                  int16_t x1, int16_t y1){

    int16_t d;
    int8_t n;
    int8_t na = 0, nb = 0; // number of points above/below horizon line
    int16_t xaa[4]; // x array, above
    int16_t yaa[4]; // y array, above
    int16_t xab[4]; // x array, below
    int16_t yab[4]; // y array, below

    // Load horizon points into arrays.
    xa[0] = x0;
    ya[0] = y0;
    xa[1] = x1;
    ya[1] = y1;
    n = 2;

    // NOTE: Points that are on the horizon line are not added as they
    //       already exist from the horizon line itself.

    // Check lower left corner.
    d = EFIS_SIDE_OF_LINE(GL_MIN_X, GL_MIN_Y);
    if (d > 0){
        xaa[na] = GL_MIN_X; yaa[na++] = GL_MIN_Y;
    } else if (d < 0){
        xab[nb] = GL_MIN_X; yab[nb++] = GL_MIN_Y;
    }

    // Check lower right corner.
    d = EFIS_SIDE_OF_LINE(GL_MAX_X, GL_MIN_Y);
    if (d > 0){
        xaa[na] = GL_MAX_X; yaa[na++] = GL_MIN_Y;
    } else if (d < 0){
        xab[nb] = GL_MAX_X; yab[nb++] = GL_MIN_Y;
    }

    // Check upper right corner.
    d = EFIS_SIDE_OF_LINE(GL_MAX_X, GL_MAX_Y);
    if (d > 0){
        xaa[na] = GL_MAX_X; yaa[na++] = GL_MAX_Y;
    } else if (d < 0){
        xab[nb] = GL_MAX_X; yab[nb++] = GL_MAX_Y;
    }

    // Check upper left corner.
    d = EFIS_SIDE_OF_LINE(GL_MIN_X, GL_MAX_Y);
    if (d > 0){
        xaa[na] = GL_MIN_X; yaa[na++] = GL_MAX_Y;
    } else if (d < 0){
        xab[nb] = GL_MIN_X; yab[nb++] = GL_MAX_Y;
    }

    // Choose the sky as the drawn entity.
    if (na <= nb){
        // Copy sky points to arrays.
        while(na > 0){
            xa[n] = xaa[--na]; ya[n++] = yaa[na];
        }
        // Return length of result array.
        return n;

    // Choose the ground as the drawn entity.
    } else {
        // Copy sky points to arrays.
        while(nb > 0){
            xa[n] = xab[--nb]; ya[n++] = yab[nb];
        }
        // Return negative length of result array.
        return -n;
    }
}


void efisDrawHorizonAsPolygon(uint8_t xa[4], uint8_t ya[4], uint8_t color){

    uint8_t peak;

    // Horizontal case.
    if (xa[2] == xa[3]){

        // Drawing left side of frame buffer.
        if (xa[2] == GL_MIN_X){
            // Get index of triangle point.
            peak = (xa[0] > xa[1]) ? 0 : 1;
            // Draw rectangle.
            glRectFill_(GL_MIN_X, GL_MIN_Y, xa[!peak], GL_MAX_Y, color);

        // Drawing right side of frame buffer.
        } else {
            // Get index of triangle point.
            peak = (xa[0] < xa[1]) ? 0 : 1;
            // Draw rectangle.
            glRectFill_(xa[!peak], 0, GL_MAX_X, GL_MAX_Y, color);
        }

        // Draw triangle.
        glTriangleFillFVS(xa[!peak], GL_MIN_Y, GL_MAX_Y,
                          xa[peak], ya[peak], color);

    // Vertical case.
    } else { // ya[0] == ya[1]

        // Drawing bottom of frame buffer.
        if (ya[2] == GL_MIN_Y){
            // Get index of triangle point.
            peak = (ya[0] > ya[1]) ? 0 : 1;
            // Draw rectangle.
            glRectFill_(GL_MIN_X, GL_MIN_Y, GL_MAX_X, ya[!peak], color);

        // Drawing top of frame buffer.
        } else {
            // Get index of triangle point.
            peak = (ya[0] < ya[1]) ? 0 : 1;
            // Draw rectangle.
            glRectFill_(GL_MIN_X, ya[!peak], GL_MAX_X, GL_MAX_Y, color);
        }

        // Draw triangle.
        glTriangleFillFVS(xa[peak], ya[peak], ya[!peak],
                          xa[!peak], ya[!peak], color);
    }
}
