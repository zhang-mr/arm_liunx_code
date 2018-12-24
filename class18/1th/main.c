#include "s3c2440_soc.h"
#include "uart.h"
#include "my_printf.h"
#include "init.h"
#include "led.h"
#include "nor_flash.h"
#include "nand_flash.h"
#include "adc_touch/adc_test.h"

unsigned char g_char1 = 'A';
unsigned char g_char2 = 'a';

void printException(unsigned int codes,const char *str) {
	puts("\r\n");
	puts("cprs:");
	printHex(codes);puts("  ");
	printHex(0x12345678);
	puts(" ");
	puts(str);
	puts("\r\n");
}

void printfSWIVal(unsigned int swiVal) {
	puts("\r\n");
	puts("swi val:");
	printHex(swiVal);
	puts("\r\n");
}

void printfLocalVal(unsigned int swiVal) {
	puts("\r\n");
	puts("local val:");
	printHex(swiVal);
	puts("\r\n");
}


int sdram_test(void) {
	volatile unsigned char *p = (volatile unsigned char *)0x30003000;

	int i;
    printf("\r\nwrite sdram ...\r\n");
	for (i=0; i<100; i++){
		p[i] = i;
		printf(" 0x%x ",i);
	}
	printf("\r\nread sdram ...\r\n");
	for (i=0; i<100; i++){
		printf(" 0x%x ",p[i]);
	}

	return 0;
	
}

int main(void) {
	unsigned char c;

	led_init();
	nand_init();
	delay(1000);
//		key_eint_init();
//		timer_init();
	lcd_test();
    adc_test();
	nand_flash_test();
	nor_flash_test();	
	//sdram_test();
	while(1) {
		putchar(g_char1);
		putchar(g_char2);
		g_char1++;
		g_char2++;
		delay(1000000);

	}
	return 0;
}

