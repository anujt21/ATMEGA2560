#include <avr/io.h>

#include "pwm.h"

void initPWMTimer3() {
  // Set header pins 5 output for Timer 3 A and B
  DDRE |= (1 << DDE3);

  // Set non-inverting mode for both Timer 3 A and B
  // COM3A1 = 1, COM3A0 = 0 (Channel A)
  // COM3B1 = 1, COM3B0 = 0 (Channel B)
  TCCR3A |= (1 << COM3A1);
  TCCR3A &= ~ (1 << COM3A0);

  // Use fast PWM mode 10-bit, top value is 0x3FF (1023)
  // Fast PWM 10-bit mode: WGM30 = 1, WGM31 = 1, WGM32 = 1, WGM33 = 0
  TCCR3A |= (1 << WGM30) | (1 << WGM31);
  TCCR3B |= (1 << WGM32);
  TCCR3B &= ~(1 << WGM33);

  // Set prescaler to 1 for PWM frequency of 15.625 kHz (assuming a 16MHz clock)
  // CS30 = 1, CS31 = 0, CS32 = 0
  TCCR3B |= (1 << CS30);
  TCCR3B &= ~((1 << CS31) | (1 << CS32));

}

void initPWMTimer4() {
  // Set header pins 6 output for Timer 3 A and B
  DDRH |= (1 << DDH3);

  // Set non-inverting mode for both Timer 3 A and B
  // COM3A1 = 1, COM3A0 = 0 (Channel A)
  // COM3B1 = 1, COM3B0 = 0 (Channel B)
  TCCR4A |= (1 << COM4A1);
  TCCR4A &= ~ (1 << COM4A0);

  // Use fast PWM mode 10-bit, top value is 0x3FF (1023)
  // Fast PWM 10-bit mode: WGM30 = 1, WGM31 = 1, WGM32 = 1, WGM33 = 0
  TCCR4A |= (1 << WGM40) | (1 << WGM41);
  TCCR4B |= (1 << WGM42);
  TCCR4B &= ~(1 << WGM43);

  // Set prescaler to 1 for PWM frequency of 15.625 kHz (assuming a 16MHz clock)
  // CS30 = 1, CS31 = 0, CS32 = 0
  TCCR4B |= (1 << CS40);
  TCCR4B &= ~((1 << CS41) | (1 << CS42));
}

void changeDutyCycle(unsigned int num){
    OCR3A = num;
    OCR4A = 1023 - num;
}