#include <avr/io.h>

#include "pwm.h"

void initPWMTimer3() {
  // Set header pins 3 output for Timer 3 C
  DDRE |= (1 << DDE5);

  // Set non-inverting mode for both Timer 3 C
  // COM3C1 = 1, COM3C0 = 0 (Channel C)
  TCCR3A |= (1 << COM3C1);
  TCCR3A &= ~ (1 << COM3C0);

  // Use fast PWM mode 15, top value is OCR3A
  // WGM30 = 1, WGM31 = 1, WGM32 = 1, WGM33 = 1
  TCCR3A |= (1 << WGM30) | (1 << WGM31);
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << WGM33);
  
  // Turn off the alarm initally
  TCCR3B &= ~(1 << CS30);
}

void changeFre(unsigned int num){
  // Calculating the OCR3A value
  // OCR3A = fclk/(1*fpwm)
  OCR3AH = ((16000000/num) >> 8);
  OCR3AL = 16000000/num;

  // Set the duty cycle to 50% by
  // setting OCR3C = OCR3A/2
  OCR3CH = OCR3AH >> 1;
  OCR3CL = OCR3AL >> 1;

}

void turnOnAlarm(){
  // Set prescalar to 1 to turn on the alarm
  TCCR3B |= (1 << CS30);
}

void turnOffAlarm(){
  TCCR3B &= ~(1 << CS30);
}




