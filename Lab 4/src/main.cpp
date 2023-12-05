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
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "ssd.h"

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
  wait_press, debounce_press, wait_release, debounce_release, pause
}StateType;


volatile StateType buttonState = wait_press; 


int main(){

  sei();

  Serial.begin(9600);
  Serial.println("Start");
  Serial.flush();
  // Initializations
  
  
  initADC();
  initTimer0();
  initTimer1();
  initPWMTimer3();
  initPWMTimer4();
  seven_segment_init();
  initSwitchPD0();

  
	while (1) {
    unsigned int adc_value = read_adc();
    //Serial.println(adc_value);
    changeDutyCycle(adc_value);

    
    switch(buttonState){
      case wait_press:
      //sei();
      Serial.println("In active");
      //enableSwitch();   // enables INT0
      break;

      case debounce_press:
      delayUs(100);
      Serial.println("In debounce");
      buttonState = wait_release;
      break;

      case wait_release:
      Serial.println("In wait releasse");
      break;

      case debounce_release:
      delayUs(100);
      buttonState = pause;

      case pause:
      //cli();
      disableSwitch();    // disables INT0
      changeDutyCycle(511);
      Serial.println("Starting Countdown");
      countdown();
      enableSwitch();
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

ISR(INT0_vect){
  if(buttonState == wait_press){
    buttonState = debounce_press;
    Serial.println("In ISR now");
  }
  else if(buttonState == wait_release){
    buttonState = debounce_release;
    Serial.println("In ISR now release");
  }
}

/*
ISR(PCINT0_vect){
  if(buttonState == active){
    buttonState = debounce;
  }
}*/