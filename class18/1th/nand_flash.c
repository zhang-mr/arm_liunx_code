#include "s3c2440_soc.h"
#include "my_printf.h"
#include "string_utils.h"
#include "uart.h"

void nand_init(void) {
#define		TACLS		1
#define		TWRPH0		2
#define		TWRPH1		0
	/* …Ë÷√ ±–Ú */
	NFCONF = (1<<15)|(1<<12)|(TACLS<<8)|(TWRPH0<<4)|(TWRPH1<<0);

}

void nand_select(void) {
	NFCONT &= ~(1<<12);
}

void nand_deselect(void) {
	NFCONT |= (1<<12);
}

void nand_cmd(unsigned char cmd) {
	volatile int i;
	NFCCMD = cmd;
	for(i=0; i<20; i++);
}

void nand_addr_byte(unsigned char addr) {
	volatile int i;

	NFADDR = addr;
	for(i=0; i<20; i++);
}

unsigned char nand_data(void) {
	return NFDATA;
} 

void nand_w_data(unsigned char val) {
	NFDATA = val;
}
void nand_chip_id(void) {
	unsigned char buf[5] = {0};

	nand_select();
	nand_cmd(0x90);
	nand_addr_byte(0x00);

	buf[0] = nand_data();
	buf[1] = nand_data();
	buf[2] = nand_data();
	buf[3] = nand_data();
	buf[4] = nand_data();

	nand_deselect();

	printf("maker    id = 0x%x\r\n",buf[0]);
	printf("device   id = 0x%x\r\n",buf[1]);
	printf("3rd byte    = 0x%x\r\n",buf[2]);
	printf("4rd byte    = 0x%x\r\n",buf[3]);
	printf("page  size  = %dkb\r\n",1<<(buf[3]&0x03));
	printf("block size  = %dkb\r\n",64<<((buf[3]>>4)&0x03));
	printf("5rd byte    = 0x%x\r\n",buf[4]);
	printf("\r\n");
}

void nand_wait_ready() {
	while(!(NFSTAT & 0x01));
}

void nand_read(unsigned int addr, unsigned char *buff, unsigned int len) {
	int i = 0;
	int page = 0;
	int col = 0;

	nand_select();
		
	while(1) {
		page = addr/512;
		col = addr%512;
		if(col > 255) {
			nand_cmd(0x01);
		} else {
			nand_cmd(0x0);
		}
		
		nand_addr_byte(col & 0xff);
		
		nand_addr_byte(page & 0xff);
		nand_addr_byte((page>>8) & 0xff);
		nand_addr_byte((page>>16) & 0xff);

		nand_wait_ready();

		for(;(col<256)&&(i<len); col++) {
			buff[i++] = nand_data();
		}
		if(i == len) {
			break;
		}
		addr += (256-col);
		col = 0;
		 
	}
	nand_deselect();
}

void nand_erase(unsigned int addr, int len) {
	int page = 0, col = 0;
	
	nand_select();
	while(1) {
		page = addr/512;
		col = addr%512;
	
		nand_cmd(0x60);
		nand_addr_byte(page & 0xff);
		nand_addr_byte((page>>8) & 0xff);
		nand_addr_byte((page>>16) & 0xff);
		nand_cmd(0xD0);
		nand_wait_ready();

		len -= (512 - col);

		if(len < 0) {
			break;
		}
		page++;
		col = 0;
	}

	nand_deselect();
}


void nand_write(unsigned int addr, unsigned char *buff, unsigned int len) {
	int i = 0;
	int page = addr/512;
	int col = addr&(512 - 1);

	nand_select();

	while(1) {
		nand_cmd(0x80);
		nand_addr_byte(col & 0xff);
		
		nand_addr_byte(page & 0xff);
		nand_addr_byte((page>>8) & 0xff);
		nand_addr_byte((page>>16) & 0xff);

		for(;(col<512)&&(i<len); col++) {
			nand_w_data(buff[i++]);
		}
		nand_cmd(0x10);
		nand_wait_ready();
		
		if(i == len) {
			break;
		} else {
			col = 0;
			page++;
		}
		 
	}
	nand_deselect();
}

void do_nand_flash_erase(void) {
	unsigned int addr;

	printf("Enter te address of sector to erase: ");
	addr = get_uint();

	printf("erasint ...\r\n");
	
	nand_erase(addr,512);
	
}

void do_nand_flash_write(void) {
	unsigned int addr;
	unsigned char str[100];
	int i = 0;
	unsigned int val;

	printf("Enter the address of sector to wirte: ");
	addr = get_uint();

	printf("Enter the string to write: ");
	gets(str);
	
	printf("writing ...\r\n");

	nand_write(addr, str, strlen(str)+1);
	
}

void do_nand_flash_read(void) {
	unsigned char i, j, tmp;
	unsigned int addr = 0;
	unsigned char str[64];
	
	printf("Enter the address to read:");
	addr = get_uint();
	nand_read(addr, str, 64);

	for(i=0; i<4; i++) {
		for(j=0; j<16; j++) {
			printf("%02x ", str[i*16+j]);	
		}
		printf("; ");
		for(j=0; j<16; j++) {
			tmp = str[i*16+j];
			if(tmp <0x20 || tmp > 0x7e) {
				putchar('.');
			} else {
				putchar(tmp);
			}
		}
		printf("\r\n");	
	}	
	printf("\r\n");	
	
}

void nand_flash_test(void) {
	char c;
	
	while(1) {
		printf("[s] Scan  nand flash\r\n");
		printf("[e] Erase nand flash\r\n");
		printf("[w] Write nand flash\r\n");
		printf("[r] Read  nand flash\r\n");
		printf("[q] quit\r\n");
		printf("Enter selection: ");

		c = getchar();

		printf("%c\r\n", c);

		switch(c) {
			case 'q':
			case 'Q':
				return;
				break;
			case 'e':
			case 'E':
				do_nand_flash_erase();
				break;
			case 's':
			case 'S':
				nand_chip_id();
				break;
			case 'w':
			case 'W':
				do_nand_flash_write();
				break;
			case 'r':
			case 'R':
				do_nand_flash_read();
				break;

			default :
				break;
		}
	}
}




