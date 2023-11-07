#include <avr/io.h>
#include "timer.h"


// Initialize timer 0 to debounce states
void initTimer0(){
    //for CTC mode WGM00 = 0, WGM01 = 1, WGM02 = 0
    TCCR0A &= ~( 1 << WGM00); //setting WGM00 to 0
    TCCR0A |=  ( 1 << WGM01); //setting WGM01 to 1
    TCCR0B &= ~( 1 << WGM02); //setting WGM02 to 0
}

void delayMs(unsigned int delay){
    unsigned int count = 0;
    // set outout compare value
    OCR0A = 249;

    // turn on clock with the CS bits and start counting
    // Use Prescaleer of 8
    TCCR0B &= ~(1 << CS00);
    TCCR0B |=  (1 << CS01);
    TCCR0B &= ~(1 << CS02);


    // set TCNT0 = 0; start timer at 0
    TCNT0 = 0;

    // set output compare flag TIFR0 down by writing a logic 1
    TIFR0 |= (1 << OCF0A);

     
    // while the flag bit OCF0A is down , do nothing
    while (count < delay){
        while (!( TIFR0 & ( 1 << OCF0A))) ;
        count ++;
    }
    
    // exit out we have our delay required
    // turn off clock
    TCCR0B &= ~(1 << CS02);
    TCCR0B &= ~(1 << CS01); 
    TCCR0B &= ~(1 << CS00);
}

/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){ 
    TCCR1A &= ~ (1 << WGM10);
    TCCR1A &= ~ (1 << WGM11);
    TCCR1B |=   (1 << WGM12);
    TCCR1B &= ~ (1 << WGM13);
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delaySec(unsigned int delay){
    unsigned int count = 0;

    // set outout compare value with 16MHz
    OCR1A = 15624;
    
    TIMSK1 = (1 << OCIE1A); // Enable compare match interrupt

    // turn on clock with the CS bits and start counting
    // Use Prescaleer of 256
    TCCR1B |= (1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS10);

    // set TCNT1 = 0
    TCNT1 = 0;

    // set output compare flag down by writing a logic 1
    TIFR1 |= (1 << OCF1A);

    // poll the flag OCF1A bit to see when it is raised    
    while (count < delay){
        TIFR1 |= (1 << OCF1A);
        
        TCNT1 = 0;
        // while the flag bit OCF1A is down , do nothing
        while (!( TIFR1 & ( 1 << OCF1A))) ;
        count++;
    }
    
    // exit out we have our delay required
    // turn off clock
    TCCR1B &= ~(1 << CS12);
    TCCR1B &= ~(1 << CS11); 
    TCCR1B &= ~(1 << CS10);
}