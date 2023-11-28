#include "keypad.h"
#include <avr/io.h>
#include <util/delay.h>

const int ROWS = 4; // Four rows
const int COLS = 4; // Four columns

char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

int rowPins[ROWS] = {13, 12, 11, 10}; // Row pinouts
int colPins[COLS] = {9, 8, 7, 6}; // Column pinouts

void initButtons() {
    // Set pins as input with pull-up resistors for rows
        for (int i = 0; i < ROWS; ++i) {
        PORTB &= ~(1 << rowPins[i]);  // Set as input
        PORTB |= (1 << rowPins[i]);   // Enable pull-up resistor
    }

    // Set pins as input with pull-up resistors for columns
    for (int i = 0; i < COLS; ++i) {
        PORTH &= ~(1 << colPins[i]);  // Set as input
        PORTH |= (1 << colPins[i]);   // Enable pull-up resistor
    }

}

char readButtons() {
    char pressedButton = '\0';

    for (int i = 0; i < ROWS; ++i) {
        // Set current row as LOW
        PORTB &= ~(1 << rowPins[i]);

        for (int j = 0; j < COLS; ++j) {
            // Check if the column is LOW
            if (!(PINH & (1 << colPins[j]))) {
                // Check again after debounce
                if (!(PINH & (1 << colPins[j]))) {
                    pressedButton = hexaKeys[i][j];
                    break;
                }
            }
        }

        // Set current row back to HIGH
        PORTB |= (1 << rowPins[i]);

        if (pressedButton != '\0') {
            break;
        }
    }
    return pressedButton;
}
