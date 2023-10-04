#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

const int button = 6;
const int RedLED = 16;
volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
   signal_received = button;
}

int main() {
   if (gpioInitialise() == PI_INIT_FAILED) {
      printf("ERROR: Failed to initialize the GPIO interface.\n");
      return 1;
   }
   
   // Set the button pin as an input
   gpioSetMode(button, PI_INPUT);
   
   // Register the SIGINT (Ctrl+C) handler
   signal(SIGINT, sigint_handler);
   
   printf("Press CTRL-C to exit.\n");
   
   while (!signal_received) {
      // Read the value of the button pin
      int pin_value = gpioRead(button);
      
      if(pin_value == PI_LOW){
         // Debounce the button with a short delay
         time_sleep(0.2);
         
         // Read the state of the Red LED
         gpioSetMode(RedLED, PI_INPUT);
         int red = gpioRead(RedLED);
         
         if(red == PI_LOW){
            // Toggle the Red LED on
            gpioSetMode(RedLED, PI_OUTPUT);
            gpioWrite(RedLED, PI_HIGH);
         } 
         else{
            // Toggle the Red LED off
            gpioSetMode(RedLED, PI_OUTPUT);
            gpioWrite(RedLED, PI_LOW);
         }
      }
   }
 
   // Cleanup GPIO settings and terminate
   gpioSetMode(RedLED, PI_INPUT);
   gpioTerminate();
   printf("\n");
   return 0;
}
