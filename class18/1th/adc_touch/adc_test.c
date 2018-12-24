#include "adc.h"
#include "../my_printf.h"
#include "../lcd/font.h"

void adc_test(void) {
    int val;
    char str[100];

    adc_init();
    printf("adc_test start\r\n");
    printf("\r\n");

    while(1) {
        val = adc_read_ain1();
        val = (int)val * 3.2258;
        
        printf("vol: %d.%03dV\r",val/1000,val%1000);
        
        sprintf(str, "vol: %d.%03dv",val/1000,val%1000);
        
        fb_print_string(0, 32, str, 0x00ff00);
        delay(100000);
    }

}
