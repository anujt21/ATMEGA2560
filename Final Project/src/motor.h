#ifndef MOTOR_H
#define MOTOR_H

#include <avr/io.h>

void initPWMTimer4();
void lock();
void unlock();

#endif