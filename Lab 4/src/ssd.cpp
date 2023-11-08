/*
    ## Seven Segment display.
    Use PORTC pins as outputs to drive the seven segment display
    configured in a common cathode mode.
*/

#include <avr/io.h>
#include "ssd.h"
#include "timer.h"

void seven_segment_init(){

    DDRC = 0xFF; //Setting PortC as an output
    PORTC = 0x00; //common cathode; initializing all segments off

}


void display(int num){
    //look up table for common cathode for nums 1-9
    unsigned char lookup[10] = {
        0x3f,
        0x06,
        0x5b,
        0x4f,
        0x66,
        0x6d,
        0x7d,
        0x07,
        0x7f,
        0x6f
    };

    PORTC = lookup[num]; // display digit
}


void countdown(){
    unsigned int n = 9;
    while(n!=0){
        display(n);
        delayMs(1000);
        n = n-1;
    }
    display(0);
    delayMs(1000);
    PORTC = 0x00;
}