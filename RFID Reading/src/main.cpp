#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <arduino.h>

#include "rfid.h"

#define PICC_REQIDL    0x26

// Define the valid card ID
const unsigned char validCardID[] = {0xD7, 0x46, 0x03, 0x53};

int main(void) {
    Serial.begin(9600);

    sei();
    unsigned char status;
    unsigned char str[16];
    unsigned char serNum[5];

    spiInit();  // Initialize SPI
    MFRC522_Init(); // Initialize RC522
    Serial.println("Hello");
    while (1) {
        status = MFRC522_Request(PICC_REQIDL, str);  
        if (status == 1) {
            Serial.println("Card Detected");
            status = MFRC522_Anticoll(serNum);
            if (status == 0) {
                // Print UID
                Serial.print("Card UID:");
                for (int i = 0; i < 4; i++) {
                    Serial.print(serNum[i], HEX);
                    if (i < 3) {
                        Serial.print(":");
                    }
                }
                Serial.println();
            }
        }
        _delay_ms(1000); // Delay between reads
    }
}
