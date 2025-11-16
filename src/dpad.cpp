#include <Arduino.h>
#include "config.h"
#include "dpad.h"

// Functions for operating 4 directional keys

// Define variables needed for switch debouncing
unsigned keyLEFT_last = 0;
unsigned keyDOWN_last = 0;
unsigned keyUP_last = 0;
unsigned keyRIGHT_last = 0;
bool flagLEFT = 0;
bool flagDOWN = 0;
bool flagUP = 0;
bool flagRIGHT = 0;

// Setup dpad pin modes, pullup resistors required
void dpad_setup(){
    pinMode(keyLEFT_pin, INPUT_PULLUP);
    pinMode(keyDOWN_pin, INPUT_PULLUP);
    pinMode(keyUP_pin, INPUT_PULLUP);
    pinMode(keyRIGHT_pin, INPUT_PULLUP);
}

void dpad_read(keyset *dpad){
    // Define variables to store Boolean state of direction keys
    bool keyLEFT_state = digitalRead(keyLEFT_pin);
    bool keyDOWN_state = digitalRead(keyDOWN_pin);
    bool keyUP_state = digitalRead(keyUP_pin);
    bool keyRIGHT_state = digitalRead(keyRIGHT_pin);

    // Record time of last HIGH state for debouncing, resets flags
    if(keyLEFT_state == 1) {
    keyLEFT_last = millis();
    flagLEFT = 0;
    }
    if(keyDOWN_state == 1) {
    keyDOWN_last = millis();
    flagDOWN = 0;
    }
    if(keyUP_state == 1) {
    keyUP_last = millis();
    flagUP = 0;
    }
    if(keyRIGHT_state == 1) {
    keyRIGHT_last = millis();
    flagRIGHT = 0;
    }

    // Increments count on falling edge after 40ms debounce time, flag prevents multiple counts
    if(keyLEFT_state == 0 && (millis() - keyLEFT_last > 40) && flagLEFT == 0) {
    ++dpad->left;
    flagLEFT = 1;
    }
    if(keyDOWN_state == 0 && (millis() - keyDOWN_last > 40) && flagDOWN == 0) {
    ++dpad->down;
    flagDOWN = 1;
    }
    if(keyUP_state == 0 && (millis() - keyUP_last > 40) && flagUP == 0) {
    ++dpad->up;
    flagUP = 1;
    }
    if(keyRIGHT_state == 0 && (millis() - keyRIGHT_last > 40) && flagRIGHT == 0) {
    ++dpad->right;
    flagRIGHT = 1;
    }
}