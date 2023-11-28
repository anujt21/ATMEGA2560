#include <avr/io.h>
#include <util/delay.h>

#include "rfid.h"

// Define RFID module register addresses
#define MFRC522_REG_COMMAND     0x01
#define MFRC522_REG_COM_IRQ     0x04
#define MFRC522_REG_FIFO_DATA   0x09

// Define some commands (pg72 in data sheet)
#define MFRC522_CMD_IDLE        0x00
#define MFRC522_CMD_MEM         0x01
#define MFRC522_CMD_RANDOM_ID   0x02
#define MFRC522_CMD_CALC_CRC    0x03
#define MFRC522_CMD_TRANSMIT    0x04
#define MFRC522_CMD_NO_CMD      0x07
#define MFRC522_CMD_SOFT_RESET  0x0F

// Function to initialize SPI communication
void SPI_Init() {
    // Set MOSI(11), SCK(13), and SS(10) as output MISO(12) 
    DDRB |= (1 << DDB2) | (1 << DDB1) | (1 << DDB0);

    // Enable SPI, Master mode, set clock rate fck/16
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

// Function to transmit and receive data via SPI
unsigned char SPI_Transceive(unsigned char data) {
    // Start transmission
    SPDR = data;

    // Wait for transmission complete
    while (!(SPSR & (1 << SPIF)))
        ;

    // Return received data
    return SPDR;
}

// Function to initialize RFID module
void initModule() {
    // Send a soft reset command to the MFRC522
    PORTB &= ~(1 << PB0); // Set SS (Slave Select) low
    SPI_Transceive(MFRC522_REG_COMMAND);
    SPI_Transceive(MFRC522_CMD_SOFT_RESET);
    PORTB |= (1 << PB0); // Set SS high

    _delay_ms(50); // Wait for the soft reset to complete

    // Configure the MFRC522
    // ... (You need to configure the module based on your requirements)
}

// Function to read RFID data
void readModule() {
    // Send a command to the MFRC522 to start reading
    PORTB &= ~(1 << PB0); // Set SS (Slave Select) low
    SPI_Transceive(MFRC522_REG_COMMAND);
    SPI_Transceive(MFRC522_CMD_TRANSMIT);
    PORTB |= (1 << PB0); // Set SS high

    // Wait for the MFRC522 to complete reading

    // Read the received data from the FIFO buffer
    PORTB &= ~(1 << PB0); // Set SS (Slave Select) low
    SPI_Transceive(MFRC522_REG_FIFO_DATA | 0x80); // Read command for FIFO
    unsigned char data = SPI_Transceive(0x00); // Dummy data
    PORTB |= (1 << PB0); // Set SS high

    // Process the received data
    // ... (You need to process the data based on your requirements)
}
