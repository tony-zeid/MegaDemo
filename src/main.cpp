#include <Arduino.h>
#include "config.h"
#include "pots.h"
#include "dip_sw.h"
#include "dpad.h"
#include "imu.h"
#include "leds.h"
#include "segdisp.h"
#include "motor.h"
#include "console.h"

enum LedMode { LED_MODE_OFF = 0, LED_MODE_PULSE = 1, LED_MODE_OTHER = 2 };
enum DispMode { DISP_MODE_OFF = 0, DISP_MODE_POTS = 1, DISP_MODE_POT4_SPLIT = 2, DISP_MODE_MOTOR_DEG = 3, DISP_MODE_LDR_SPLIT = 4, DISP_MODE_ARROWS = 5 };
enum MotorMode {
    MOTOR_MODE_OFF = 0,
    MOTOR_MODE_DPAD_SPEED = 1,
    MOTOR_MODE_LDR_BOOST = 2,
    MOTOR_MODE_SPEED_POT = 3,
    MOTOR_MODE_BIDIR_POT = 4,
    MOTOR_MODE_POSITION_POT = 5,
    MOTOR_MODE_IMU_BALANCE = 6
};

// Select LED array function                    
int leds_func = LED_MODE_PULSE;
// 0 - LEDs OFF
// 1 - Pulse wave 
// 2 - Something else

// Select Display function            
int disp_func = DISP_MODE_POTS;
// 0 - Display OFF
// 1 - Individual pots control each digit
// 2 - Read Pot 4 (0-1023) and split across 4 digits
// 3 - Read Motor Position and display in degrees
// 4 - Read LDR and split across 4 digits
// 5 - Up down arrows increment (positive only)

// Select Motor function     
int motor_func = MOTOR_MODE_OFF;    
// 0 - Motor OFF      
// 1 - Left/Right keys change direction, Up/Down keys adjust speed
// 2 - Left/Right keys change direction, LDR gives speed boost
// 3 - Simple speed control using Pot 4
// 4 - Simple position control using Pot 4
// 5 - Bidirectional speed control using Pot 4
// 6 - Maintain upright position using IMU

// Printout / debug parameters
bool printout_mode = 1;             // 0 - Text | 1 - Graph
unsigned int tprint_text = 1000;    // Printing interval in ms (Serial Analyzer)
unsigned int tprint_graph = 50;     // Printing interval in ms (Serial Monitor)

// Global Variables
unsigned long int tlast = 0;    // Time stamp
unsigned long cycles = 0;       // Cycle count
bool printout = 1;              // Print flag, set to 1 to print errors during setup
float *ERRptr;      // Pointer to IMU error array: Acc(X,Y),Gyr(X,Y,Z)

// Struct instances to hold data
potset pots;    // Pot readings
keyset dpad;    // DPAD counts
DIPSW DIP;      // DIPSW states
LED LEDs;       // LED set of 6
seg segD1;      // 7SEG digit 1
seg segD2;      // 7SEG digit 2
seg segD3;      // 7SEG digit 3
seg segD4;      // 7SEG digit 4
dig4 digs;      // 4 digit block

// Logging helpers keep the main loop shorter 
bool readyToPrint(unsigned long now, unsigned int interval, unsigned long &lastPrint){
    if (now - lastPrint >= interval){
        lastPrint = now;
        return true;
    }
    return false;
}

void printBasics(unsigned long now, unsigned long cycle){
    Serial.println();
    Serial.print("Time: "); Serial.print(now); Serial.print(",\t");
    Serial.print("Cycle: "); Serial.println(cycle);
}

void printAnalogueReadings(int ldr, long motor){
    Serial.print("LDR: "); Serial.print(ldr); Serial.print(",\t");
    Serial.print("Motor: "); Serial.println(motor);
}

void printPotValues(const potset &pots){
    Serial.print("Pot1: "); Serial.print(pots.pot1); Serial.print(",\t");
    Serial.print("Pot2: "); Serial.print(pots.pot2); Serial.print(",\t");        
    Serial.print("Pot3: "); Serial.print(pots.pot3); Serial.print(",\t");
    Serial.print("Pot4: "); Serial.println(abs(512 - pots.pot4));
}

