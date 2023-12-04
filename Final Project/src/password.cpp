#include "password.h"
#include <string.h>
#include <Arduino.h>


unsigned int checkPassword(char* str1, char* str2){
    return strcmp(str1, str2);
}

void getInput(char* str, unsigned int maxLength){

    unsigned int index = 0;
    while(1){
    char userInput = readButtons(); // Read character from keypad

    if(userInput){
      
      if(userInput == '#'){
        str[index] = '\0';  // If user entered # then end string
        return;
      }

      if(userInput == '*'){
        if(index > 0){
          index--;
        }
        print_password_char(' ', index);
      }
      else if(index != maxLength){
        Serial.println(userInput);
        print_password_char(userInput, index);  // Print the character on the LCD

        str[index] = userInput; // Store the input in the array
        index++;  // increment index

      }

      
    }
  }


}