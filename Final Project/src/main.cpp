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

typedef enum{
  locked, unlocked, alert
}StateType;
volatile StateType lockState = locked; 


int main() {
  Serial.begin(9600);

  // Enable global interrupts
  sei();

  // Initialize all pins and components
  initButtons();
  initTimer0();
  initTimer1();
  initPWMTimer3();
  initPWMTimer4();
  initLCD();
  Serial.println ("Init of LCD complete");
  
  // Lock the door
  lock();
  
  unsigned int numStrikes = 0;
  char savedPassword[WORD_SIZE + 1];
  char enteredPassword[WORD_SIZE + 1];

  // Setup password when the system first starts
  moveCursor(0,0);
  writeString("Set Password:");

  getInput(savedPassword, WORD_SIZE);
  Serial.println(savedPassword);

  delayMs(500);
  //Serial.println(savedPassword);

  moveCursor(1,0);
  writeString("Password Set!");

  // Delay 1sec before asking for password
  delayMs(1000);


  

  while (1) {

    switch(lockState){
      case locked:
      displayMessage("Enter Password", 0);
      getInput(enteredPassword, WORD_SIZE);

      if(checkPassword(enteredPassword, savedPassword) == 0){
        // Display
        displayMessage("Door Opened!", 1000);

        // Open lock
        unlock();

        // Clear all strikes
        numStrikes = 0;
        
        // Change State
        lockState = unlocked;

      }
      else{
        // Display
        displayMessage("Incorrect Password", 1000);

        numStrikes++;
        if(numStrikes == 3){
          Serial.println("Switch to alert");
          lockState = alert;
        }
      }
      break;

      case unlocked:
        
      // Wait for the door to close
      // Door Closes
      // Delay time for simulation
      delayMs(1000);
      delayMs(1000);
      delayMs(1000);
      delayMs(1000);
      delayMs(1000);
      lock();
      lockState = locked;
      break;

      case alert:
      displayMessage("Alarm Acrivated", 0);

      chirpingSound(5000);

      // Check RFID
      // IF RFID passes change state to locked and turn off alarm
      // if not then stay in this state

      turnOffAlarm();
      lockState = locked;
      break;

    }

  }



  return 0;
}
