#include <avr/interrupt.h>
#include <avr/io.h>

#include "lcd.h"
#include "timer.h"
#include "pwm.h"
#include "keypad.h"
#include "spi.h"
#include "pwm.h"
#include "rfid.h"

int main() {
  Serial.begin(9600);
  sei();
  initButtons();
  initTimer0();
  initTimer1();


  // Display Set

  char password[4] = {'1', '2', '3', '4'};  // Set your desired password
  char enteredPassword[4];
  int passwordIndex = 0;

  while (1) {
    char pressedButton = readButtons();

    if (pressedButton != '\0') {
      enteredPassword[passwordIndex] = pressedButton;
      passwordIndex++;

      // Check if the entered password is complete
      if (passwordIndex == 4) {
        // Compare entered password with the desired password
        int match = 1;
        for (int i = 0; i < 4; ++i) {
          if (enteredPassword[i] != password[i]) {
            match = 0;
            break;
          }
        }

        // Reset password variables
        passwordIndex = 0;
        for (int i = 0; i < 4; ++i) {
          enteredPassword[i] = '\0';
        }

        // Handle the result
        if (match) {
          // Correct password(set lcd and motor)
        } 
        else {
          // Incorrect password
        }
      }
    }

    delayMs(100);
  }



  return 0;
}
