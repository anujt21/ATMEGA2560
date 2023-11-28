/*
## switch.cpp
1. Uses a switch to silence the audio chirping alarm.
*/

#include "switch.h"
#include <avr/io.h>

void initSwitch(){
  //set the data direction register to input
  DDRD &= ~(1<<DDD2);      // set direction for input 
  PORTD |= (1 << PORTD2);  // enable the pullup resistor for stable input

  EICRA |= (1 << ISC20);

  EIMSK |= (1 << INT2);

}
