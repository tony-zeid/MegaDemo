#ifndef CONSOLE_H
#define CONSOLE_H

// Initialise console (reserve memory)
void console_init();

// Prase incomming commands
void console_run(int *leds_func, int *disp_func, int *motor_func);

// Comman handler
void handleCommand(String command, String arg, int *leds_func, int *disp_func, int *motor_func);

// Command implementations

// Return status 
void command_status();
// Control built-in LED
void command_led(String arg);
// Sets LED function
void command_ledfunc(String arg, int **leds_func);
// Sets display function
void command_dispfunc(String arg, int **disp_func);
// Sets motor function
void command_motorfunc(String arg, int **motor_func);

#endif