void printDipSwitches(const DIPSW &dip){
    Serial.print("Sw1: "); Serial.print(dip.sw1); Serial.print(",\t");
    Serial.print("Sw2: "); Serial.print(dip.sw2); Serial.print(",\t");  
    Serial.print("Sw3: "); Serial.print(dip.sw3); Serial.print(",\t"); 
    Serial.print("Sw4: "); Serial.println(dip.sw4);
}

void printDpadCounts(const keyset &dpad){
    Serial.print("Left: "); Serial.print(dpad.left); Serial.print(",\t"); 
    Serial.print("Down: "); Serial.print(dpad.down); Serial.print(",\t"); 
    Serial.print("Up: "); Serial.print(dpad.up); Serial.print(",\t"); 
    Serial.print("Right: "); Serial.println(dpad.right);
}

void printIMUValues(const float *imu){
    Serial.print("Roll: "); Serial.print(imu[0]); Serial.print(",\t");
    Serial.print("Pitch: "); Serial.print(imu[1]); Serial.print(",\t");
    Serial.print("Yaw: "); Serial.print(imu[2]); Serial.print(",\t");
    Serial.print("Tdif: "); Serial.println(imu[3]); 
}

void printLedValues(const LED &leds){
    Serial.print("LED1: "); Serial.print(leds.led1); Serial.print(",\t");
    Serial.print("LED2: "); Serial.print(leds.led2); Serial.print(",\t"); 
    Serial.print("LED3: "); Serial.print(leds.led3); Serial.print(",\t"); 
    Serial.print("LED4: "); Serial.println(leds.led4); 
}

// Initialisation Functions 
void setup() {      

    // Communications
    Serial.begin(9600);     // Initalise serial port, 9600 baud rate
    console_init();         // Initialise command console

    // Input Devices
    pinMode(LED_BUILTIN, OUTPUT);       // Enable built-in LED
    pinMode(LDR_pin, INPUT);            // Setup LDR pin as input
    pots_setup();                       // Setup potentiometer pins
    dip_sw_setup();                     // Setup DIP switch pins       
    dpad_setup();                       // Setup Dpad pins   

    // Output Devices
    leds_setup();                       // Setup LEDs
    segdisp_setup();                    // Setup 7-segment display
    motor_setup();                      // Setup motor & encoder

    // Inertial Measurement Unit
    IMU_initialise();           // Initialise I2C comms and reset IMU      
    IMU_sens_config();          // Configures sensitivity of Accel and Gyro        
    ERRptr = IMU_error_calc();  // Measure steady-state errors for compensation      

    // Print message to serial monitor to confirm setup complete
    if (printout == 1 && printout_mode == 0){
        Serial.println("\nSetup complete");
        Serial.print("\n");                                                                                            
        Serial.print("IMU Errors:\n");
        Serial.print("AccX: "); Serial.print(ERRptr[0]); Serial.print(",\t"); 
        Serial.print("AxxY: "); Serial.print(ERRptr[1]); Serial.print(",\t");
        Serial.print("GyrX: "); Serial.print(ERRptr[2]); Serial.print(",\t");
        Serial.print("GyrY: "); Serial.print(ERRptr[3]); Serial.print(",\t");
        Serial.print("GyrZ: "); Serial.print(ERRptr[4]); Serial.print(",\t");
        Serial.print("\n");
    }
  }

