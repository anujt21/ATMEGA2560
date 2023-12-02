#include <avr/interrupt.h>
#include <avr/io.h>

#include "motor.h"
#include "timer.h"
#include "pwm.h"
#include "keypad.h"
#include "spi.h"
#include "pwm.h"
#include "rfid.h"

int main() {
  Serial.begin(9600);

  initButtons();
  initTimer1();

    while (1) {
      char pressedButton = readButtons();

      if (pressedButton != '\0') {
        Serial.print("Button ");
        Serial.print(pressedButton);
        Serial.print(" pressed\n");
        delayMs(500);
      }
    }

/*
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
*/

    SPI_Init();
    initModule();

    while (1) {
      readModule();
    }

  return 0;
}
