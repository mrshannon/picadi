////////////////////////////////////////////////////////////////////////
// File: sdcard.h
// Author: Michael R. Shannon
// Written: Sunday, December 06, 2015
// Updated: Sunday, December 06, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library is used to talk to the 2908-05WB-MG SD card
//      connector.
//
//      This library currently contains only the minimum macros needed
//      to unselect the chip.
//
////////////////////////////////////////////////////////////////////////


#ifndef SDCARD_H
#define SDCARD_H


// SD card pin select/unselect macros.
#define SDC_SELECT_PIN_OUTPUT() (TRISHbits.TRISH0 = 0)
#define SDC_SELECT() (LATHbits.LATH0 = 0)
#define SDC_DESELECT() (LATHbits.LATH0 = 1)


#endif // SDCARD_H
