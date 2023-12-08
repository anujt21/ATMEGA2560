#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <arduino.h>

#include "rfid.h"

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53          // Configurable, see typical pin layout above

// Define the valid card ID
byte validCardID[] = {0x35, 0x79, 0x35, 0x76};

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

int main(void) {
    Serial.begin(9600);
    sei();
    //void spiInit();	
    Serial.println("Hello");
    Serial.flush();		
	mfrc522.PCD_Init();
    Serial.println("Hello");
    byte* ID;
    unsigned int IDsize;
    
    while (1) {
        bool b = 1;
        bool match = 1;
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
		    b = 0;
	    }

        // Select one of the cards
        if(b == 1){
            if ( ! mfrc522.PICC_ReadCardSerial()) {
                b = 0;
            }
        }

        // Dump debug info about the card; PICC_HaltA() is automatically called
        if(b == 1){
	        //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

            ID = mfrc522.PICC_GetUid(&(mfrc522.uid));
            IDsize = mfrc522.PICC_GetSize(&(mfrc522.uid));
            Serial.println("ID is:");
            for(byte i=0;i<IDsize;i++){
                Serial.print(ID[i] < 0x10 ? " 0" : " ");
                Serial.print(ID[i], HEX);               
            }
            Serial.print("\n");

            for(byte i=0;i<4;i++){
                if(validCardID[i] != ID[i]){
                    match=0;
                    break;
                }
            }

            if(match == 1){
                Serial.println("Valid Card");
            }
            else{
                Serial.println("Invalid Card");
            }
        }

        _delay_ms(100);

    }
}
