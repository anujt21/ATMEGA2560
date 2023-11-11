// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPD0(){
  //set the data direction register to input

  DDRD &= ~(1<<DDD0); // set direction for input
  PORTD |= (1 << PORTD0);  // enable the pullup resistor for stable input
  
  
  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);

  EIMSK |= (1 << INT0);

  /*
  DDRB &= ~(1<<DDB3); // set direction for input
  PORTB |= (1 << PORTB3);  // enable the pullup resistor for stable input
  
  PCICR |= (1 << PCIE0);   // enable pin change for PORT B
  PCMSK0 |= (1 << PCINT3);*/
}


void enableSwitch(){
  EIMSK |= (1 << INT0);
}

void disableSwitch(){
  EIMSK &= ~(1 << INT0);
}