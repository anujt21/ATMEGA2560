// Author:
// Net ID:
// Date:
// Assignment:     Lab 3
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//

#include <Arduino.h>
#include <avr/io.h>
#include "led.h"
#include "switch.h"
#include "timer.h"
#include "lcd.h"

// Connections:
/*
RS  - PB6
E   - PB4

D4  - PA0
D5  - PA1
D6  - PA2
D7  - PA3

LED - PD0-3

Switch  - PB3
*/

/*
 * Define a set of states that can be used in the state machine using an enum.
 */
// typedef enum .......

//Define a set of states that can be used in the state machine using an enum.

typedef enum{
  wait_press, debounce_press, wait_release, debounce_release
}StateType;
volatile StateType buttonState = wait_press; 

volatile unsigned int delay_time = 100;



int main(){

  // set baud rate for the serial port print function
  Serial.begin(9600);

  // Use Arduino library to print to serial port
  Serial.println ("Hello World I am here");

  // flush out buffer so that entire string will print
  Serial.flush();

  // enabe global interrupts
  sei();
  // Setup for LEDs
  initLED();
  // Setup for the switch
  initSwitchPB3();
  // initialize timer 0 and timer 1
  initTimer0();
  initTimer1();
  Serial.println ("Initialization of timers complete");

  initLCD();
  Serial.println ("Init of LCD complete");
  moveCursor(0,0);
  writeString("Blinking rate = ");
  print_state(delay_time);


  /*
  moveCursor(0, 0); // moves the cursor to 0,0 position
  writeString("Current mode: ");
  moveCursor(1, 0);  // moves the cursor to 1,0 position
  writeString("Fast");
  initLCD();
  */


  unsigned int num = 0;
	while (1) {

    // Display num on the LEDs
    turnOnLEDWithChar(num);

    // Increment num
    num++;
    num = num%16;

    // Add delay
    delayMs(delay_time);

    switch(buttonState){
      case wait_press:
      break;

      case debounce_press:
      delayUs(100);
      buttonState = wait_release;
      break;

      case wait_release:
      break;  

      case debounce_release:
      print_state(delay_time);
      delayUs(100);
      // Change text on LED
      buttonState = wait_press;
      break;
    }

	}
  
  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/

ISR(PCINT0_vect){
  if(buttonState == wait_press){
    buttonState = debounce_press;
  }
  else if(buttonState == wait_release){
    delay_time = 300 - delay_time;
    
    buttonState = debounce_release;
  }
}