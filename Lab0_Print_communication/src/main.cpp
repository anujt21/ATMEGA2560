// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 0
//
// Description:

// Requirements:
//----------------------------------------------------------------------//

//this is a program to demonstrate how to use the serial print
// Dale Hetherington
// Sept 2, 2021
// ECE 372A Lab0

#include <Arduino.h>
#include <avr/io.h>

int main()    {

// set baud rate for serial port

Serial.begin(9600);


while(1) {

// print something
Serial.println("Hello World.  I am here");
//Serial.println("Hello again");

_delay_ms(5000);

// flush out the buffer
Serial.flush();
// do nothing

}
 return 0;

}