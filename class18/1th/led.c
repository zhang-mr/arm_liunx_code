#include "s3c2440_soc.h"
#include "uart.h"

//	#define GPECON (*(volatile unsigned int *)0x56000040)
//	#define GPEDAT (*(volatile unsigned int *)0x56000044)
//	#define GPFCON (*(volatile unsigned int *)0x56000050)
//	#define GPFDAT (*(volatile unsigned int *)0x56000054)

void delay(volatile unsigned int ms) {
    unsigned int i;

	for(i=0; i<ms; i++){
		;;
	}
}

void led_init(void) {
	GPECON = 0x01400000;
	GPEDAT = 0x1000;    
}

void do_led_1(void) {
    static unsigned int led_count = 0;
	
	led_count++;
	if(led_count%2 == 0)
		GPEDAT &= ~(1<<11);
	else
		GPEDAT |= (1<<11);
}


