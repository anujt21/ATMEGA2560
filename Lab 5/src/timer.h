// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 3
//----------------------------------------------------------------------//

#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include <avr/io.h>

void initTimer0();
void delayMs(unsigned int delay); 
void initTimer1();
void delayUs(unsigned int delay);

#endif
