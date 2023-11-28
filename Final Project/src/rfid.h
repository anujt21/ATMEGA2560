#ifndef RFID_H
#define RFID_H

void SPI_Init();
unsigned char SPI_Transceive(unsigned char data);
void initModule();
void readModule();


#endif
