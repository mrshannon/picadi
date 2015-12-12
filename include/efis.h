////////////////////////////////////////////////////////////////////////
// File: efis.h
// Author: Michael R. Shannon
// Written: Monday, November 30, 2015
// Updated: Friday, December 11, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library draws an EFIS to the frame buffer using the
//      graphics library.  It does not compute the attitude solution,
//      only displays it.
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"
#include "stdbool.h"


#ifndef EFIS_H
#define EFIS_H


// Description:
//      Draw the entire EFIS to the frame buffer.  This is everything
//      that is drawn to the OLED display.
//
// Input:
//      int16_t yaw:
//          Heading from 0 to 360 degrees in TRIG16 units.
//
//      int16_t pitch:
//          Pitch from -90 to 90 degrees in TRIG16 units.
//
//      int16_t roll:
//          Roll (positive clockwise) from -180 to 180 degrees in TRIG16
//          units.
//
//      bool valid:
//          True if the attitude solution can be trusted.
//
void efisDraw(int16_t yaw, int16_t pitch, int16_t roll, bool valid);


// Description:
//      Draw invalid boxes on either side of the screen if <valid> is
//      false.
//
// Input:
//      bool valid:
//          Draw invalid boxes if false.
//
void efisDrawInvalid(bool valid);


// Description:
//      Draw compass heading.  Consists of a black filled rectangle and
//      a 3 digit heading number.
//
// Input:
//      int16_t yaw:
//          Heading from 0 to 360 degrees in TRIG16 units.
//
void efisDrawCompass(int16_t yaw);


// Description:
//      Draw attitude indicator.  This consists of the entire EFIS
//      except for the compass and invalid indicator.
//
// Input:
//      int16_t pitch:
//          Pitch from -90 to 90 degrees in TRIG16 units.
//
//      int16_t roll:
//          Roll (positive clockwise) from -180 to 180 degrees in TRIG16
//          units.
//
void efisDrawAI(int16_t pitch, int16_t roll);


// Description:
//      Draw plane symbol.  Consists of a circle, vertical line, and two
//      horizontal lines about the center of the frame buffer.
//
void efisDrawPlane(void);


// Description:
//      Draw pitch lines at 5 and 10 degree increments.
//
// Input:
//      int16_t pitch:
//          Pitch from -90 to 90 degrees in TRIG16 units.
//
//      int16_t rollSin:
//          Sine of roll angle.  Scaled from zero to TRIG16_ONE.
//
//      int16_t rollCos:
//          Cosine of roll angle.  Scaled from zero to TRIG16_ONE.
//
void efisDrawPitch(int16_t pitch, int16_t rollSin, int16_t rollCos);


// Description:
//      Draw bank indicator.  Consists of a single pointer triangle and
//      bank lines and 0, 10, 20, 30,45, 60 and 90 degrees (as well as
//      their inverses).
//
// Input:
//      int16_t roll:
//          Roll (positive clockwise) from -180 to 180 degrees in TRIG16
//          units.
//
void efisDrawRoll(int16_t roll);


// Description:
//      Draw a single bank indicator line of given length and angle to
//      the frame buffer.
//
//  Input:
//      int16_t theta:
//          Rotation angle bank indicator line.
//
//      uint8_t length:
//          Length of bank indicator line in pixels.
//
void efisDrawRollLine(int16_t theta, uint8_t length);


// Description:
//      Draw artificial horizon to the frame buffer.
//
// Input:
//      int16_t pitch:
//          Pitch from -90 to 90 degrees in TRIG16 units.
//
//      int16_t rollSin:
//          Sine of roll angle.  Scaled from zero to TRIG16_ONE.
//
//      int16_t rollCos:
//          Cosine of roll angle.  Scaled from zero to TRIG16_ONE.
//
void efisDrawHorizon(int16_t pitch, int16_t rollSin, int16_t rollCos);


// Description:
//      Calculate the horizon line ant return it in the given pointers.
//
// Input:
//      int16_t pitch:
//          Pitch from -90 to 90 degrees in TRIG16 units.
//
//      int16_t rollSin:
//          Sine of roll angle.  Scaled from zero to TRIG16_ONE.
//
//      int16_t rollCos:
//          Cosine of roll angle.  Scaled from zero to TRIG16_ONE.
//
//      int16_t *x0_ptr:
//          Pointer to horizontal coordinate of left side of horizon.
//          line.
//
//      int16_t *y0_ptr:
//          Pointer to vertical coordinate of left side of horizon line.
//
//      int16_t *x1_ptr:
//          Pointer to horizontal coordinate of right side of horizon.
//          line.
//
//      int16_t *y1_ptr:
//          Pointer to vertical coordinate of right side of horizon
//          line.
//
bool efisHorizon(int16_t pitch, int16_t rollSin, int16_t rollCos,
                 int16_t *x0_ptr, int16_t *y0_ptr,
                 int16_t *x1_ptr, int16_t *y1_ptr);


// Description:
//      Determine points to draw and whether to draw the sky or ground.
//
//      The two horizon points will always be returned first.
//
//      Inverted from side of line algorithm at:
//          http://math.stackexchange.com/a/274728
//
// Input:
//      uint8_t xa[]:
//          Array to return the horizontal coordinates of points that
//          make up the polygon to draw.  The two horizon points will be
//          returned first.
//
//      uint8_t ya[]:
//          Array to return the vertical coordinates of points that make
//          up the polygon to draw.  The two horizon points will be
//          returned first.
//
//      int16_t *x0_ptr:
//          Pointer to horizontal coordinate of left side of horizon.
//          line.
//
//      int16_t *y0_ptr:
//          Pointer to vertical coordinate of left side of horizon line.
//
//      int16_t *x1_ptr:
//          Pointer to horizontal coordinate of right side of horizon.
//          line.
//
//      int16_t *y1_ptr:
//          Pointer to vertical coordinate of right side of horizon
//          line.
//
// Output (int8_t):
//      Length of xa[] and ya[].  Negative if the ground is to be drawn
//      instead of the sky.
//
int8_t efisPoints(uint8_t xa[], uint8_t ya[],
                  int16_t x0, int16_t y0,
                  int16_t x1, int16_t y1);


// Description:
//      Draw artificial horizon where the drawn polygon requires a 4
//      point polygon.
//
// Input:
//      uint8_t xa[4]:
//          Horizontal coordinates of the points of the drawn polygon.
//          Horizon line is given first.
//
//      uint8_t ya[4]:
//          Vertical coordinates of the points of the drawn polygon.
//          Horizon line is given first.
//
//      uint8_t color:
//          Color (defined only).
//
void efisDrawHorizonAsPolygon(uint8_t xa[4], uint8_t ya[4], uint8_t color);


#endif // EFIS_H
