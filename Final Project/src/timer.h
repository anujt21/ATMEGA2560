
#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include <avr/io.h>

void initTimer0();
void delayUs(unsigned int delay);
void initTimer1();
void delayMs(unsigned int delay);
void chirpingSound(unsigned int delay);

#endif
