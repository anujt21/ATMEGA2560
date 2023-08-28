// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 1
//----------------------------------------------------------------------//

#include <avr/io.h>

// initSwitch returns void and takes no parameters
void initSwitch(){
  // I'm using the development board header pin 0 (Port E bit0)

  // set direction for input
DDRA &= ~(1 << DDA0);

  // enable the pullup resistor for stable input
PORTA |= (1 << PORTA0);

}
