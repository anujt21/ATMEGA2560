/*
## In a file called spi.cpp
    1.All communication with the 8 x8 led matrix must be done over the SPI
    related pins.
    2.Read the 8x8 MAX7219 datasheet for setting up SPI mode
*/

#include <Arduino.h>
#include "spi.h"
#include <avr/interrupt.h>
#include <avr/io.h>

void init_spi(){
    // set MOSI,SCK,and SS direction to outputs
    DDRB = (1 << DDB2)| (1 << DDB1) | (1 << DDB0);

    // set SS high initially (chip select off)
    PORTB |= (1 << PORTB0);

    // enable SPI, master mode, CPOL, CPHA, default clock and fosc/128
    // datasheet says sample on rising edge CPOL = 1 CPHA =1
    SPCR  |= (1 <<SPE)| (1 << MSTR) | (1 << CPOL) | (1 << CPHA ) | (1 << SPR1) | (1 << SPR0);
}

void write_execute(unsigned char CMD, unsigned char data) {
    PORTB &= ~(1 << PORTB0);  // enable chip select bit to begin SPI frame
    SPDR = CMD; // load the CMD address into register
    while(!(SPSR & (1 << SPIF))); // wait for flag to raise
    SPDR = data; // load the data into register
    while(!(SPSR & (1 << SPIF))); // wait for flag to raise
    PORTB |= (1 << PORTB0); // disable chip select to end SPI frame
}

void frown_face(){
write_execute(0x0A, 0x08);  // brightness control
write_execute(0x0B, 0x07); // scanning all rows and columns
write_execute(0x0C,0x01);  // set shutdown register to normal operation (0x01)
write_execute(0x0F, 0x00); // display test register - set to normal operation (0x01)

write_execute(0x01, 0b00111100); // row 1 LEDS
write_execute(0x02, 0b01000010); // row 2 LEDS 
write_execute(0x03, 0b10100101); // row 3 LEDS
write_execute(0x04, 0b10000001); // row 4 LEDS
write_execute(0x05, 0b10011001); // row 5 LEDS
write_execute(0x06, 0b10100101); // row 6 LEDS
write_execute(0x07, 0b01000010); // row 7 LEDS
write_execute(0x08, 0b00111100); // row 8 LEDS
}

void happy_face(){
write_execute(0x0A, 0x08);  // brightness control
write_execute(0x0B, 0x07); // scanning all rows and columns
write_execute(0x0C,0x01); // set shutdown register to normal operation (0x01)
write_execute(0x0F, 0x00); // display test register - set to normal operation (0x01)

write_execute(0x01, 0b00111100); // row 1 LEDS
write_execute(0x02, 0b01000010); // row 2 LEDS 
write_execute(0x03, 0b10100101); // row 3 LEDS
write_execute(0x04, 0b10000001); // row 4 LEDS
write_execute(0x05, 0b10100101); // row 5 LEDS
write_execute(0x06, 0b10011001); // row 6 LEDS
write_execute(0x07, 0b01000010); // row 7 LEDS
write_execute(0x08, 0b00111100); // row 8 LEDS
}
