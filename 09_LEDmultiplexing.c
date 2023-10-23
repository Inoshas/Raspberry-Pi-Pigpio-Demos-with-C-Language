#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

const int PI_DATA = 16;
const int PI_LATCH = 5;
const int PI_CLOCK = 6;

volatile sig_atomic_t signal_received = 0;

void ledZero() {
    // Function to clear all LEDs in the multiplexed display.
    gpioWrite(PI_LATCH, PI_LOW);

    for (int i = 0; i < 8; i++) {
        gpioWrite(PI_DATA, 0); // '0'
        gpioWrite(PI_CLOCK, PI_HIGH);
        time_sleep(0.01);
        gpioWrite(PI_CLOCK, PI_LOW);
    }

    gpioWrite(PI_LATCH, PI_HIGH);
}

void ledSetup(char ledpattern[]) {
    // Function to set the LED pattern based on the binary input `ledpattern`.
    gpioWrite(PI_LATCH, PI_LOW);

    for (int i = 0; i < 8; i++) {
        gpioWrite(PI_DATA, ledpattern[i] - '0');
        gpioWrite(PI_CLOCK, PI_HIGH);
        time_sleep(0.01);
        gpioWrite(PI_CLOCK, PI_LOW);
    }

    gpioWrite(PI_LATCH, PI_HIGH);
}

void sigint_handler(int signal) {
    signal_received = signal;
}

int main() {
    if (gpioInitialise() == PI_INIT_FAILED) {
        printf("ERROR: Failed to initialize the GPIO interface.\n");
        return 1;
    }

    gpioSetMode(PI_DATA, PI_OUTPUT);
    gpioSetMode(PI_LATCH, PI_OUTPUT);
    gpioSetMode(PI_CLOCK, PI_OUTPUT);

    signal(SIGINT, sigint_handler);
    printf("Press CTRL-C to exit.\n");

    while (!signal_received) {
        // Clear all LEDs
        ledZero();
        time_sleep(1);

        // Display pattern '11111111'
        ledSetup("11111111");
        time_sleep(1);

        // Cycle through some patterns
        for (int i = 0; i < 10; i++) {
            ledSetup("10101010");
            time_sleep(0.5);
            ledSetup("01010101");
            time_sleep(0.5);
        }
    }

    // Cleanup and exit
    gpioWrite(PI_LATCH, PI_INPUT);
    gpioWrite(PI_DATA, PI_INPUT);
    gpioWrite(PI_CLOCK, PI_INPUT);

    gpioTerminate();
    printf("\n");
    return 0;
}
