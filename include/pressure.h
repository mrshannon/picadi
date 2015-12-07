////////////////////////////////////////////////////////////////////////
// File: pressure.h
// Author: Michael R. Shannon
// Written: Sunday, December 06, 2015
// Updated: Sunday, December 06, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library is used to talk to the LPS331AP pressure sensor.
//
//      This library currently contains only the minimum macros needed
//      to unselect the chip.
//
////////////////////////////////////////////////////////////////////////

#ifndef PRESSURE_H
#define PRESSURE_H


// Pressur pin select/unselect macros.
#define PRS_SELECT_PIN_OUTPUT() (TRISHbits.TRISH6 = 0)
#define PRS_SELECT() (LATHbits.LATH6 = 0)
#define PRS_DESELECT() (LATHbits.LATH6 = 1)


#endif // PRESSURE_H
