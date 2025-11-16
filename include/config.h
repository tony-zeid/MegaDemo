// Define pin numbers based on connections. This allows us to reference inputs and 
// outputs by given names. If connections change, we only need to change this file

#ifndef CONFIG_H
#define CONFIG_H

/* ======================== Input Devices ======================== */

// Define pins for potentiometers
#define pot1_pin A0
#define pot2_pin A1
#define pot3_pin A2
#define pot4_pin A3

// Define pin for LDR
#define resLD_pin A7

// Define pins for DIP switches
#define sw1_pin 46
#define sw2_pin 47
#define sw3_pin 48
#define sw4_pin 49

// Define pins for direction keys
#define keyLEFT_pin 50
#define keyDOWN_pin 51
#define keyUP_pin 52
#define keyRIGHT_pin 53

// Define pins for MPU6050
#define imu_sda 20
#define imu_scl 21

// Define pins for motor encoder
#define enc_A_pin 18
#define enc_B_pin 19

/* ======================== Output Devices ======================== */

// Define pins for LEDs
#define led1_pin 12
#define led2_pin 11
#define led3_pin 10
#define led4_pin 9

// Define pins for 7-segment display
// Segments (drive HIGH to enable)
#define dispA 26
#define dispB 27
#define dispC 28
#define dispD 29
#define dispE 30
#define dispF 31
#define dispG 32
#define dispDP 33
// Digits (drive LOW to enable)
#define dispD1 22
#define dispD2 23
#define dispD3 24
#define dispD4 25

// Define pins for motor
#define mot_ENA 45
#define mot_IN1 38
#define mot_IN2 39

#endif