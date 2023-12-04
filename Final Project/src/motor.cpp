#include "motor.h"

#include <avr/io.h>

void initPWMTimer4() {

    // Set header pins 6 output for Timer 4 A
    DDRH |= (1 << DDH3);

    // Set Fast PWM mode with ICR4 as TOP
    TCCR4A |= (1 << WGM41) | (1 << COM4A1);
    TCCR4B |= (1 << WGM43) | (1 << WGM42) | (1 << CS41); // Prescaler 8

    // Set ICR4 for a period of 20 ms (50 Hz)
    ICR4 = 39999;

    // Initialize servo to minimum position (1 ms pulse width)
    OCR4A = 2000;
}

void lock(){
    OCR4A = 4000;
}

void unlock(){
    OCR4A = 2000;
}