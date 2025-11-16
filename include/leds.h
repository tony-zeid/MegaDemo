#ifndef LEDS_H
#define LEDS_H

// Struct type to hold LED brightnesses
typedef struct{
    short led1;
    short led2;
    short led3;
    short led4;
}   LED;


// Setup LEDs pins
void leds_setup();

// Drive LED outputs
void leds_driver(LED *LEDs);

// LED pulse wave patter
void leds_pulse_wave(LED *LEDs, unsigned long tnow);

void leds_other_pattern(LED *LEDs, unsigned long tnow);

#endif