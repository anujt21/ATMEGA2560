/*
## switch.cpp
1. Uses a switch to silence the audio chirping alarm.
*/

#include "switch.h"
#include <avr/io.h>

void initSwitch(){
  //set the data direction register to input
  DDRA &= ~(1<<DDA0); // set direction for input (pin 22)
  PORTA |= (1 << PORTA0);  // enable the pullup resistor for stable input
}

void initalarm() {
  // set data direction register for output;
  // use header pin # 31 which corresponds to PORTD0
  DDRC |= (1 << DDC6);
  // turn off alarm initially
  PORTC &= ~(1 << PORTC6);
}

void turnon_alarm() {
  // Write a logic one (5V) to the output of PORTD0
  PORTC |= (1 << PORTC6);
}

void silence_alarm(){
  // Write a logic zero (0V) to the output of PORTD0
  PORTC &= ~(1 << PORTC6);
}
