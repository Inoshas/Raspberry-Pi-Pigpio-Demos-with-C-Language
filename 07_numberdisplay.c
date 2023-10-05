#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

// Define GPIO pin numbers for LED segments and the common anode (+)
const int ledA = 6;
const int ledB = 25;
const int ledC = 24;
const int ledD = 22;
const int ledE = 17;
const int ledF = 23;
const int ledG = 27;
const int ledDP = 5;

// Define an array to represent the LED segment patterns for numbers 0-9
const int LEDarr[10][8] = {
   {ledA, ledB, ledC, ledD, ledE, ledF, ledDP, 0},
   {ledB, ledC, ledDP, 0, 0, 0, 0, 0},
   {ledA, ledB, ledD, ledG, ledE, ledDP, 0, 0},
   {ledA, ledB, ledC, ledD, ledG, ledDP, 0, 0},
   {ledB, ledC, ledF, ledG, ledDP, 0, 0, 0},
   {ledA, ledC, ledD, ledG, ledF, ledDP, 0, 0},
   {ledC, ledD, ledE, ledF, ledG, ledDP, 0, 0},
   {ledA, ledB, ledC, ledDP, 0, 0, 0, 0},
   {ledA, ledB, ledC, ledD, ledE, ledF, ledG, ledDP},
   {ledA, ledB, ledC, ledF, ledG, ledDP, 0, 0}
};

// Define an array of individual LED segments
const int LEDarray[] = {ledA, ledB, ledC, ledD, ledE, ledF, ledG, ledDP};

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
   signal_received = signal;
}

int main() {
   if (gpioInitialise() == PI_INIT_FAILED) {
      printf("ERROR: Failed to initialize the GPIO interface.\n");
      return 1;
   }

   signal(SIGINT, sigint_handler); // Register the SIGINT (Ctrl+C) handler

   printf("Press CTRL-C to exit.\n");

   // Set all LED segment pins as outputs
   for (int i = 0; i < 8; i++) {
      gpioSetMode(LEDarray[i], PI_OUTPUT);
   }

   while (!signal_received) {
      // Display numbers 0-9 on a 7-segment display with a 1-second delay
      for (int i = 0; i < 10; i++) {
         for (int j = 0; j < 8; j++) {
            gpioWrite(LEDarr[i][j], PI_HIGH); // Turn on LED segment
         }

         time_sleep(1); // Sleep for 1 second

         for (int j = 0; j < 8; j++) {
            gpioWrite(LEDarr[i][j], PI_LOW); // Turn off LED segment
         }

         time_sleep(1); // Sleep for 1 second
      }
   }

   // Set all LED segment pins back to input mode and terminate GPIO
   for (int i = 0; i < 8; i++) {
      gpioSetMode(LEDarray[i], PI_INPUT);
   }

   gpioTerminate(); // Cleanup GPIO settings
   printf("\n");
   return 0;
}
