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

// Select LED array function                    
int leds_func = 1;
// 0 - LEDs OFF
// 1 - Pulse wave 
// 2 - Something else

// Select Display function            
int disp_func = 1;
// 0 - Display OFF
// 1 - Individual pots control each digit
// 2 - Read Pot 4 (0-1023) and split across 4 digits
// 3 - Read Motor Position and display in degrees
// 4 - Read LDR and split across 4 digits
// 5 - Up down arrows increment (positive only)

// Select Motor function     
int motor_func = 0;    
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

    // Read Inputs & Print to Serial
    pots_read(&pots);                       // Read potentiometers 
    dpad_read(&dpad);                       // Read dpad counts 
    dip_sw_read(&DIP);                      // Read DIP switches
    int LDR_val = analogRead(LDR_pin);      // Read input from LDR
    float *IMUptr = IMU_read_data();        // Update IMU measurements, (R,P,Y,T,dT)

    // LED Array Select Function 
    switch(leds_func){
        case 0:                 // LEDs OFF
            leds_reset();         
            break;
        case 1:                 // Pulse wave pattern
            leds_pulse_wave(&LEDs, millis());
            break;
        case 2:                 // Some other pattern
            leds_other_pattern(&LEDs, millis());
            break;
        // Create new pattern functions and add here...
    }
    // LED Array Drive Outputs
    leds_driver(&LEDs);         // Outputs brightness based on pattern

    // 7-Segment Display Select Function
    switch(disp_func){
        case 0:                 // Display OFF
            segdisp_reset();
            break;
        case 1:                 // Individual pots control each digit
            segdisp_pots(&digs, pots.pot1, pots.pot2, pots.pot3, pots.pot4);
            break;
        case 2:                 // Read Pot 4 (0-1023) and display across 4 digits
            segdisp_dig4(&digs, pots.pot4);
            break;
        case 3:                 // Read Motor Position and display in degrees
            segdisp_dig4(&digs, motor_pos*360/1000);
            break;
        case 4:                 // Read LDR and split across 4 digits
            segdisp_dig4(&digs, LDR_val);
            break;
        case 5:                 // Up down arrows increment (positive only)
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
        case 0:                 // Motor OFF
            motor_run(0, 0);
            break;
        case 1:                 // Left/Right keys change dir, Up/Down keys adjust speed
            motor_run((dpad.right - dpad.left)%2, 50+10*(dpad.up - dpad.down));
            break;
        case 2:                 // Left/Right keys change dir, LDR gives speed boost
            motor_run((dpad.right - dpad.left)%2, 30+LDR_val);
            break;
        case 3:                 // Simple  speed control using Pot 4 
            motor_run(1, pots.pot4 / 4);
            break;
        case 4:                 // Bidirectional  speed control using Pot 4 
            motor_run(pots.pot4 > 512, abs(512 - pots.pot4)/2);
            break;
        case 5:                 // Simple  position control using Pot 4 
            motor_run(pots.pot4 > motor_pos, abs(pots.pot4 - motor_pos));
            break;
        case 6:                 // Maintain upright position using IMU
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
            Serial.print("\n");
            Serial.print("Time: "); Serial.print(millis()); Serial.print(",\t");
            Serial.print("Cycle: "); Serial.print(cycles); Serial.print(",\t");  
            Serial.print("\n");
            Serial.print("LDR: "); Serial.print(LDR_val); Serial.print(",\t");
            Serial.print("Motor: "); Serial.print(motor_pos); Serial.print(",\t");
            Serial.print("\n");
            Serial.print("Pot1: "); Serial.print(pots.pot1); Serial.print(",\t");
            Serial.print("Pot2: "); Serial.print(pots.pot2); Serial.print(",\t");        
            Serial.print("Pot3: "); Serial.print(pots.pot3); Serial.print(",\t");
            Serial.print("Pot4: "); Serial.print(abs(512 - pots.pot4)); Serial.print(",\t");
            Serial.print("\n");
            Serial.print("Sw1: "); Serial.print(DIP.sw1); Serial.print(",\t");
            Serial.print("\tSw2: "); Serial.print(DIP.sw2); Serial.print(",\t");  
            Serial.print("\tSw3: "); Serial.print(DIP.sw3); Serial.print(",\t"); 
            Serial.print("\tSw4: "); Serial.print(DIP.sw4); Serial.print(",\t"); 
            Serial.print("\n");
            Serial.print("Left: "); Serial.print(dpad.left); Serial.print(",\t"); 
            Serial.print("Down: "); Serial.print(dpad.down); Serial.print(",\t"); 
            Serial.print("Up: "); Serial.print(dpad.up); Serial.print(",\t"); 
            Serial.print("\tRight: "); Serial.print(dpad.right); Serial.print(",\t"); 
            Serial.print("\n");
            Serial.print("Roll: "); Serial.print(IMUptr[0]); Serial.print(",\t");
            Serial.print("Pitch: "); Serial.print(IMUptr[1]); Serial.print(",\t");
            Serial.print("Yaw: "); Serial.print(IMUptr[2]); Serial.print(",\t");
            Serial.print("Tdif: "); Serial.print(IMUptr[3]); Serial.print(",\t"); 
            Serial.print("\n");
            Serial.print("LED1: "); Serial.print(LEDs.led1); Serial.print(",\t");
            Serial.print("LED2: "); Serial.print(LEDs.led2); Serial.print(",\t"); 
            Serial.print("LED3: "); Serial.print(LEDs.led3); Serial.print(",\t"); 
            Serial.print("LED4: "); Serial.print(LEDs.led4); Serial.print(",\t"); 
            Serial.print("\n");
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
    if(printout_mode == 0){
        if(millis() - tlast >= tprint_text){
            printout = 1;
            tlast = millis();
        }
        else printout = 0;
    }
    if(printout_mode == 1){
        if(millis() - tlast >= tprint_graph){
            printout = 1;
            tlast = millis();
        }
        else printout = 0;
    }
}