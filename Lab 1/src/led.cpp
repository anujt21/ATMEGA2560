// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 1
//----------------------------------------------------------------------//
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

// Used to store the previous ON LED positions
unsigned int prevLED1 = 1;
unsigned int prevLED2 = 6;

// initLED which returns void and takes no parameters
void initLEDs(){
    DDRB  |= (1 << DDB4)| (1 << DDB5);
    DDRH  |= (1 << DDH3)| (1 << DDH4)| (1 << DDH5)| (1 << DDH6);
    DDRE  |= (1 << DDE3);
    DDRG  |= (1 << DDG5);
}

// turnOnLED which returns void and takes in an unsigned int parameter called LEDnum
void turnOnLED(unsigned int LEDnum){
    switch (LEDnum)
    {
    case 0:  // header pin 4
        PORTG |= ( 1 << PORTG5);
        break;

    case 1:  // header pin 5
        PORTE |= (1 << PORTE3);
        break;

    case 2:  // header piin 6
        PORTH |= (1 << PORTH3);
        break;

    case 3:  // header pin 7
        PORTH |= ( 1 << PORTH4);
        break;

    case 4:  // header pin 8
        PORTH |= (1 << PORTH5);
        break;

    case 5:  // header piin 9
        PORTH |= (1 << PORTH6);
        break;
    
    case 6:  // header pin 10
        PORTB |= ( 1 << PORTB4);
        break;

    case 7:  // header pin 11
        PORTB |= (1 << PORTB5);
        break;

    default:
        break;
    }  
}

// turnOffLED which returns void and takes in an unsigned int parameter called LEDnum
void turnOffLED(unsigned int LEDnum){
    switch (LEDnum)
    {
    case 0:  // header pin 4
        PORTG &= ~( 1 << PORTG5);
        break;

    case 1:  // header pin 5
        PORTE &= ~(1 << PORTE3);
        break;

    case 2:  // header piin 6
        PORTH &= ~(1 << PORTH3);
        break;

    case 3:  // header pin 7
        PORTH &= ~( 1 << PORTH4);
        break;

    case 4:  // header pin 8
        PORTH &= ~(1 << PORTH5);
        break;

    case 5:  // header piin 9
        PORTH &= ~(1 << PORTH6);
        break;
    
    case 6:  // header pin 10
        PORTB &= ~( 1 << PORTB4);
        break;

    case 7:  // header pin 11
        PORTB &= ~(1 << PORTB5);
        break;

    default:
        break;
    }
}

// runLED which returns void and takes in an unsigned int parameter called LEDnum
void runLED(unsigned int LEDnum){
    
    // Turn on the LEDs on the opposite sides
    turnOnLED(LEDnum);
    turnOnLED(7 - LEDnum);
    // Turn off the previous LEDs
    turnOffLED(prevLED1);
    turnOffLED(prevLED2);

    // Update the previous LEDs positions
    prevLED1 = LEDnum;
    prevLED2 = 7 - LEDnum;
  
}
