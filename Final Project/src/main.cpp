#include <avr/interrupt.h>
#include <avr/io.h>

#include "password.h"
#include "lcd.h"
#include "timer.h"
#include "pwm.h"
#include "keypad.h"
#include "spi.h"
#include "pwm.h"
#include "rfid.h"
#include "motor.h"

#define WORD_SIZE 6

int main() {
  Serial.begin(9600);

  // Enable global interrupts
  sei();

  // Initialize all pins and components
  initButtons();
  initTimer0();
  initTimer1();
  initPWMTimer4();
  initLCD();
  Serial.println ("Init of LCD complete");

  // Lock the door
  lock();
  

  char savedPassword[WORD_SIZE];
  char enteredPassword[WORD_SIZE];

  // Setup password when the system first starts
  moveCursor(0,0);
  writeString("Set Password:");

  getInput(savedPassword, WORD_SIZE);

  delayMs(1000);
  //Serial.println(savedPassword);

  moveCursor(1,0);
  writeString("Password Set!");

  // Delay 3sec before clearing
  delayMs(1000);
  delayMs(1000);
  delayMs(1000);

  clearDisplay(); // Clear

  delayMs(500);
  moveCursor(0,0);
  writeString("Enter Password");

  while (1) {

    getInput(enteredPassword, WORD_SIZE);

    if(checkPassword(enteredPassword, savedPassword)){
      // open lock
      // Press 1 to lock the door
    }

  }



  return 0;
}
