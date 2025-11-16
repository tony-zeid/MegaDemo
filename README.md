# Hardware Demonstration Board for Arduino Mega

## Overview
This project is a simple hardware demonstration board for the **Arduino Mega**. It showcases basic input/output devices to help understand common hardware interfacing. The project includes potentiometers, LEDs, DIP switches, push buttons, a 7-segment display, an IMU and motor with rotary encoder, driven via L298D.

All hardware connections are specified in the `config.h` file, making it easy to modify the connections if necessary.

## Hardware Devices

### Input Devices
- **Potentiometers** (4x) - Analog inputs to simulate varying values.
- **LDR** - Light Dependent Resistor (analog input for light sensing).
- **DIP Switches** (4x) - Digital inputs for toggling settings.
- **Direction Keys** (4x) - Digital inputs (Up, Down, Left, Right).
- **MPU6050** - I2C accelerometer/gyroscope.
- **Motor Encoder** (2x) - Used to measure motor rotation.

### Output Devices
- **LEDs** (4x) - Visual feedback or status indicators.
- **7-Segment Display** (4x) - Numeric display.
- **Motor** - Controlled via motor driver pins.

## Pin Configuration

### Input Devices

| Device | Variable | Pin |
|--------|--------|--------|
| Pot 1 | pot1_pin | A0 |
| Pot 2 | pot2_pin | A1 |
| Pot 3 | pot3_pin | A2 |
| Pot 4 | pot4_pin | A3 |
| LDR | resLD_pin | A7 |
| Switch 1 | sw1_pin | 46 |
| Switch 2 | sw2_pin | 47 |
| Switch 3 | sw3_pin | 48 |
| Switch 4 | sw4_pin | 49 |	
| Left Key | keyLEFT_pin | 50 |
| Down Key | keyDOWN_pin | 51 |
| Up Key | keyUP_pin | 52 |
| Right Key | keyRIGHT_pin | 53 |
| IMU Data | imu_sda | 20 |
| IMU Clock | imu_scl | 21 |
| Encoder A Phase | enc_A_pin | 18 |
| Encoder B Phase | enc_B_pin | 19 |

## Output Devices

| Device | Variable | Pin |
|--------|--------|--------|
| LED 1 | led1_pin | 12 |
| LED 2 | led2_pin | 11 |
| LED 3 | led3_pin | 10 |
| LED 4 | led4_pin | 9 |
| Segment A | dispA | 26 |
| Segment B | dispB | 27 |
| Segment C | dispC | 28 |
| Segment D | dispD | 29 |
| Segment E | dispE | 30 |
| Segment F | dispF | 31 |
| Segment G | dispG | 32 |
| Decimal Point | dispDP | 33 |
| Digit 1 | dispD1 | 22 |
| Digit 2 | dispD2 | 23 |
| Digit 3 | dispD3 | 24 |
| Digit 4 | dispD4 | 25 |
| Motor Enable | mot_ENA | 45 |
| Motor Input 1 | mot_IN1 | 38 |
| Motor Input 2 | mot_IN2 | 39 |

