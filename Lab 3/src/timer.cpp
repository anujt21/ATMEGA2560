// Author: 
// Net ID: 
// Date: 
// Assignment: Lab 3
//----------------------------------------------------------------------//

#include "timer.h"

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
void delayUs(unsigned int delay){
    delayCounterUs = delay;

    // turn on clock with the CS bits and start counting
    // Use Prescaleer of 8 (2 counts is  1 us)
    TCCR1B |= (1 << CS11);
    TCCR1B &= ~((1 << CS12)| (1 << CS10));

    OCR1A = 1;

    // Start timer at 0
    TCNT1 = 0;

    // Wait for the interrupt to handle the delay
    while(delayCounterUs > 0);

    TCCR1B &= ~( (1 <<  CS10) | (1 <<  CS11) | (1 <<  CS12));
}

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
    // Set timer to CTC mode
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |=  (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // Enable Timer0 Compare Match A Interrupt
    TIMSK0 |= (1 << OCIE0A);
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
    delayCounter = delay;

    // Set Prescalar to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TCCR0B &= ~(1 << CS02);

    // Set compare value to 249 for 1 ms
    OCR0A = 249;

    // Start timer at 0
    TCNT0 = 0;

    // Wait for the interrupt to handle the delay
    while(delayCounter > 0);

    // Turn off clock
    TCCR0B &= ~( (1 << CS00) | (1 << CS01) | (1 << CS02) );
}

// This is the interrupt handler for Timer0 Compare Match A
ISR(TIMER0_COMPA_vect){
    if(delayCounter > 0){
        delayCounter--;
    }
}

ISR(TIMER1_COMPA_vect){
    if(delayCounterUs > 0){
        delayCounterUs--;
    }
}