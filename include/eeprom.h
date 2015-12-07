////////////////////////////////////////////////////////////////////////
// File: eeprom.h
// Author: Michael R. Shannon
// Written: Sunday, December 06, 2015
// Updated: Sunday, December 06, 2015
// Device: PIC18F87K22
// Compiler: C18
// Description:
//      This library is used to talk to the 25AA128-I/P EEPROM chip.
//
//      This library currently contains only the minimum macros needed
//      to unselect the chip.
//
////////////////////////////////////////////////////////////////////////


#ifndef EEPROM_H
#define EEPROM_H


// EEPROM pin select/unselect macros.
#define EPM_SELECT_PIN_OUTPUT() (TRISHbits.TRISH4 = 0)
#define EPM_SELECT() (LATHbits.LATH4 = 0)
#define EPM_DESELECT() (LATHbits.LATH4 = 1)


#endif // EEPROM_H
