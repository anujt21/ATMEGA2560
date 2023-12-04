#ifndef PASS_H
#define PASS_H

#include <avr/io.h>
#include "keypad.h"
#include "lcd.h"

unsigned int checkPassword(char* str1, char* str2);
void getInput(char* str, unsigned int maxLength);






#endif