// Main Program Loop
void loop() {

    cycles ++;                  // Increment program counter
    const unsigned long now = millis();

    // Read Inputs & Print to Serial
    pots_read(&pots);                       // Read potentiometers 
    dpad_read(&dpad);                       // Read dpad counts 
    dip_sw_read(&DIP);                      // Read DIP switches
    int LDR_val = analogRead(LDR_pin);      // Read input from LDR
    float *IMUptr = IMU_read_data();        // Update IMU measurements, (R,P,Y,T,dT)

    // LED Array Select Function 
    switch(leds_func){
        case LED_MODE_OFF:                 // LEDs OFF
            leds_reset();         
            break;
        case LED_MODE_PULSE:                 // Pulse wave pattern
            leds_pulse_wave(&LEDs, now);
            break;
        case LED_MODE_OTHER:                 // Some other pattern
            leds_other_pattern(&LEDs, now);
            break;
        // Create new pattern functions and add here...
    }
    // LED Array Drive Outputs
    leds_driver(&LEDs);         // Outputs brightness based on pattern

    // 7-Segment Display Select Function
    switch(disp_func){
        case DISP_MODE_OFF:                 // Display OFF
            segdisp_reset();
            break;
        case DISP_MODE_POTS:                 // Individual pots control each digit
            segdisp_pots(&digs, pots.pot1, pots.pot2, pots.pot3, pots.pot4);
            break;
        case DISP_MODE_POT4_SPLIT:                 // Read Pot 4 (0-1023) and display across 4 digits
            segdisp_dig4(&digs, pots.pot4);
            break;
        case DISP_MODE_MOTOR_DEG:                 // Read Motor Position and display in degrees
            segdisp_dig4(&digs, motor_pos*360/1000);
            break;
        case DISP_MODE_LDR_SPLIT:                 // Read LDR and split across 4 digits
            segdisp_dig4(&digs, LDR_val);
            break;
        case DISP_MODE_ARROWS:                 // Up down arrows increment (positive only)
            segdisp_dig4(&digs, dpad.up - dpad.down);
            break;        
    }

    // 7-Segment Display Decode Digits (x4)
    segdisp_decode(&segD1, digs.x1, digs.x1dp);     
    segdisp_decode(&segD2, digs.x2, digs.x2dp);    
    segdisp_decode(&segD3, digs.x3, digs.x3dp);     
    segdisp_decode(&segD4, digs.x4, digs.x4dp);   
    // 7-Segment Display Drive Outputs
    segdisp_driver(&segD1, &segD2, &segD3, &segD4);
    
    // Motor Driver Select Run Mode
    switch(motor_func){
        case MOTOR_MODE_OFF:                 // Motor OFF
            motor_run(0, 0);
            break;
        case MOTOR_MODE_DPAD_SPEED:                 // Left/Right keys change dir, Up/Down keys adjust speed
            motor_run((dpad.right - dpad.left)%2, 50+10*(dpad.up - dpad.down));
            break;
        case MOTOR_MODE_LDR_BOOST:                 // Left/Right keys change dir, LDR gives speed boost
            motor_run((dpad.right - dpad.left)%2, 30+LDR_val);
            break;
        case MOTOR_MODE_SPEED_POT:                 // Simple  speed control using Pot 4 
            motor_run(1, pots.pot4 / 4);
            break;
        case MOTOR_MODE_BIDIR_POT:                 // Bidirectional  speed control using Pot 4 
            motor_run(pots.pot4 > 512, abs(512 - pots.pot4)/2);
            break;
        case MOTOR_MODE_POSITION_POT:                 // Simple  position control using Pot 4 
            motor_run(pots.pot4 > motor_pos, abs(pots.pot4 - motor_pos));
            break;
        case MOTOR_MODE_IMU_BALANCE:                 // Maintain upright position using IMU
            motor_run(-(IMUptr[0]*1000/360) > motor_pos, abs(-(IMUptr[0]*1000/360) - motor_pos));
            break;
        // Create new motor run modes and add here...       
        // motor_run(dir [0-1], spd[0-255])
        }

    // Command Console
    console_run(&leds_func, &disp_func, &motor_func);       // Parse and execute incomming commands

    // Serial Logging of Data Values
    if(printout){
        
        // Verbose text printout for Serial Monitor
        if(printout_mode == 0){
            printBasics(now, cycles);
            printAnalogueReadings(LDR_val, motor_pos);
            printPotValues(pots);
            printDipSwitches(DIP);
            printDpadCounts(dpad);
            printIMUValues(IMUptr);
            printLedValues(LEDs);
        }
        
        // Minimal printout formatted for graphical Serial Analyzer (by CurioRes) 
        if(printout_mode == 1){
            Serial.print(IMUptr[0]);        // Roll
            Serial.print(" ");
            Serial.print(IMUptr[1]);        // Pitch
            Serial.print(" ");
            Serial.print(IMUptr[2]);        // Yaw
            Serial.println();       
        }
    }

    // Decide whether to print serial logs next cycle
    unsigned int interval = (printout_mode == 0) ? tprint_text : tprint_graph;
    printout = readyToPrint(now, interval, tlast);
}