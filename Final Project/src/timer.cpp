/*
## timer.cpp
1.Implement a precise millisecond timer using timer 1 for switch
debouncing
*/

#include "timer.h"
#include <avr/io.h>

//Global variable for counting ms and us
volatile unsigned int delayCounter = 0;
volatile unsigned int delayCounterUs = 0;

/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
    TCCR1A &= ~( (1 << WGM10) | ( 1<< WGM11));
    TCCR1B |= ( 1 << WGM12);
    TCCR1B &= ~ ( 1 << WGM13);

    // Enable Timer0 Compare Match A Interrupt
    TIMSK1 |= (1 << OCIE1A);
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayMs(unsigned int delay){
    delayCounter = delay;

    // Set Prescalar to 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TCCR1B &= ~(1 << CS12);

    OCR1A = 249;

    // Start timer at 0
    TCNT1 = 0;

    while(delayCounter > 0);

    TCCR1B &= ~( (1 <<  CS10) | (1 <<  CS11) | (1 <<  CS12));
}

ISR(TIMER1_COMPA_vect){
    if(delayCounter > 0){
        delayCounter--;
    }
}