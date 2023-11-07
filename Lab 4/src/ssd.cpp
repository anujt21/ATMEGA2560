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
    PORTC = 0xFF; //common cathode; initializing all segments off

}


void display(int num){
    //look up table for common cathode for nums 1-9
    unsigned char lookup[10] = {
        0x3F, //0
        0x06, //1
        0x5B, //2
        0x4F, //3
        0x66, //4
        0x6D, //5
        0x7D, //6
        0x07, //7
        0x7F, //8
        0x6F  //9
    };

    PORTC = lookup[num]; // display digit
}


void countdown(){
    unsigned int n = 9;
    while(n!=0){
        display(n);
        delayMs(1000);
        n--;
    }
}