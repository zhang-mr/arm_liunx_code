#include "s3c2440_soc.h"
#include "uart.h"

/* 115200,n,1 */
void uart0_init(int ubrdrate)  {
	/* 设置引脚 */
	GPHCON &= ~((3<<4) | (3<<6));
	GPHCON |= ((2<<4) | (2<<6));
	GPHUP &= ~((1<<2) | (1<<3));  //使能内部上拉
	
	/* 设置波特率 */
	UCON0 = 0x05;
	/* UBRDIVn  = (int)(UART_clock/(buad rate x 16) -1) 
	 * UBRDIVn  = (int)(67M/(115200 x 16) -1) = 35
	 */
	UBRDIV0 = 35;
	
	/* 设置数据格式 8,n,1*/
	ULCON0 = 0x00000003;

	/* 其他寄存器配置 如FIFO          */
	
}

int putchar(int c) {
	while(!(UTRSTAT0 & (1<<2)));
	UTXH0 = (unsigned char) c;
}

int getchar(void) {
	while(!(UTRSTAT0 & (1<<0)));
	return URXH0;	
}

int puts(const char *s) {
	while(*s){
		putchar(*s);
		s++;
	}
}

void printHex(unsigned int val) {
	int i;
	unsigned char arr[8];

	for(i=0; i<8; i++) {
		arr[i] = val &0xf;
		val >>= 4;
	}

	puts("0x");
	for(i=7; i>=0; i--) {
		if( arr[i] <= 9) {
			putchar(arr[i] + '0');
		}else if(arr[i] >= 0x0A && arr[i] <= 0x0F) {
			putchar(arr[i] - 0x0A + 'A');
		}else {
			
		}
	}
}


