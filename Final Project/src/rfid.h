#ifndef RFID_H
#define RFID_H

#include <avr/io.h>
#include <util/delay.h>

void spiInit();
void spiWrite(unsigned char data);
unsigned char spiRead();
void initRFIDModule();
bool readRFIDCard(unsigned char* cardID, const unsigned char* validCardID, unsigned char size);

#endif // RFID_H