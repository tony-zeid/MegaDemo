#include <Arduino.h>
#include "config.h"
#include "pots.h"

// Functions for operating 4 potentiometers

// Setup potentiometer pin modees
void pots_setup(){
    pinMode(pot1_pin, INPUT);
    pinMode(pot2_pin, INPUT);
    pinMode(pot3_pin, INPUT);
    pinMode(pot4_pin, INPUT);
}

// Read from potentiometers (10 Bit)
void pots_read(potset *pots){
    pots->pot1 = 1023 - analogRead(pot1_pin);
    pots->pot2 = 1023 - analogRead(pot2_pin);
    pots->pot3 = 1023 - analogRead(pot3_pin);
    pots->pot4 = 1023 - analogRead(pot4_pin);
}