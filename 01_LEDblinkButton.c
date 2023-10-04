#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

// Define the GPIO number for the Red LED
const int RedLED = 16;
volatile sig_atomic_t signal_received = 0;

// Signal handler function for SIGINT (Ctrl+C)
void sigint_handler(int signal) {
    signal_received = signal;
}

int main() {
    // Initialize the GPIO interface
    if (gpioInitialise() == PI_INIT_FAILED) {
        printf("ERROR: Failed to initialize the GPIO interface.\n");
        return 1;
    }

    // Set the mode of the GPIO pin (RedLED) to output
    gpioSetMode(RedLED, PI_OUTPUT);

    // Set up a signal handler for Ctrl+C (SIGINT)
    signal(SIGINT, sigint_handler);

    // Print a message indicating how to exit the program
    printf("Press CTRL-C to exit.\n");

    // Main loop: Blink the Red LED until Ctrl+C is pressed
    while (!signal_received) {
        // Turn on the Red LED
        gpioWrite(RedLED, PI_HIGH);
        
        // Sleep for 1 second
        time_sleep(1);
        
        // Turn off the Red LED
        gpioWrite(RedLED, PI_LOW);
        
        // Sleep for 1 second
        time_sleep(1);
    }

    // Set the mode of the GPIO pin (RedLED) back to input
    gpioSetMode(RedLED, PI_INPUT);

    // Terminate the GPIO interface
    gpioTerminate();

    // Print a newline character for formatting
    printf("\n");

    return 0;
}
