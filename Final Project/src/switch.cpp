/*
## switch.cpp
1. Uses a switch to silence the audio chirping alarm.
*/

#include "switch.h"
#include <avr/io.h>

void initSwitch(){
  //set the data direction register to input

  DDRD &= ~(1<<DDD0); // set direction for input
  PORTD |= (1 << PORTD0);  // enable the pullup resistor for stable input
  
  
  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);

  EIMSK |= (1 << INT0);

}
