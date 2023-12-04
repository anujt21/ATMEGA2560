#include "password.h"
#include <string.h>


unsigned int checkPassword(char* str1, char* str2){
    return 1 - strcmp(str1, str2);
}

void getInput(char* str, unsigned int maxLength){

    unsigned int index = 0;
    while(1){
    char userInput = readButtons(); // Read character from keypad

    if(userInput){

      if(userInput == '#'){
        if(index != maxLength){
          str[index] = '\0';  // If user entered # then end string
        }
        break;
      }
      
      if(index != maxLength){
        print_password_char(userInput, index);  // Print the character on the LCD

        str[index] = userInput; // Store the input in the array
        index++;  // increment index

      }

      
    }
  }


}