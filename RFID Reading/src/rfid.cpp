#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

// RC522 Registers
#define CommandReg     0x01
#define ComIEnReg      0x02
#define DivIEnReg      0x03
#define ComIrqReg      0x04  // Interrupt request register
#define DivIrqReg      0x05  // Interrupt request register
#define ErrorReg       0x06  // Error register
#define Status1Reg     0x07  // Status register
#define Status2Reg     0x08  // Status register
#define FIFODataReg    0x09
#define FIFOLevelReg   0x0A
#define BitFramingReg  0x0D
#define ControlReg     0x0C  // Control register
#define ModeReg        0x11
#define TxModeReg      0x12
#define RxModeReg      0x13
#define TxControlReg   0x14   

// RC522 Commands
#define PCD_IDLE       0x00   
#define PCD_TRANSCEIVE 0x0C   
#define PCD_RESETPHASE 0x0F
#define PICC_ANTICOLL  0x93   

// Maximum length of the array
#define MAX_LEN 16
#define MI_OK 0
#define MI_ERR 1

// PE3 is the RST pin and Port B0 is the SS pin
void spiInit() {
    // Set MOSI, SCK, SS as Output
    DDRB = (1<<DDB2)|(1<<DDB1)|(1<<DDB0);
    // set the MISO pin as input
    DDRB &= ~(1 << DDB3);
    // set SS high at first
    PORTB |= (1 << PB0);
    // Enable SPI, Set as Master
    // Prescaler: Fosc/4, Enable Interrupts
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}


void spiWrite(unsigned char data) {
    SPDR = data; // Start transmission
    while(!(SPSR & (1<<SPIF))); // Wait for transmission to complete
}

void MFRC522_Write(unsigned char reg, unsigned char data) {
    // set SS low to begin SPI frame
    PORTB &= ~(1<< PB0); // Select MFRC522

    spiWrite((reg << 1) & 0x7E); // Address format: 0XXXXXX0
    spiWrite(data);
    PORTB |= (1<<PB0); // Deselect MFRC522
}

void MFRC522_Init() {
    DDRE |= (1<<PE3); // Set RST_PIN as Output
    
    PORTE |= (1<<PE3); // Reset MFRC522

    _delay_ms(100);
    
    PORTE &= ~(1<<PE3);
    _delay_ms(10);
    
    PORTE |= (1<<PORTE3);
    _delay_ms(10);

    MFRC522_Write(CommandReg, PCD_RESETPHASE); // Soft reset
    MFRC522_Write(TxControlReg, 0x83); // Turn on the antenna: TX1/2 enanled; TX2 gives inverted output
}

unsigned char spiRead() {
    spiWrite(0xFF); // Send dummy byte to read from slave
    while(!(SPSR & (1<<SPIF))); // Wait for transmission to complete
    return SPDR; // Return received data
}



unsigned char MFRC522_Read(unsigned char reg) {
    PORTB &= ~(1<<PORTB0); // Select MFRC522
    spiWrite(((reg << 1) & 0x7E) | 0x80); // Address format: 1XXXXXX0
    unsigned char val = spiRead();
    PORTB |= (1<<PORTB0); // Deselect MFRC522
    return val;
}


unsigned char MFRC522_Request(unsigned char reqMode, unsigned char* TagType) {
    unsigned char status;
    unsigned int backBits; // The received data bits
    Serial.println("Mode:");
    Serial.println(MFRC522_Read(ModeReg), HEX);
    Serial.println("TxMode:");
    Serial.println(MFRC522_Read(TxModeReg));
     Serial.println("RxMode:");
    Serial.println(MFRC522_Read(TxModeReg));

    // TxLastBits = 000b, StartSend = 0
    //MFRC522_Write(BitFramingReg, 0x00);
    MFRC522_Write(BitFramingReg, 0x00);

    TagType[0] = reqMode;
    MFRC522_Write(CommandReg, PCD_TRANSCEIVE);
    MFRC522_Write(FIFOLevelReg, 0x80);  // Flush the FIFO buffer
    MFRC522_Write(FIFODataReg, TagType[0]); // Write data to the FIFO

    // Set StartSend bit to 1 to start transmission
    MFRC522_Write(BitFramingReg, 0x80);

    // Wait for a response
    _delay_ms(10);
    status = MFRC522_Read(ComIrqReg);
    Serial.println("ComIRq bits:");
    Serial.println(ComIrqReg,HEX);
    _delay_ms(10);
    Serial.println("Error bits:");
    Serial.println(ErrorReg, HEX);

    if ((status & 0x40) && (status & 0x20)) { // Check for TxIRq and RxIRq
        backBits = MFRC522_Read(FIFOLevelReg);
        if (backBits > 0) {
            for (int i = 0; i < backBits; i++) {
                TagType[i] = MFRC522_Read(FIFODataReg);
            }
            MFRC522_Write(CommandReg, PCD_IDLE);
            return 1; // Card detected
        }
    }

    MFRC522_Write(CommandReg, PCD_IDLE);
    return 0; // No card detected
}




unsigned char MFRC522_ToCard(unsigned char command, unsigned char* sendData, unsigned char sendLen, unsigned char* backData, unsigned int* backLen) {
    unsigned char status = MI_ERR;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;

    MFRC522_Write(CommandReg, PCD_IDLE);  // Stop any active command.
    MFRC522_Write(ComIrqReg, 0x7F);  // Clear all the interrupt flags
    MFRC522_Write(FIFOLevelReg, 0x80);  // FlushBuffer=1, FIFO Initialization

    // Writing data to the FIFO
    for (i = 0; i < sendLen; i++) {   
        MFRC522_Write(FIFODataReg, sendData[i]);    
    }

    // Execute the command
    MFRC522_Write(CommandReg, command);
    if (command == PCD_TRANSCEIVE) {    
        MFRC522_Write(BitFramingReg, 0x80);  // Start transmission
    }
    
    // Waiting for a response from the card
    i = 2000; // Adjust for response time
    do {
        n = MFRC522_Read(FIFOLevelReg);
        i--;
    } while ((i != 0) && !(n & 0x80));  // Wait until the transmission is complete

    if (i != 0) {
        // Check for errors
        if (!(MFRC522_Read(ErrorReg) & 0x13)) { 
            status = MI_OK;
            if (command == PCD_TRANSCEIVE) {
                n = MFRC522_Read(FIFOLevelReg);
                lastBits = MFRC522_Read(ControlReg) & 0x07;
                if (lastBits) {
                    *backLen = (n - 1) * 8 + lastBits;
                } else {
                    *backLen = n * 8;
                }

                if (n == 0) {
                    n = 1;
                }
                if (n > MAX_LEN) {
                    n = MAX_LEN;
                }

                // Reading the received data from FIFO
                for (i = 0; i < n; i++) {
                    backData[i] = MFRC522_Read(FIFODataReg);
                }
            }
        } else {
            status = MI_ERR;
        }
    }

    MFRC522_Write(CommandReg, PCD_IDLE); // Stop any active command

    return status;
}

unsigned char MFRC522_Anticoll(unsigned char* serNum) {
    unsigned char status;
    unsigned char i;
    unsigned char serNumCheck = 0;
    unsigned int unLen;

    MFRC522_Write(BitFramingReg, 0x00);  // TxLastBists = BitFramingReg[2..0]
    
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_OK) {
        // Check card serial number
        for (i = 0; i < 4; i++) {   
          serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i]) {   
          status = MI_ERR;    
        }
    }

    return status;
}
