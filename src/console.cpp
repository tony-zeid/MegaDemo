#include <Arduino.h>
#include "console.h"

String inputString = "";     // A string to hold incoming data
bool stringComplete = false; // Whether the string is complete

void console_init(){
    // Reserve memory to avoid fragmentation
    inputString.reserve(200); 
}

void console_run(int *leds_func, int *disp_func, int *motor_func) {
    if (stringComplete) {
        inputString.trim(); // Remove any newline or extra spaces

        // Parse command and argument
        int spaceIndex = inputString.indexOf(' ');
        String command = (spaceIndex == -1) ? inputString : inputString.substring(0, spaceIndex);
        String argument = (spaceIndex == -1) ? "" : inputString.substring(spaceIndex + 1);

        // Run command handler
        handleCommand(command, argument, leds_func, disp_func, motor_func);

        // Clear the input
        inputString = "";
        stringComplete = false;
    }
    }

// Serial Event: called when new data is available
void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        if (inChar == '\n') {
        stringComplete = true;
        } else {
        inputString += inChar;
        }
    }
}

// Command handler
void handleCommand(String command, String arg, int *leds_func, int *disp_func, int *motor_func) {
    command.toLowerCase(); // Normalize to lowercase
    if (command == "status") {
        command_status();
    } else if (command == "led") {
        command_led(arg);
    } else if (command == "leds") {
        command_ledfunc(arg, &leds_func);
    } else if (command == "disp") {
        command_dispfunc(arg, &disp_func);
    } else if (command == "mot") {
        command_motorfunc(arg, &motor_func);
    } else {
        Serial.println("Unknown command");
    }
}

// Returns system status
void command_status() {
  Serial.println("System is running normally");
}

// Control Built-in LED
void command_led(String arg){
    if (arg == "on") {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED is ON");
    } else if (arg == "off") {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED is OFF");
    } else {
        Serial.println("Unknown argument");
    }
}

// Sets LED array function
void command_ledfunc(String arg, int **leds_func){
    if (arg == "0") {
        **leds_func = 0;
        Serial.print("LED function: ");
        Serial.println(arg);
    } else if (arg == "1") {
        **leds_func = 1;
        Serial.print("LED function: ");
        Serial.println(arg);
    } else if (arg == "2") {
        **leds_func = 2;
        Serial.print("LED function: ");
        Serial.println(arg);
    } else {
        Serial.println("Unknown argument");
    }
}

// Sets display function
void command_dispfunc(String arg, int **disp_func){
    if (arg == "0") {
        **disp_func = 0;
        Serial.print("7SEG function: ");
        Serial.println(arg);
    } else if (arg == "1") {
        **disp_func = 1;
        Serial.print("7SEG function: ");
        Serial.println(arg);
    } else if (arg == "2") {
        **disp_func = 2;
        Serial.print("7SEG function: ");
        Serial.println(arg);
    } else if (arg == "3") {
        **disp_func = 3;
        Serial.print("7SEG function: ");
        Serial.println(arg);
    } else if (arg == "4") {
        **disp_func = 4;
        Serial.print("7SEG function: ");
        Serial.println(arg);
    } else {
        Serial.println("Unknown argument");
    }
}
// Sets motor function
void command_motorfunc(String arg, int **motor_func){
    if (arg == "0") {
        **motor_func = 0;
        Serial.print("Motor function: ");
        Serial.println(arg);
    } else if (arg == "1") {
        **motor_func = 1;
        Serial.print("Motor function: ");
        Serial.println(arg);
    } else if (arg == "2") {
        **motor_func = 2;
        Serial.print("Motor function: ");
        Serial.println(arg);
    } else if (arg == "3") {
        **motor_func = 3;
        Serial.print("Motor function: ");
        Serial.println(arg);
    } else if (arg == "4") {
        **motor_func = 4;
        Serial.print("Motor function: ");
        Serial.println(arg);
    } else if (arg == "5") {
        **motor_func = 5;
        Serial.print("Motor function: ");
        Serial.println(arg);
    } else {
        Serial.println("Unknown argument");
    }
}