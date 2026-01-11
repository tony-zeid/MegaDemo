#ifndef SEGDISP_H
#define SEGDISP_H

// Struct type for 7-segment characters
typedef struct{
    bool EN;
    bool DP; 
    bool A;
    bool B;
    bool C;
    bool D;
    bool E;
    bool F;
    bool G;
}   seg;

// Struct type for 4 digits
typedef struct{
    int x1;
    int x2;
    int x3;
    int x4;
    int x1dp;
    int x2dp;
    int x3dp;
    int x4dp;
}   dig4;

// Setup 7-segment display 
void segdisp_setup();

// Reset 7-segment display to OFF
void segdisp_reset();

// 7 - Segment Display Decode Digits
void segdisp_decode(seg *segDX, byte digit, bool dp);

// 7-Segment Display Drive Outputs
void segdisp_driver(seg *segD1, seg *segD2, seg *segD3, seg *segD4);

// Individual pots control each digit
void segdisp_pots(dig4 *digs, int pot1, int pot2, int pot3, int pot4);

// Seperate 4 decimal digits for display
void segdisp_dig4(dig4 *digs, int xxxx);

#endif