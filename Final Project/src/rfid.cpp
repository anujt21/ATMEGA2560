#include <avr/io.h>
#include <util/delay.h>
#include "rfid.h"


// Define RFID module register addresses
#define RC522_REG_COMMAND     0x01
#define RC522_REG_COM_IRQ     0x04
#define RC522_REG_FIFO_DATA   0x09

// Define some commands (pg72 in data sheet)
#define RC522_CMD_IDLE        0x00
#define RC522_CMD_MEM         0x01
#define RC522_CMD_RANDOM_ID   0x02
#define RC522_CMD_CALC_CRC    0x03
#define RC522_CMD_TRANSMIT    0x04
#define RC522_CMD_NO_CMD      0x07
#define RC522_CMD_SOFT_RESET  0x0F


#define RST_PIN 5
#define SDA_PIN 53
#define MOSI_PIN 51
#define SCK_PIN 52
#define MISO_PIN 50

// Define the valid card ID
const unsigned char validCardID[] = {0xDA, 0xE9, 0x2A, 0xFE};

// Function declaration
bool compareCardID(const unsigned char* cardID, const unsigned char* validCardID, unsigned char size);

void spiInit() {
  // Set MOSI (51), SCK (53), and SS (53) as output, MISO (50) as input
  DDRB |= (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
  DDRD |= (1 << DDD5);

  // Enable SPI, Master mode, set clock rate fck/4
  SPCR = (1 << SPE) | (1 << MSTR);
}

void spiWrite(unsigned char data) {
  SPDR = data;
  while (!(SPSR & (1 << SPIF)));//indicates whether date transfer is complete and the recieved data is ready to read
}

unsigned char spiRead() {
  spiWrite(0xFF);  // Send a dummy byte to generate clock
  return SPDR;
}

void initRFIDModule() {
    PORTE &= ~(1 << PE3);
    _delay_ms(1);
    PORTE |= (1 << PE3);
    _delay_ms(100);
}

bool readRFIDCard(unsigned char* cardID, const unsigned char* validCardID, unsigned char size) {
    for (unsigned char i = 0; i < size; ++i) {
        cardID[i] = spiRead();
    }

    return compareCardID(cardID, validCardID, size);
}

bool compareCardID(const unsigned char* cardID, const unsigned char* validCardID, unsigned char size) {
  for (unsigned char i = 0; i < size; ++i) {
    if (cardID[i] != validCardID[i]) {
      return false; // Not equal
    }
  }
  return true; // Equal
}

