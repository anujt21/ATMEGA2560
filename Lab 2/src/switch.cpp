// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3(){
  //set the data direction register to input

  DDRB &= ~(1<<DDB3); // set direction for input
  PORTB |= (1 << PORTB3);  // enable the pullup resistor for stable input
  
  PCICR |= (1 << PCIE0);   // enable pin change for PORT B
  PCMSK0 |= (1 << PCINT3); //enable interrupt for PB3
}
