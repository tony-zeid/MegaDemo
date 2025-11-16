#include <Arduino.h>
#include "config.h"
#include "motor.h"

// Motor position counter
volatile long motor_pos = 0;

// Setup motor pins and encoder interrupts
void motor_setup(){
    pinMode(mot_ENA, OUTPUT);
    pinMode(mot_IN1, OUTPUT);
    pinMode(mot_IN2, OUTPUT);
    pinMode(enc_A_pin, INPUT);
    pinMode(enc_B_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(enc_A_pin), pulse_A, RISING);
    attachInterrupt(digitalPinToInterrupt(enc_B_pin), pulse_B, RISING); 
}

// Run motor drive
void motor_run(bool dir, unsigned short spd){
    // Set direction forwards
    if (dir == 1){
        digitalWrite(mot_IN1, 1);
        digitalWrite(mot_IN2, 0);
    }
    // Set direction reverse
    else if (dir == 0){
        digitalWrite(mot_IN1, 0);
        digitalWrite(mot_IN2, 1);
    }
    // Default to stopped
    else{
        digitalWrite(mot_IN1, 0);
        digitalWrite(mot_IN2, 0);
        Serial.println("Direction fault");
    }
    
    // Limit maximum speed
    if (spd > 255) spd = 255;
    // Set minimum speed ()
    if (spd >=15 && spd < 30) spd = 30;
    // Set deadzone (stops idle buzzing)
    if (spd < 15) spd = 0;

    // Drive motor output
    analogWrite(mot_ENA, spd);
}

// Increment position on pulse interrupt
// A-phase interrupt
void pulse_A(){
    if(digitalRead(enc_B_pin) > 0){
        motor_pos++; 
    }
    else{
        motor_pos--;
    }
}

// B-phase interrupt
void pulse_B(){
    if(digitalRead(enc_A_pin) > 0){
        motor_pos--;   
    }
    else{
        motor_pos++;
    }
}
