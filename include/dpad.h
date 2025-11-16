#ifndef DPAD_H
#define DPAD_H

// Struct type to hold dpad counts
typedef struct{
    unsigned left;
    unsigned down;
    unsigned up;
    unsigned right;
}   keyset;

// Setup dpad pin modes
void dpad_setup();

// Read dpad counts
void dpad_read(keyset *dpad);

#endif