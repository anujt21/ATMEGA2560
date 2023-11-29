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

int rowPins[ROWS] = {22, 23, 24, 25}; // Row pinouts
int colPins[COLS] = {26, 27, 28, 29}; // Column pinouts

void initButtons() {
    // Set pins as input with pull-up resistors for rows
    for (int i = 0; i < ROWS; ++i) {
        DDRA &= ~(1 << rowPins[i]);  // Set as input
        PORTA |= (1 << rowPins[i]);   // Enable pull-up resistor
    }

    // Set pins as input with pull-up resistors for columns
    for (int j = 0; j < COLS; ++j) {
        DDRA &= ~(1 << colPins[j]);  // Set as input
        PORTA |= (1 << colPins[j]);   // Enable pull-up resistor
    }

}

// Define debouncing states
enum DebounceState {
    STATE_IDLE,
    STATE_PRESSED,
    STATE_RELEASED
};

char readButtons() {
    char pressedButton = '\0';

    static DebounceState debounceState = STATE_IDLE;
    static char lastButton = '\0';

    for (int i = 0; i < ROWS; ++i) {
        // Set current row as LOW
        PORTA &= ~(1 << rowPins[i]);

        for (int j = 0; j < COLS; ++j) {
            // Check if the column is LOW
            if (!(PINA & (1 << colPins[j]))) {
                switch (debounceState) {
                    case STATE_IDLE:
                        debounceState = STATE_PRESSED;
                        break;

                    case STATE_PRESSED:
                        // Button is still pressed
                        break;

                    case STATE_RELEASED:
                        debounceState = STATE_IDLE;
                        break;
                }
            } 
            
            else {
                switch (debounceState) {
                    case STATE_IDLE:
                        break;

                    case STATE_PRESSED:
                        debounceState = STATE_RELEASED;
                        break;

                    case STATE_RELEASED:
                        // Button is still released
                        break;
                }
            }

            // Check again after debounce
            if (debounceState == STATE_PRESSED && lastButton == '\0') {
                pressedButton = hexaKeys[i][j];
                lastButton = pressedButton;
            }
        }

        // Set current row back to HIGH
        PORTA |= (1 << rowPins[i]);

        if (debounceState == STATE_RELEASED) {
            lastButton = '\0';
        }
    }

    return pressedButton;
}
