#ifndef POTS_H
#define POTS_H

// Struct type to hold potentiometer readings
typedef struct{
    int pot1;
    int pot2;
    int pot3;
    int pot4;
}   potset;

// Setup potentiometer pin modes
void pots_setup();

// Read from potentiometers
void pots_read(potset *pots);

#endif