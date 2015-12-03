////////////////////////////////////////////////////////////////////////
// File: efis.c
// Header: efis.h
// Author: Michael R. Shannon
// Written: Monday, November 30, 2015
// Updated: Thursday, December 03, 2015
// Device: PIC18F87K22
// Compiler: C18
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include <p18cxxx.h>
#include <math.h>
#include "stdint.h"
#include "stdbool.h"
#include "mathlib.h"
#include "efis.h"

#include "led.h"


void efisDraw(int16_t yaw, int16_t pitch, int16_t roll){

    efisDrawHorizon(pitch, roll);
    glCircle(63, 31, 5, GL_COLOR_INVERT);

}


void efisAI(float pitch, float roll){
}


// Calculate horizon line.
bool efisHorizon(int16_t pitch, int16_t roll,
                 int16_t *x0_ptr, int16_t *y0_ptr,
                 int16_t *x1_ptr, int16_t *y1_ptr){

    int16_t s, c;

    // Initialize horizon line to correct pitch.
    *x0_ptr = -128;
    *x1_ptr =  128;
    *y0_ptr = -((int32_t)pitch*360L)/16384L;
    *y1_ptr = *y0_ptr;

    // Rotate horizon line by pitch angle.
    s = sin16(roll);
    c = cos16(roll);
    rotate16_(x0_ptr, y0_ptr, s, c);
    rotate16_(x1_ptr, y1_ptr, s, c);

    // Shift horizon line into center of frame buffer.
    *x0_ptr += GL_FRAME_WIDTH/2;
    *x1_ptr += GL_FRAME_WIDTH/2;
    *y0_ptr += GL_FRAME_HEIGHT/2;
    *y1_ptr += GL_FRAME_HEIGHT/2;

    // Clip horizon line to frame buffer.
    return glClipLine(x0_ptr, y0_ptr, x1_ptr, y1_ptr);
}


// Determine points to draw and whether it is sky or ground.
//
// The two horizon points will always be returned first.
//
// The number of points in the polygon is returned.  If the polygon is
// for the sky this number is positive, if the polygon is for the
// ground it is negative.
//
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
            /* glRectFill_(GL_MIN_X, GL_MIN_Y, 10, GL_MAX_Y, color); */

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


void efisDrawHorizon(int16_t pitch, int16_t roll){

    bool outside, onePoint;
    int16_t s, c, x0, y0, x1, y1;
    int8_t n;
    uint8_t color;
    uint8_t xa[4];
    uint8_t ya[4];

    // Calculate horizon line.
    outside = efisHorizon(pitch, roll, &x0, &y0, &x1, &y1);
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


void efisPitch(float pitch, float roll){
}


void efisRoll(float roll){
}


void efisPlane(){
}


void efisCompass(float yaw){
}






