#include <avr/io.h>
#include <util/delay.h>
#include <timer.h>
#include <keypad.h>

// Define rows and columns
#define ROWS 4
#define COLS 4

// Define keymap
const char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Define pin mappings for rows and columns
uint8_t rowPins[ROWS] = {PL3, PL2, PL1, PL0}; // Using Port A pins, pins 8-5 on keypad
uint8_t colPins[COLS] = {PC3, PC2, PC1, PC0}; // Using Port C pins, pins 4-1 on keypad


void initButtons() {
    // Initialize row pins as outputs and set them high
    DDRL |= (1 << PL3) | (1 << PL2) | (1 << PL1) | (1 << PL0);
    PORTL |= (1 << PL3) | (1 << PL2) | (1 << PL1) | (1 << PL0);

    // Initialize column pins as inputs with pull-up resistors
    DDRC &= ~((1 << PC3) | (1 << PC2) | (1 << PC1) | (1 << PC0));
    PORTC |= (1 << PC3) | (1 << PC2) | (1 << PC1) | (1 << PC0);

}


char readButtons() {
    for (uint8_t row = 0; row < ROWS; row++) {

        PORTL &= ~(1 << rowPins[row]); // Set current row low

        for (uint8_t col = 0; col < COLS; col++) {

            if (!(PINC & (1 << colPins[col]))) { // Check if button pressed
                _delay_ms(20); // Debounce delay
                while (!(PINC & (1 << colPins[col]))); // Wait for button release
                PORTL |= (1 << rowPins[row]); // Set row high again
                return keys[row][col]; // Return key pressed
            }
        }

        PORTL |= (1 << rowPins[row]); // Set row high again
    }
    
    return 0; // No key pressed
}