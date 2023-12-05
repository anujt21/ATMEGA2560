#include <avr/interrupt.h>
#include <avr/io.h>

#include "password.h"
#include "lcd.h"
#include "timer.h"
#include "pwm.h"
#include "keypad.h"
#include "rfid.h"
#include "motor.h"
#include "switch.h"

#define WORD_SIZE 6

typedef enum{
  locked, unlocked, alert
}StateType;

typedef enum{
  wait_press, debounce_press, wait_release, debounce_release
}ButtonState;

volatile ButtonState buttonState = wait_press;

volatile StateType lockState = locked; 


int main() {
  Serial.begin(9600);

  // Enable global interrupts
  sei();

  // Initialize all pins and components
  initSwitch();
  initButtons();
  initTimer0();
  initTimer1();
  initPWMTimer3();
  initPWMTimer4();
  initLCD();
  Serial.println ("Init of LCD complete");
  
  // Wait for RFID to check to pass

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


    switch(buttonState){
      case wait_press:
      break;
      
      case debounce_press:
      delayMs(1);
      buttonState = wait_release;
      break;

      case wait_release:
      break;
      
      case debounce_release:
      delayMs(1);
      buttonState = wait_press;
      break;
    
    }

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
      
      if(buttonState == wait_release){
        lock();
        lockState = locked;
      }
      break;

      case alert:
      displayMessage("Alarm Activated", 0);

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


ISR(INT0_vect){
  if(buttonState == wait_press){
    buttonState = debounce_press;
  }
  else if(buttonState == wait_release){
    buttonState = debounce_release;
  }
}