#include "../s3c2440_soc.h"


void adc_init(void) {
    ADCCON = (1<<14) | (65<<6) | (1<<3) ;
    ADCDLY = 0x0f;
}
int adc_read_ain1(void) {
    int adcValue = 0;
    
    ADCCON |= (1<<0);

    while(!(ADCCON & (1<<15)));

    return (ADCDAT0 & 0x3ff);
}

