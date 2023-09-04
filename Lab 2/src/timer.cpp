// Description: This file implements functions that utilize the timers
//----------------------------------------------------------------------//

#include "timer.h"

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
    // Set timer to CTC mode
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |=  (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // Set Prescalar to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TCCR0B &= ~(1 << CS02);

    // Set compare value to 249 for 1 ms
    OCR0A = 249;
}

// initialize mode of operation for Timer1 using CTC mode
// WGM bits need configured

void initTIMER1() {
    TCCR1A &= ~( (1 << WGM10) | ( 1<< WGM11));
    TCCR1B |= ( 1 << WGM12);
    TCCR1B &= ~ ( 1 << WGM13);

    // turn on clock with the CS bits and start counting
    // Use Prescaleer of 8 (2 counts is  1 us)
    TCCR1B |= (1 << CS11);
    TCCR1B &= ~((1 << CS12)| (1 << CS10));

    OCR1A = 1;
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-200 milliseconds
*/
void delayMs(unsigned int delay){
    unsigned int delayCnt = 0;

    // Start timer at 0
    TCNT0 = 0;
    // Set compare flag
    TIFR0 |= (1 << OCF0A);

    while (delayCnt < delay){
        while((  TIFR0 & ( 1 << OCF0A)) == 0);
        // increment
        delayCnt++;
        // reset the flag
        TIFR0 |= (1 << OCF0A);
    }
}

void delayUs(unsigned int delay){
    unsigned int delayCnt1 = 0;

    // Start timer at 0
    TCNT1 = 0;
    // Set compare flag
    TIFR1 |= (1 << OCF1A);

    while (delayCnt1 < delay){
        while((  TIFR1 & ( 1 << OCF1A)) == 0);
        // increment
        delayCnt1++;
        // reset the flag
        TIFR1 |= (1 << OCF1A);
    }
}
