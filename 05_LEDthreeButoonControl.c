#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

const int button1 = 6;
const int button2 = 26;
const int button3 = 5;
const int LED1 = 22;
const int LED2 = 23;
const int LED3 = 17;
const int BUTTONarray[] = {button1, button2, button3};
const int LEDarray[] = {LED1, LED2, LED3};
volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
   // Set signal_received to the button GPIO number that caused the interrupt
   for(int i = 0; i < 3; i++){
      signal_received = BUTTONarray[i];
   }
}

int main() {
   if (gpioInitialise() == PI_INIT_FAILED) {
      printf("ERROR: Failed to initialize the GPIO interface.\n");
      return 1;
   }
   
   // Set all button GPIO pins as inputs
   for(int i = 0; i < 3; i++){
      gpioSetMode(BUTTONarray[i], PI_INPUT);
   }      
   signal(SIGINT, sigint_handler);
   
   printf("Press CTRL-C to exit.\n");
   
   while (!signal_received) {
      int pin_value[3];
      
      // Read the values of all button GPIO pins
      for(int i = 0; i < 3; i++){
         pin_value[i] = gpioRead(BUTTONarray[i]);
         
         if(pin_value[i] == PI_LOW){
            // Debounce the button with a short delay
            time_sleep(0.2);
            gpioSetMode(LEDarray[i], PI_INPUT);
            int read1 = gpioRead(LEDarray[i]);
            
            if(read1 == PI_LOW){
               // Toggle the LED on
               gpioSetMode(LEDarray[i], PI_OUTPUT);
               gpioWrite(LEDarray[i], PI_HIGH);
            } 
            else{
               // Toggle the LED off
               gpioSetMode(LEDarray[i], PI_OUTPUT);
               gpioWrite(LEDarray[i], PI_LOW);
            }
         }
      }
   }
   
   // Clean up GPIO settings and terminate
   for(int i = 0; i < 3; i++){
      gpioSetMode(LEDarray[i], PI_INPUT);
   }
   gpioTerminate();
   printf("\n");
   return 0;
}
