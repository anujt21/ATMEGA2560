// Author:
// Net ID:
// Date:
// Assignment: Lab 5
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//

#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "i2c.h"
#include "spi.h"


#define MPU 0x68
#define PM  0x6b
#define X_H 0x3b
#define X_L 0x3c
#define Y_H 0x3d
#define Y_L 0x3e
#define Z_H 0x3f
#define Z_L 0x40


volatile unsigned int x_value;
volatile unsigned int y_value;
volatile unsigned int z_value;

//state machine for button
typedef enum stateType_enum {
  wait_press, debounce_press, wait_release, debounce_release
} stateType;

volatile stateType buttonState = wait_press; 

//state machine for matrix
typedef enum faceType_enum {
  smiley_face, frowny_face
} face;

volatile face matrixFace= smiley_face; 

volatile int alarmState = 1;


int main(){
  Serial.begin(9600);

  //initializations
  initTimer1();
  initPWMTimer3();
  init_spi();
  initSwitch();
  initI2C();

  //enable interrupts
  sei();
  
  // Setting up the i2c transmission with accelerometer
  Serial.println("Completed init");
  StartI2C_Trans(0x68);
  Serial.println("Completed I2C transfer");
  write(PM);
  write(0x00);
  Serial.println("Completed Setup!");
  StopI2C_Trans();
  Serial.flush();
  

	while (1) { 

    // read X value
    Read_from(MPU, 0x3b);
    x_value = Read_data();
    
    Read_from(MPU, 0x3c);
    x_value = (x_value<<8) | Read_data();

    // read Y value
    Read_from(MPU, 0x3d);
    y_value = Read_data();
    
    Read_from(MPU, 0x3e);
    y_value = (y_value<<8) | Read_data();

    // read Z value
    Read_from(MPU, 0x3f);
    z_value = Read_data();

    Read_from(MPU, 0x40);
    z_value = (z_value<<8) | Read_data();

    Serial.println("X value is:");
    Serial.println(x_value);
    Serial.println("Y value is:");
    Serial.println(y_value);
    Serial.println("Z value is:");
    Serial.println(z_value);
    Serial.println("\n");

    delayMs(100); // Can change the update time for the x,y,z values
    
    // Turn on the alarm and change the matrix face
    // if the x or y value exceed the threshold values
    if((abs(y_value) > 7000) & (abs(y_value) < 56000)){
      matrixFace = frowny_face;
      turnOnAlarm();
    }
    else if((abs(x_value) > 9000) & (abs(x_value) < 56000)){
      matrixFace = frowny_face;
      turnOnAlarm();
    }
    else{
      matrixFace = smiley_face;
    }

    // States for the matrix face
    switch (matrixFace) {
      case smiley_face:
        happy_face();
      break;

      case frowny_face:
        frown_face();
      break;

      default:
        matrixFace = smiley_face;
      break;

    }
    
    // States for the button switch
    // The alarm is turned off in the debounce release state
    switch(buttonState) { 

      case wait_press:
      for(int i = 1000; i < 4000; i++){
        changeFre(i);
      }
      break;
    
      case debounce_press: 
      for(int i = 1000; i < 4000; i++){
        changeFre(i);
        
      }
      buttonState = wait_release;
      delayMs(1); 
      break;
      
      case wait_release:
      for(int i = 1000; i < 4000; i++){
        changeFre(i);
        
      }
      break;
      
      case debounce_release:
        turnOffAlarm();
        buttonState = wait_press;
        delayMs(1);
      break;
    }
    
    //StopI2C_Trans();
	}
  return 0;
}


// ISR for switch to stop the alarm noise
ISR(INT2_vect) {
  
  if(buttonState == wait_press)
  {
    buttonState = debounce_press;
  }
  else if (buttonState == wait_release) {
      matrixFace = smiley_face;
      buttonState = debounce_release;
    }

}
