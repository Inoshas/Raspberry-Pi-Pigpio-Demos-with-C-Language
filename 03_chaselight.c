#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

// Define an array of GPIO pin numbers
const int array[] = {16, 19, 26, 5, 6, 25};
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

    // Set the mode of all pins in the array to output
    for (int i = 0; i < 6; i++) {
        gpioSetMode(array[i], PI_OUTPUT);
    }

    // Set up a signal handler for Ctrl+C (SIGINT)
    signal(SIGINT, sigint_handler);

    // Print a message indicating how to exit the program
    printf("Press CTRL-C to exit.\n");

    while (!signal_received) {
        // Loop through the array of pins
        for (int i = 0; i < 6; i++) {
            // Turn the LED on for 3 seconds
            gpioWrite(array[i], PI_HIGH);
            time_sleep(3);
            // Turn the LED off for 1 second
            gpioWrite(array[i], PI_LOW);
            time_sleep(1);
        }
    }

    // Set the mode of all pins in the array back to input
    for (int i = 0; i < 6; i++) {
        gpioSetMode(array[i], PI_INPUT);
    }

    // Terminate the GPIO interface
    gpioTerminate();

    // Print a newline character for formatting
    printf("\n");

    return 0;
}
