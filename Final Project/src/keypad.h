#ifndef KEYPAD_H
#define KEYPAD_H

#include <avr/io.h>

// Define debouncing states
enum DebounceState {
    STATE_IDLE,
    STATE_PRESSED,
    STATE_RELEASED
};


void initButtons();
char readButtons();

#endif
