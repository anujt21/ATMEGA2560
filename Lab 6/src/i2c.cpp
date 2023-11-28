// Initialization for I2C and a function sendatai2c that sends data using I2C
#include <avr/io.h>
#include <Arduino.h>
#include "i2c.h"

#define wait_for_completion while(!(TWCR & (1 << TWINT)));

void initI2C() 
{
  // wake up from power reduction:   if the I2C is in power reduction mode (PRTWI bit is 1) 
  //then it needs to be restored by writing a zero to PRTWI.
  
  PRR0 &= ~(1 << PRTWI);  //On pg. 236 of ATmega2560 Datasheet   
  
  // generate a 100kHz clock rate
  // SCL freq = 16Mhz/(16 + 2(TWBR)*4)
  // Table 24-7 Prescaler value is 4 so TWPS0 = 1 and TWPS1 = 0.
  TWSR |= (1 << TWPS0);
  TWSR &= ~(1 << TWPS1);

  TWBR = 0x12; // bit rate generator = 100k  (TWBR = 18)
  //TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)

  TWCR |= (1 << TWINT | 1 << TWEN); // enable two wire interface
  
}


void StartI2C_Trans(unsigned char SLA){

 TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);  // reset flag and intiate a start condition
 wait_for_completion;             // wait while flag is low

 while((TWSR & 0xF8) != 0x08);    // check for acknowledgement

 TWDR = (SLA << 1);      //load i2c address and write bit (0)  
 TWCR = (1<< TWINT) | (1<<TWEN);  // clear flag and enable TWI
 
 wait_for_completion;
}


void StopI2C_Trans(){
 
 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); // trigger action and stop condition

}


void Read_from(unsigned char SLA, unsigned char MEMADDRESS){
  Serial.println("Starting Read");

  StartI2C_Trans(SLA);
  write(MEMADDRESS);

  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // restart to switch to read mode
  wait_for_completion;
  
  TWDR = (SLA << 1) | 0x01;               // slave address plus read bit
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
  wait_for_completion;
  
  TWCR = (1<< TWINT) | (1 << TWEN);       // master can send a nacknow
  wait_for_completion;
  StopI2C_Trans();

}


void write(unsigned char data){
  TWDR = data;  // set the register to data 

  TWCR = (1<<TWINT) | (1<<TWEN);  // trigger action

  wait_for_completion;            
}


unsigned char Read_data(){
  return TWDR;  // return data stored in TWDR
}