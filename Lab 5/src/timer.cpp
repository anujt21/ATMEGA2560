/*
## timer.cpp
1.Implement a precise millisecond timer using timer 1 for switch
debouncing
*/

#include "timer.h"
#include <avr/io.h>

/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){ 
    TCCR1A &= ~ (1 << WGM10);
    TCCR1A &= ~ (1 << WGM11);
    TCCR1B |=   (1 << WGM12);
    TCCR1B &= ~ (1 << WGM13);
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayMs(unsigned int delay){
    unsigned int count = 0;
    // set outout compare value
    OCR0A = 249;

    // turn on clock with the CS bits and start counting
    // Use Prescaleer of 8
    TCCR0B |=  (1 << CS00);
    TCCR0B |=  (1 << CS01);
    TCCR0B &= ~(1 << CS02);


    // set TCNT0 = 0
    TCNT0 = 0;

    // set output compare flag  TIFR0 down by writing a logic 1
    TIFR0 |= (1 << OCF0A);

    

    // poll the flag OCF1A bit to see when it is raised
     while (count < delay){
        TIFR0 |= (1 << OCF0A);
        TCNT0 = 0;
        // while the flag bit OCF1A is down , do nothing
        while (( TIFR0 & ( 1 << OCF0A)) == 0) ;
        count++;
    }
    
    // exit out we have our delay required
    // turn off clock
    TCCR0B &= ~(1 << CS02);
    TCCR0B &= ~(1 << CS01); 
    TCCR0B &= ~(1 << CS00);
}
