# Hardware Demonstration Board for Arduino Mega

## Overview
This project is a simple hardware demonstration board for the **Arduino Mega**. It lets beginners try common inputs and outputs: four potentiometers, an LDR, DIP switches, direction keys, an MPU6050 IMU, a motor with encoder (L298D driver), four LEDs, and a four-digit seven-segment display. All pin definitions live in `config.h` so wiring changes only need one edit.

## Getting started
1) Install PlatformIO (VS Code extension or CLI).
2) Connect the Arduino Mega via USB.
3) Build: `platformio run`
4) Upload: `platformio run -t upload`
5) Open a serial monitor at **9600 baud** to see output and send commands.

## Using the serial console
Commands are lowercase. Examples:
- `status` — print a simple status line.
- `led on` / `led off` — toggle the built-in LED.
- `leds <mode>` — set LED pattern (0 off, 1 pulse, 2 other).
- `disp <mode>` — set display mode (0 off, 1 pots, 2 pot4 split, 3 motor degrees, 4 LDR split, 5 arrows).
- `mot <mode>` — set motor mode (0 off, 1 d-pad speed, 2 LDR boost, 3 speed pot, 4 bidirectional pot, 5 position pot, 6 IMU balance).

## Logging modes
- `printout_mode = 0` (text): verbose human-readable lines at `tprint_text` ms.
- `printout_mode = 1` (graph): minimal roll/pitch/yaw values for plotting at `tprint_graph` ms.

## Hardware devices

### Input devices
- **Potentiometers** (4×) — analogue inputs for variable values.
- **LDR** — analogue light sensor.
- **DIP switches** (4×) — digital toggles.
- **Direction keys** (4×) — digital Up/Down/Left/Right.
- **MPU6050** — I²C accelerometer/gyroscope.
- **Motor encoder** (2×) — counts motor rotation.

### Output devices
- **LEDs** (4×) — visual feedback.
- **7-segment display** (4 digits) — numeric display.
- **Motor** — driven via L298D.

## Pin configuration

### Input devices

| Device | Variable | Pin |
|--------|----------|-----|
| Pot 1 | pot1_pin | A0 |
| Pot 2 | pot2_pin | A1 |
| Pot 3 | pot3_pin | A2 |
| Pot 4 | pot4_pin | A3 |
| LDR | LDR_pin | A7 |
| Switch 1 | sw1_pin | 46 |
| Switch 2 | sw2_pin | 47 |
| Switch 3 | sw3_pin | 48 |
| Switch 4 | sw4_pin | 49 |
| Left key | keyLEFT_pin | 50 |
| Down key | keyDOWN_pin | 51 |
| Up key | keyUP_pin | 52 |
| Right key | keyRIGHT_pin | 53 |
| IMU data | imu_sda | 20 |
| IMU clock | imu_scl | 21 |
| Encoder A phase | enc_A_pin | 18 |
| Encoder B phase | enc_B_pin | 19 |

### Output devices

| Device | Variable | Pin |
|--------|----------|-----|
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
| Decimal point | dispDP | 33 |
| Digit 1 | dispD1 | 22 |
| Digit 2 | dispD2 | 23 |
| Digit 3 | dispD3 | 24 |
| Digit 4 | dispD4 | 25 |
| Motor enable | mot_ENA | 45 |
| Motor input 1 | mot_IN1 | 38 |
| Motor input 2 | mot_IN2 | 39 |

