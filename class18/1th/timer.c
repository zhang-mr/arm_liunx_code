#include "s3c2440_soc.h"
#include "led.h"

void tiemr0_irq(unsigned      	int irq );

void timer_init(void) {
	/* 设置timer0时钟 */
	/* timerclk = PCLK/(prescaler value + 1)/(divider value) */
	TCFG0 = 99;
	TCFG1 &= ~(0xf);
	TCFG1 |= 3;

	/* 设置timer0初值 */
	TCNTB0 = 15625;

	/* 加载初值，启动timer0 */
	TCON |= (1<<1);
	
	/* 设置为自动加载并启动 */
	TCON  &= ~(1<<1);
	TCON |= (1<<0) |(1<<3);

	register_irq(10, tiemr0_irq);
	
}


void tiemr0_irq(unsigned      	int irq ) {
	unsigned int val = EINTPEND;
	
	do_led_1();
	
	EINTPEND = 	val;
	
}


