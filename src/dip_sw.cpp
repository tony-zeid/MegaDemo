#include <Arduino.h>
#include "config.h"
#include "dip_sw.h"

// Setup DIP switch pin modes, pullup resistors required
void dip_sw_setup(){
    pinMode(sw1_pin, INPUT_PULLUP);
    pinMode(sw2_pin, INPUT_PULLUP);
    pinMode(sw3_pin, INPUT_PULLUP);
    pinMode(sw4_pin, INPUT_PULLUP);
}

// Read state of DIP switches
void dip_sw_read(DIPSW *DIP){
    DIP->sw1 = digitalRead(sw1_pin);
    DIP->sw2 = digitalRead(sw2_pin);
    DIP->sw3 = digitalRead(sw3_pin);
    DIP->sw4 = digitalRead(sw4_pin);
}