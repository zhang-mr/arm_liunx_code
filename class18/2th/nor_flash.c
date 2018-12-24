#include "my_printf.h"
#include "string_utils.h"

#define NOR_FLASH_BASE	  0

void nor_write_word(unsigned int base, unsigned int offset, unsigned int val) {
	volatile unsigned short *p = (volatile unsigned short *)(base + (offset<<1));
	*p = val;
}
 

void nor_cmd(unsigned int offset, unsigned int cmd) {
	nor_write_word(NOR_FLASH_BASE, offset, cmd);
}

unsigned int nor_read_word(unsigned int base, unsigned int offset) {
	volatile unsigned int short *p = (volatile unsigned int short *)(base + (offset<<1));

	return *p;
}

unsigned int nor_dat(unsigned int offset) {
	return nor_read_word(NOR_FLASH_BASE, offset);
}

void nor_wait_ready(unsigned int addr) {
	unsigned int val;
	unsigned int pre;

	pre = nor_dat(addr>>1);
	val = nor_dat(addr>>1);
	while((val & (1<<6))!=(pre & (1<<6))){
		pre = val;
		val = nor_dat(addr>>1);
	}	
}

void da_scan_nor_flash(void) {
	char str[4];
	unsigned int size;
	int regions, i;
	int region_info_base;
	int block_addr, blocks, block_size, j;
	int cnt;

	int vendor, device;

	nor_cmd(0x5555, 0xaa);
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(0x5555, 0x90);
	vendor = nor_dat(0);
	device = nor_dat(1);
	nor_cmd(0, 0xf0);

	nor_cmd(0x5555, 0xaa);
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(0x5555, 0x98);
	str[0] = nor_dat(0x10);
	str[1] = nor_dat(0x11);
	str[2] = nor_dat(0x12);
	str[3] = '\0';

	size = 1<< (nor_dat(0x27));
	printf("vendor id = 0x%x, device id = 0x%x, nor_size = 0x%x, %dM\r\n", vendor, device, size, size/1024/1024);
	/* 打印各个扇区的起始地址 */
	/* 名词解释:
	 *    erase block region : 里面含有1个或多个block, 它们的大小一样
	 * 一个nor flash含有1个或多个region
	 * 一个region含有1个或多个block(扇区)

	 * Erase block region information:
	 *    前2字节+1    : 表示该region有多少个block 
	 *    后2字节*256  : 表示block的大小
	 */
	regions = nor_dat(0x2c);
	region_info_base = 0x2d;
	block_addr = 0;
	printf("Block/Sector start address:\r\n");
	cnt = 0;
	for(i=0; i<regions; i++) {
		blocks = 1 + nor_dat(region_info_base)+(nor_dat(region_info_base + 1)<<8);
		block_size = 256 * (nor_dat(region_info_base+2)+(nor_dat(region_info_base+3)<<8));
		region_info_base += 4;
		printf("\n\rregion %d, blocks = %d, block_size = 0x%x, block_addr = 0x%x\n\r", i, blocks, block_size, block_addr);
		for(j=0; j<blocks; j++) {
			printHex(block_addr);
			putchar(' ');
			cnt++;
			block_addr += block_size;
			if(cnt % 6 == 0) {
				printf("\r\n");
			}
		}
	}
	printf("\n\r");
	/* 退出CFI模式 */
	nor_cmd(0, 0xf0);
	

	
}

void do_erase_nor_flash(void) {
	unsigned int addr;

	printf("Enter te address of sector to erase: ");
	addr = get_uint();

	printf("erasint ...\r\n");
	nor_cmd(0x5555, 0xaa);
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(0x5555, 0x80);
	nor_cmd(0x5555, 0xaa);
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(addr>>1, 0x30);
	nor_wait_ready(addr);
}

void do_write_nor_flash(void) {
	unsigned int addr;
	unsigned char str[100];
	int i, j;
	unsigned int val;

	printf("Enter the address of sector to wirte: ");
	addr = get_uint();

	printf("Enter the string to write: ");
	gets(str);

	printf("writing ...\r\n");

	i = 0;
	j = 1;
	while(str[i] && str[j]) {
		val = str[i] + (str[j]<<8);
		nor_cmd(0x5555, 0xaa);
		nor_cmd(0x2aaa, 0x55);
		nor_cmd(0x5555, 0xa0);
		nor_cmd(addr>>1, val);
		nor_wait_ready(addr);
		i += 2;
		j += 2;
		addr += 2;
	}
	val = str[i];
	nor_cmd(0x5555, 0xaa);
	nor_cmd(0x2aaa, 0x55);
	nor_cmd(0x5555, 0xa0);
	nor_cmd(addr>>1, val);
	nor_wait_ready(addr);
	
}

void do_read_nor_flash(void) {
	unsigned int addr;
	volatile unsigned char *p;
	int i, j;
	unsigned char c;
	unsigned char str[16];

	printf("Enter the address to read:");
	addr = get_uint();
	p = (volatile unsigned char *)addr;
	printf("data: \r\n");
	for(i=0; i<4; i++) {
		for(j=0; j<16; j++) {
			c = *p++;
			str[j] = c;
			printf("%02x ",c);
			
		}
		printf(" ; ");
		for(j=0; j<16; j++) {
			if(str[j]<0x20 || str[j] > 0x7e) {
				putchar('.');
			} else {
				putchar(str[j]);
			}
		}
		printf("\r\n");
	}
} 

void nor_flash_test(void) {
	char c;
	
	while(1) {
		printf("[s] Scan  nor flash\r\n");
		printf("[e] Erase nor flash\r\n");
		printf("[w] Write nor flash\r\n");
		printf("[r] Read  nor flash\r\n");
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
				do_erase_nor_flash();
				break;
			case 's':
			case 'S':
				da_scan_nor_flash();
				break;
			case 'w':
			case 'W':
				do_write_nor_flash();
				break;
			case 'r':
			case 'R':
				do_read_nor_flash();
				break;

			default :
				break;
		}
	}
}

