#ifndef RFID_H
#define RFID_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

extern const unsigned char constValidTagID[4];
extern volatile bool tagPresentFlag;
extern volatile unsigned char tagData[4];

void SPI_Init();
char SPI_Transceive(char data);
void initModule();
void readRFIDTag();
unsigned char compareTagID(const volatile unsigned char* tagData, const unsigned char* constValidTagID);

#endif
