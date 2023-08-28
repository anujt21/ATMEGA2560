// This code sequences through 3 leds using the short delay when the button is pushed
// When the button is not pushed, the leds all light up and turn off using the long delay 
// Anuj Tripathi
// ECE 372 example code


#include <Arduino.h>
#include <avr/io.h>
#include "led.h"
#include "switch.h"
#define SHORT_DELAY 100
#define LONG_DELAY 1000

int main() {

  ////////////////////////////////////////////

  // showing use of serial print to the serial monitor for debugging only
  // set baud rate for the serial port print function
  Serial.begin(9600);

  // Use Arduino library to print to serial port
  Serial.println ("Hello World I am here");
  Serial.println ("I am still here");
  // flush out buffer so that entire string will print
  Serial.flush();

  ////////////////////////////////////////////

  //initialize LEDS
  // set direction of LED pins

  initLEDs ();

  //initialize switch
  // set the switch for input
  initSwitch ();

  // set a variable count for the LEDs to the first value

  unsigned int LEDnum = 0;

  while (1) {

    // poll the switch
    // if switch is pressed SHORT_DELAY
    if (! (PINA & ( 1 << PINA0))){
      // Turns on the LEDs at the required positions, and turns off the previous ones
      runLED(LEDnum);

      // delay for 1000 ms before doing the next sequence
      _delay_ms (LONG_DELAY);

      // Moves on to the next position, and then resets after it gets 7
      if (LEDnum==3){
        LEDnum=5;
      }
      else if(LEDnum==7){
        LEDnum=1;
      }
      else{
        LEDnum++;
      }
      //LEDnum = (LEDnum+1)%8;

    }
    // if the switch is open 
    else{
      // Turns on the LEDs at the required positions, and turns off the previous ones
      runLED(LEDnum);

      // delay for 100 ms before doing the next sequence
      _delay_ms (SHORT_DELAY);
      
      // Moves on to the next position, and then resets after it gets 7
      if (LEDnum==3){
        LEDnum=5;
      }
      else if(LEDnum==7){
        LEDnum=1;
      }
      else{
        LEDnum++;
      }
    }
  }
  return 0;

}

