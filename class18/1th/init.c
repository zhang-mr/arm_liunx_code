#include "init.h"
#include "s3c2440_soc.h"


void sdram_init(void) {
	BWSCON = 0x22000000;
	BANKCON6 = 0x18005;
	REFRESH = 0x9c03eb;
	BANKSIZE = 0xb1;
	MRSRB6 = 0x30;
}

//	void copy_data_sdram(volatile unsigned int *src,volatile unsigned int *dest, unsigned int len,) {
//		unsigned int i = 0;
//	
//		while(i < len) {
//			*dest++ = *src++;
//			i += 4;
//		}
//	}
//	
//	void clean_bss_sdram(volatile unsigned int *start,volatile unsigned int *end) {
//		while(start <= end) {
//			*start++ = 0;
//		}
//	}

void copy_data_sdram(void) {
	// 从lds文件中获得
	extern int __code_start,__bss_start;

	volatile unsigned int *dest = (volatile unsigned int *)&__code_start;
	volatile unsigned int *end = (volatile unsigned int *)&__bss_start;
	volatile unsigned int *src = (volatile unsigned int *)0;
	
	while(dest < end) {
		*dest++ = *src++;
	}
}

void clean_bss_sdram(void) {
	// 从lds文件中获得
	extern int __bss_start,__end;

	volatile unsigned int *start = (volatile unsigned int *)&__bss_start;
	volatile unsigned int *end = (volatile unsigned int *)&__end;
	while(start <= end) {
		*start++ = 0;
	}
}






