#include <Arduino.h>
#include "config.h"
#include "leds.h"

// Setup LED pins
void leds_setup(){

    // Pin modes for LED array
    pinMode(led1_pin, OUTPUT);
    pinMode(led2_pin, OUTPUT);
    pinMode(led3_pin, OUTPUT);
    pinMode(led4_pin, OUTPUT);

    // Reset LEDs
    leds_reset();
}
 
// Reset LED outputs to OFF
void leds_reset(){
    // Active low set high to disable by default
    analogWrite(led1_pin, HIGH);
    analogWrite(led2_pin, HIGH);
    analogWrite(led3_pin, HIGH);
    analogWrite(led4_pin, HIGH);
    
}

// Drive LED outputs
void leds_driver(LED *LEDs){
    analogWrite(led1_pin, LEDs->led1);
    analogWrite(led2_pin, LEDs->led2);
    analogWrite(led3_pin, LEDs->led3);
    analogWrite(led4_pin, LEDs->led4);
}

// LEDs pulse wave pattern
void leds_pulse_wave(LED *LEDs, unsigned long tnow){
    LEDs->led1 = (((tnow / 4) + 0) % 255) - 50;
    LEDs->led2 = (((tnow / 4) + 40) % 255) - 50;
    LEDs->led3 = (((tnow / 4) + 80) % 255) - 50;
    LEDs->led4 = (((tnow / 4) + 120) % 255) -50;
}

// LEDs some other pattern
void leds_other_pattern(LED *LEDs, unsigned long tnow){
    LEDs->led1 = (tnow + 0)% 500000;
    LEDs->led2 = (tnow + 100)% 500;
    LEDs->led3 = (tnow + 200)% 10000;
    LEDs->led4 = (tnow + 300)% 250;
}