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
#define PM  0x6B
#define X_H 0x3B
#define X_L 0x3C
#define Y_H 0x3D
#define Y_L 0x3E
#define Z_H 0x3F
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
typedef enum matrixType_enum {
  smiley_face, frowny_face
} matrixType;

volatile matrixType matrixFace = smiley_face; 


int main(){
  Serial.begin(9600);

  //initializations
  initTimer1();
  initPWMTimer3();
  initPWMTimer4();
  init_spi();
  initSwitch();
  initalarm();
  initI2C();

  //enable interrupts
  sei();
  
  int x_value, y_value, z_value;
  
  Serial.println("Completed init");
  StartI2C_Trans(MPU);
  Serial.println("Completed I2C transfer");
  Serial.println(TWSR);
  write(PM);
  write(0x00);
  Serial.println("Completed Setup!");
  
	while (1) {
    
    // read X value
    Read_from(MPU, X_H);
    x_value = Read_data();
    Read_from(MPU, X_L);
    x_value = (x_value << 8) | Read_data();

    // read Y value
    Read_from(MPU, Y_H);
    y_value = Read_data();
    Read_from(MPU, Y_L);
    y_value = (y_value << 8) | Read_data();

    // read Z value
    Read_from(MPU, Z_H);
    z_value = Read_data();
    Read_from(MPU, Z_L);
    z_value = (z_value << 8) | Read_data();

    if((abs(y_value) > 10000) | (abs(x_value) > 10000))
    {
       matrixFace = frowny_face; 
      Serial.print("The system triggered at:");
      Serial.print(abs(x_value));
      Serial.print(", ");
      Serial.println(abs(y_value));
      turnon_alarm();
    }

    Serial.println("X, Y, Z values are: ");
    Serial.println(x_value);
    Serial.println(y_value);
    Serial.println(z_value);
    Serial.println("\n");

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

    switch(buttonState) { //waiting for switch to be pressed
      case wait_press:
      break;
    
      case debounce_press: 
        buttonState = wait_release;
        delayMs(1); 
      break;
      
      case wait_release:
      break;
      
      case debounce_release:
        buttonState = wait_press;
        delayMs(1);
      break;
    }
  
    StopI2C_Trans();
	}
  return 0;
}

// ISR for switch to stop the alarm noise
ISR(INT0_vect) {
  if(buttonState == wait_press)
    {
        buttonState = debounce_press;
    }
    else if (buttonState == wait_release) {
     silence_alarm();
      matrixFace = smiley_face;
      buttonState = debounce_release;
    }

}
