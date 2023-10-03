#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

// Define the GPIO numbers for the LEDs
const int RedLED = 16;
const int YellowLED = 19;
const int GreenLED = 26;
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

    // Set the GPIO pin modes to output
    gpioSetMode(RedLED, PI_OUTPUT);
    gpioSetMode(YellowLED, PI_OUTPUT);
    gpioSetMode(GreenLED, PI_OUTPUT);

    // Set up a signal handler for Ctrl+C (SIGINT)
    signal(SIGINT, sigint_handler);

    // Print a message indicating how to exit the program
    printf("Press CTRL-C to exit.\n");

    while (!signal_received) {
        // Turn on the Green LED
        gpioWrite(GreenLED, PI_HIGH);
        
        // Sleep for 3 seconds
        time_sleep(3);
        
        // Turn off the Green LED
        gpioWrite(GreenLED, PI_LOW);

        // Blink the Yellow LED three times with 1-second intervals
        for (int i = 0; i < 3; i++) {
            gpioWrite(YellowLED, PI_HIGH);
            time_sleep(1);
            gpioWrite(YellowLED, PI_LOW);
            if (i != 2) {
                time_sleep(1);
            }
        }

        // Turn on the Red LED
        gpioWrite(RedLED, PI_HIGH);
        
        // Sleep for 3 seconds
        time_sleep(3);
        
        // Turn off the Red LED and Yellow LED
        gpioWrite(RedLED, PI_LOW);
        gpioWrite(YellowLED, PI_LOW);
    }

    // Set the GPIO pin modes back to input
    gpioSetMode(RedLED, PI_INPUT);
    gpioSetMode(YellowLED, PI_INPUT);
    gpioSetMode(GreenLED, PI_INPUT);

    // Terminate the GPIO interface
    gpioTerminate();

    // Print a newline character for formatting
    printf("\n");

    return 0;
}
