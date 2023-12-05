#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

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

#define SS_PIN PB0

// Define RFID tag ID to grant access
const unsigned char validTagID[] = {0xDA, 0xE9, 0x2A, 0xFE};

// Global variable to indicate RFID tag presence
volatile bool tagPresentFlag = false;

volatile unsigned char tagData[4];

const unsigned char RST_PIN = 5;

// Function to initialize SPI communication
void SPI_Init() {
    // Set MOSI(51), SCK(52), and SS as output, MISO(50) as input
    DDRB |= (1 << DDB2) | (1 << DDB1) | (1 << DDB3);

    PORTE &= ~(1 << RST_PIN);

    // Enable SPI, Master mode, set clock rate fck/16
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

// Function to transmit and receive data via SPI
char SPI_Transceive(char data) {
    // Start transmission
    SPDR = data;

    // Wait for transmission complete
    while (!(SPSR & (1 << SPIF)));

    // Return received data
    return SPDR;
}

// Function to initialize RFID module
void initModule() {
    // Send a soft reset command to the MFRC522
    PORTB &= ~(1 << SS_PIN); // Set SS (Slave Select) low
    SPI_Transceive(RC522_REG_COMMAND);
    SPI_Transceive(RC522_CMD_SOFT_RESET);
    PORTB |= (1 << SS_PIN); // Set SS high

    _delay_ms(50); // Wait for the soft reset to complete

    // Enable external interrupt on the RFID module's IRQ pin
    EIMSK |= (1 << INT0);   // Enable INT0
    EICRA |= (1 << ISC01);  // Trigger on falling edge

}

// Function to read RFID data
bool readRFIDTag() {
    // Send a command to the RC522 to start reading
    PORTB &= ~(1 << SS_PIN); // Set SS (Slave Select) low
    SPI_Transceive(RC522_REG_COMMAND);
    SPI_Transceive(RC522_CMD_TRANSMIT);
    PORTB |= (1 << SS_PIN); // Set SS high

    // Wait for the RC522 to complete reading
    while (!tagPresentFlag) {
        // Wait for tag presence
    }

    tagPresentFlag = false;

    // Read the received data from the FIFO buffer
    PORTB &= ~(1 << SS_PIN); // Set SS (Slave Select) low
    SPI_Transceive(RC522_REG_FIFO_DATA | 0x80); // Read command for FIFO

    // Read tag data
    for (unsigned char i = 0; i < 4; ++i) {
        tagData[i] = SPI_Transceive(0x00);
    }

    PORTB |= (1 << SS_PIN); // Set SS high

    // Compare RFID tag ID
    unsigned char compareResult = compareTagID(tagData, validTagID);

    return compareTagID(tagData, validTagID);
}

// External interrupt service routine for RC522 module's IRQ pin
ISR(INT0_vect) {
    tagPresentFlag = true; // Set the tag presence flag
}

// Function to compare RFID tag ID
unsigned char compareTagID(const volatile unsigned char* tagData, const unsigned char* constValidTagID){
    for (unsigned char i = 0; i < 4; ++i) {
        if (tagData[i] != constValidTagID[i]) {
            return 0; // Not equal
        }
    }
    return 1; // Equal
}
