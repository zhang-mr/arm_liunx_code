#ifndef __UART_H_Z
#define __UART_H_Z

#define SYSCLK_PLCK   67000000

void uart0_init(int ubrdrate);
int putchar(int c);
int getchar(void);
int puts(const char *s);
void printHex(unsigned int val);


#endif


