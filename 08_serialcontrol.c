#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

// Define GPIO pin numbers for LED segments and the common anode (+)
const int data = 6;
const int clock = 25;
const int latch = 24;


int ledState = 0;
const int ON = HIGH;
const int OFF = LOW;



void setup()
{
pinMode(data, OUTPUT);
pinMode(clock, OUTPUT);
pinMode(latch, OUTPUT);
}
void loop()
{
for(int i = 0; i < 256; i++)
{
updateLEDs(i);
delay(500);
}
}
void updateLEDs(int value)
{
digitalWrite(latch, LOW); 
shiftOut(data, clock, MSBFIRST, ~value);
digitalWrite(latch, HIGH); // lock
}




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
