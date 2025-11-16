#include <Arduino.h>
#include "config.h"
#include "segdisp.h"

// Setup 7-segment display pins
void segdisp_setup(seg *segD1, seg *segD2, seg *segD3, seg *segD4){

    // Pin modes for 7-segment display
    pinMode(dispA, OUTPUT);
    pinMode(dispB, OUTPUT);
    pinMode(dispC, OUTPUT);
    pinMode(dispD, OUTPUT);
    pinMode(dispE, OUTPUT);
    pinMode(dispF, OUTPUT);
    pinMode(dispG, OUTPUT);
    pinMode(dispDP, OUTPUT);
    pinMode(dispD1, OUTPUT);
    pinMode(dispD2, OUTPUT);
    pinMode(dispD3, OUTPUT);
    pinMode(dispD4, OUTPUT);

    // Active low set high to disable by default
    digitalWrite(dispD1, HIGH);
    digitalWrite(dispD2, HIGH);
    digitalWrite(dispD3, HIGH);   
    digitalWrite(dispD4, HIGH);
}

// Decimal to 7-segment decoder
void segdisp_decode(seg *segDX, byte digit, bool dp){       // (ptr, digit, DP)
    // Set whether to display decimal point
    if(dp == 0) segDX->DP = 0;
    else if(dp == 1) segDX->DP = 1;

    // Set segments state based on desired digit to display
    switch(digit){
        case 0:     // Zero
            segDX->A = 1;
            segDX->B = 1;
            segDX->C = 1;
            segDX->D = 1;
            segDX->E = 1;
            segDX->F = 1;
            segDX->G = 0;
            break;
        case 1:     // One
            segDX->A = 0;
            segDX->B = 1;
            segDX->C = 1;
            segDX->D = 0;
            segDX->E = 0;
            segDX->F = 0;
            segDX->G = 0;
            break;
        case 2:     // Two
            segDX->A = 1;
            segDX->B = 1;
            segDX->C = 0;
            segDX->D = 1;
            segDX->E = 1;
            segDX->F = 0;
            segDX->G = 1;
            break;
        case 3:     // Three
            segDX->A = 1;
            segDX->B = 1;
            segDX->C = 1;
            segDX->D = 1;
            segDX->E = 0;
            segDX->F = 0;
            segDX->G = 1;
            break;
        case 4:     // Four
            segDX->A = 0;
            segDX->B = 1;
            segDX->C = 1;
            segDX->D = 0;
            segDX->E = 0;
            segDX->F = 1;
            segDX->G = 1;
            break;
        case 5: // Five
            segDX->A = 1;
            segDX->B = 0;
            segDX->C = 1;
            segDX->D = 1;
            segDX->E = 0;
            segDX->F = 1;
            segDX->G = 1;
            break;
        case 6:     // Six
            segDX->A = 1;
            segDX->B = 0;
            segDX->C = 1;
            segDX->D = 1;
            segDX->E = 1;
            segDX->F = 1;
            segDX->G = 1;
            break;
        case 7:     // Seven
            segDX->A = 1;
            segDX->B = 1;
            segDX->C = 1;
            segDX->D = 0;
            segDX->E = 0;
            segDX->F = 0;
            segDX->G = 0;
            break;
        case 8:     // Eight
            segDX->A = 1;
            segDX->B = 1;
            segDX->C = 1;
            segDX->D = 1;
            segDX->E = 1;
            segDX->F = 1;
            segDX->G = 1;
            break;
        case 9:     // Nine
            segDX->A = 1;
            segDX->B = 1;
            segDX->C = 1;
            segDX->D = 1;
            segDX->E = 0;
            segDX->F = 1;
            segDX->G = 1;
            break;
    } 
}

// 7-Segment Display Drive Outputs
void segdisp_driver(seg *segD1, seg *segD2, seg *segD3, seg *segD4){

    // First Digit 
    // Set enable bit (active low)
    digitalWrite(dispD1, segD1->EN);
    // Drive display segments
    digitalWrite(dispA, segD1->A);
    digitalWrite(dispB, segD1->B);
    digitalWrite(dispC, segD1->C);
    digitalWrite(dispD, segD1->D);
    digitalWrite(dispE, segD1->E);
    digitalWrite(dispF, segD1->F);
    digitalWrite(dispG, segD1->G);
    digitalWrite(dispDP, segD1->DP);
    delay(2);
    // Set enable bit (active low)
    digitalWrite(dispD1, HIGH);
    
    // Second Digit
    // Set enable bit (active low)
    digitalWrite(dispD2, segD2->EN);
    // Drive display segments
    digitalWrite(dispA, segD2->A);
    digitalWrite(dispB, segD2->B);
    digitalWrite(dispC, segD2->C);
    digitalWrite(dispD, segD2->D);
    digitalWrite(dispE, segD2->E);
    digitalWrite(dispF, segD2->F);
    digitalWrite(dispG, segD2->G);
    digitalWrite(dispDP, segD2->DP);
    delay(2);
    // Set enable bit (active low)
    digitalWrite(dispD2, HIGH);
    
    // Third Digit
    // Set enable bit (active low)
    digitalWrite(dispD3, segD3->EN);
    // Drive display segments
    digitalWrite(dispA, segD3->A);
    digitalWrite(dispB, segD3->B);
    digitalWrite(dispC, segD3->C);
    digitalWrite(dispD, segD3->D);
    digitalWrite(dispE, segD3->E);
    digitalWrite(dispF, segD3->F);
    digitalWrite(dispG, segD3->G);
    digitalWrite(dispDP, segD3->DP);
    delay(2);
    // Set enable bit (active low)
    digitalWrite(dispD3, HIGH);
    
    // Fourth Digit
    // Set enable bit (active low)
    digitalWrite(dispD4, segD4->EN);
    // Drive display segments
    digitalWrite(dispA, segD4->A);
    digitalWrite(dispB, segD4->B);
    digitalWrite(dispC, segD4->C);
    digitalWrite(dispD, segD4->D);
    digitalWrite(dispE, segD4->E);
    digitalWrite(dispF, segD4->F);
    digitalWrite(dispG, segD4->G);
    digitalWrite(dispDP, segD4->DP);
    delay(2);
    // Set enable bit (active low)
    digitalWrite(dispD4, HIGH);
}

// Individual pots control each digit
void segdisp_pots(dig4 *digs, int pot1, int pot2, int pot3, int pot4){
    // Each pot controls digits 0-9 by scaling down full range
    digs->x1 = ((pot1*10) / 1024); 
    digs->x2 = ((pot2*10) / 1024); 
    digs->x3 = ((pot3*10) / 1024);  
    digs->x4 = ((pot4*10) / 1024);  
}

//  Take 4 digit number and split up for display
void segdisp_dig4(dig4 *digs, int xxxx){
    // 4 digits show full range of 4 0-1023
    digs->x1 = (xxxx / 1000) % 10;  // Thousand
    digs->x2 = (xxxx / 100) % 10;   // Hundreds
    digs->x3 = (xxxx / 10) % 10;    // Tens
    digs->x4 = xxxx % 10;           // Units
}