/*
## timer.cpp
1.Implement a precise millisecond timer using timer 1 for switch
debouncing
*/

#include "timer.h"
#include "pwm.h"
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

void chirpingSound(unsigned int delay){
    delayCounter = delay;

    // Set Prescalar to 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TCCR1B &= ~(1 << CS12);

    OCR1A = 249;

    // Start timer at 0
    TCNT1 = 0;
    turnOnAlarm();
    while(delayCounter > 0){
        
        for(int i = 1000; i < 4000; i++){
            changeFre(i);
        }
    }
    turnOffAlarm();

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
void delayUs(unsigned int delay){
    delayCounter = delay;

    // turn on clock with the CS bits and start counting
    // Use Prescaleer of 8 (2 counts is  1 us)
    TCCR0B |= (1 << CS01);
    TCCR0B &= ~((1 << CS02)| (1 << CS00));

    // Set compare value to 249 for 1 ms
    OCR0A = 1;

    // Start timer at 0
    TCNT0 = 0;

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
    if(delayCounter > 0){
        delayCounter--;
    }
}