/* 
    ## ADC
    1.A function exists to initialize the ADC
    2.Uses the A0 pin as an input
*/

#include "adc.h"
#include <avr/io.h>

void initADC(){

    //AVCC with external capacitor at AREF pin; uses Vcc which is 5V
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    //ADLAR = 0; rigbt justified
    ADMUX &= ~(1 << ADLAR);

    // Specify ADC input channel and mode
    //Set ADC0 as single-ended input with MUX[5:0] = 0b000000
    ADMUX &= ~((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    ADCSRB &= ~(1 << MUX5);

    // set Auto Trigger Source Selection
    // free running mode ADTS[2:0] = 0b000
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

    // enable ADC and enable auto-triggering and enable ADC interrupts
    ADCSRA|= (1 << ADEN) | (1 << ADATE);


    // set the ADC clock frequency.  Use a pre-scaler of 128
    // Sampling rate is 1/ ((1/125K Hz )*(13 clock cycles)) = 9615 Hz
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // disable ADC0 pin digital input - pin A0 on board
    DIDR0 |= (1 << ADC0D);

    // start the first ADC conversion
    ADCSRA |= ( 1 << ADSC);
}


unsigned int read_adc(){
    unsigned int val = ADCL;
    val +=  ((unsigned int) ADCH) << 8;

    return val;
}
