////////////////////////////////////////////////////////////////////////
// File: oled.c
// Header: oled.h
// Author: Michael R. Shannon
// Written: Thursday, November 12, 2015
// Updated: Friday, November 13, 2015
// Device: PIC18F87K22
// Fosc: 10 MHz
// Instruction Clock: Fosc/4
//
// See header file for documentation.
//
////////////////////////////////////////////////////////////////////////


#include "stdint.h"
#include "oled.h"




void oledInit(void){
    
}


void oledInitCom(void){
}


void oledWriteInit(void){
}


void oledSendCommand(uint8_t byte){
}


void oledWriteBuffer(void){
}


void oledNormal(void){
}


void oledInvert(void){
}



////////////////////////////////////////////////////////////////////////
// Frame Buffer
////////////////////////////////////////////////////////////////////////

// Every two pages of the frame buffer needs its own data section.
#pragma idata oled_page01

// Page 0 of PIC side OLED buffer (rows 0-7)
uint8_t oledBuffer_PAGE0[OLED_PAGE_WIDTH] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 40-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 48-55
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 64-71
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 72-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 120-127
};

// Page 1 of PIC side OLED buffer (rows 8-15)
uint8_t oledBuffer_PAGE1[OLED_PAGE_WIDTH] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 40-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 48-55
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 64-71
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 72-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 120-127
};

#pragma idata oled_page23

// Page 2 of PIC side OLED buffer (rows 16-23)
uint8_t oledBuffer_PAGE2[OLED_PAGE_WIDTH] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 40-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 48-55
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 64-71
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 72-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 120-127
};

// Page 3 of PIC side OLED buffer (rows 24-31)
uint8_t oledBuffer_PAGE3[OLED_PAGE_WIDTH] = {    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 40-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 48-55
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 64-71
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 72-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 120-127
};

#pragma idata oled_page45

// Page 4 of PIC side OLED buffer (rows 32-39)
uint8_t oledBuffer_PAGE4[OLED_PAGE_WIDTH] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 40-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 48-55
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 64-71
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 72-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 120-127
};

// Page 5 of PIC side OLED buffer (rows 40-47)
uint8_t oledBuffer_PAGE5[OLED_PAGE_WIDTH] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 40-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 48-55
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 64-71
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 72-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 120-127
};

#pragma idata oled_page67

// Page 6 of PIC side OLED buffer (rows 48-55)
uint8_t oledBuffer_PAGE6[OLED_PAGE_WIDTH] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 40-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 48-55
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 64-71
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 72-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 120-127
};

// Page 7 of PIC side OLED buffer (rows 56-63)
uint8_t oledBuffer_PAGE7[OLED_PAGE_WIDTH] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 0-7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 8-15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 16-23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 24-31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 32-39
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 40-47
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 48-55
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 56-63
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 64-71
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 72-79
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 80-87
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 88-95
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 96-103
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 104-111
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 112-119
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // columns 120-127
};

#pragma idata
////////////////////////////////////////////////////////////////////////
