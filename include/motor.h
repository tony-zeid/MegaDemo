#ifndef MOTOR_H
#define MOTOR_H

// Motor position counter
extern volatile long motor_pos;

// Setup motor pins
void motor_setup();

// Run motor drive
void motor_run(bool dir, unsigned short spd);

// Increment position on pulse interrupt
void pulse_A();
void pulse_B();

#endif