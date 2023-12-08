#ifndef RFID_H
#define RFID_H

#include <avr/io.h>
#include <util/delay.h>

void spiInit();
void spiWrite(unsigned char data);
unsigned char spiRead();
void MFRC522_Write(unsigned char reg, unsigned char data);
unsigned char MFRC522_Read(unsigned char reg);
void MFRC522_Init();
unsigned char MFRC522_Request(unsigned char reqMode, unsigned char* TagType);
unsigned char MFRC522_Anticoll(unsigned char* serNum);
unsigned char MFRC522_ToCard(unsigned char command, unsigned char* sendData, unsigned char sendLen, unsigned char* backData, unsigned int* backLen);

#endif // RFID_H