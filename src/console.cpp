#include <Arduino.h>
#include "console.h"

String inputString = "";     // A string to hold incoming data
bool stringComplete = false; // Whether the string is complete

namespace {
// Simple numeric parser to see when input is not a number
bool parseNumber(const String &arg, int &value) {
    if (arg.length() == 0) {
        return false;
    }
    for (unsigned int i = 0; i < arg.length(); i++) {
        if (!isDigit(arg[i])) {
            return false;
        }
    }
    value = arg.toInt();
    return true;
}
}

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
    command.toLowerCase(); // Normalise to lowercase
    if (command == "status") {
        command_status();
    } else if (command == "led") {
        command_led(arg);
    } else if (command == "leds") {
        command_ledfunc(arg, leds_func);
    } else if (command == "disp") {
        command_dispfunc(arg, disp_func);
    } else if (command == "mot") {
        command_motorfunc(arg, motor_func);
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
void command_ledfunc(String arg, int *leds_func){
    int value = 0;
    if (!parseNumber(arg, value)) {
        Serial.println("Please provide a number");
        return;
    }

    switch (value) {
        case 0:
        case 1:
        case 2:
            *leds_func = value;
            Serial.print("LED function: ");
            Serial.println(value);
            break;
        default:
            Serial.println("Unknown argument");
            break;
    }
}

// Sets display function
void command_dispfunc(String arg, int *disp_func){
    int value = 0;
    if (!parseNumber(arg, value)) {
        Serial.println("Please provide a number");
        return;
    }

    switch (value) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            *disp_func = value;
            Serial.print("7SEG function: ");
            Serial.println(value);
            break;
        default:
            Serial.println("Unknown argument");
            break;
    }
}
// Sets motor function
void command_motorfunc(String arg, int *motor_func){
    int value = 0;
    if (!parseNumber(arg, value)) {
        Serial.println("Please provide a number");
        return;
    }

    switch (value) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            *motor_func = value;
            Serial.print("Motor function: ");
            Serial.println(value);
            break;
        default:
            Serial.println("Unknown argument");
            break;
    }
}