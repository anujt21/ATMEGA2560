// Author:
// Net ID:
// Date:
// Assignment:     Lab 3
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//

#include <Arduino.h>
#include <avr/io.h>

#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "i2c.h"



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


int main(){

  sei();

  initTimer0();
  initTimer1();

  Serial.begin(9600);

  initI2C();
  Serial.println("Completed init");
  StartI2C_Trans(MPU);
  Serial.println("Completed I2C transfer");
  Serial.println(TWSR);
  write(PM);
  write(0x00);
  Serial.println("Completed Setup!");
  
	while (1) {
    
    // read X value
    Read_from(MPU, 0x3b);
    x_value = Read_data();

    Read_from(MPU, 0x3c);
    x_value = (x_value<<8) | Read_data();

    // read Y value
    Read_from(MPU, 0x3d);
    y_value = Read_data();

    Read_from(MPU, 0x3);
    y_value = (y_value<<8) | Read_data();

    // read Z value
    Read_from(MPU, 0x3f);
    z_value = Read_data();

    Read_from(MPU, 0x40);
    z_value = (z_value<<8) | Read_data();

    Serial.println("X, Y, Z values are: ");
    Serial.println(x_value);
    Serial.println(y_value);
    Serial.println(z_value);
    Serial.println("\n");
    

	}
  
  return 0;
}

