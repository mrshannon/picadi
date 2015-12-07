////////////////////////////////////////////////////////////////////////
// File: dac.h
// Author: Michael R. Shannon
// Written: Sunday, December 06, 2015
// Updated: Sunday, December 06, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library is used to talk to the MCP4812-E/P DAC chip.
//
//      This library currently contains only the minimum macros needed
//      to unselect the chip.
//
////////////////////////////////////////////////////////////////////////

#ifndef DAC_H
#define DAC_H


// SD card pin select/unselect macros.
#define DAC_SELECT_PIN_OUTPUT() (TRISHbits.TRISH3 = 0)
#define DAC_SELECT() (LATHbits.LATH3 = 0)
#define DAC_DESELECT() (LATHbits.LATH3 = 1)


#endif // DAC_H
