#ifndef DIP_SW_H
#define DIP_SW_H

typedef struct{
    bool sw1;
    bool sw2;
    bool sw3;
    bool sw4;
}   DIPSW;

// Setup DIP switch pin modes
void dip_sw_setup();

// Read state of DIP switches
void dip_sw_read(DIPSW *DIP);

#endif