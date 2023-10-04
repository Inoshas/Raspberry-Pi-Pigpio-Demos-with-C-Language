#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

const int buzzer = 16; // Define the GPIO pin for the buzzer
volatile sig_atomic_t signal_received = 0; // Initialize the signal flag

// Signal handler function for Ctrl+C
void sigint_handler(int signal) {
   signal_received = buzzer; // Set the signal flag to the buzzer GPIO pin number
}

int main() {
   if (gpioInitialise() == PI_INIT_FAILED) {
      printf("ERROR: Failed to initialize the GPIO interface.\n");
      return 1;
   }
   
   signal(SIGINT, sigint_handler); // Register the SIGINT (Ctrl+C) handler
   
   printf("Press CTRL-C to exit.\n");

   gpioSetMode(buzzer, PI_OUTPUT); // Set the buzzer pin as an output
   
   while (!signal_received) {
      // Toggle the buzzer on and off with a 2-second delay
      gpioWrite(buzzer, PI_HIGH); // Turn the buzzer on (high)
      time_sleep(2); // Sleep for 2 seconds
      gpioWrite(buzzer, PI_LOW); // Turn the buzzer off (low)
      time_sleep(2); // Sleep for 2 seconds
   }
   
   gpioSetMode(buzzer, PI_INPUT); // Set the buzzer pin back to input mode
   gpioTerminate(); // Cleanup GPIO settings
   printf("\n");
   return 0;
}
