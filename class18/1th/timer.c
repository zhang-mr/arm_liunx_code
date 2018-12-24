#include "s3c2440_soc.h"
#include "led.h"

void tiemr0_irq(unsigned      	int irq );

void timer_init(void) {
	/* ����timer0ʱ�� */
	/* timerclk = PCLK/(prescaler value + 1)/(divider value) */
	TCFG0 = 99;
	TCFG1 &= ~(0xf);
	TCFG1 |= 3;

	/* ����timer0��ֵ */
	TCNTB0 = 15625;

	/* ���س�ֵ������timer0 */
	TCON |= (1<<1);
	
	/* ����Ϊ�Զ����ز����� */
	TCON  &= ~(1<<1);
	TCON |= (1<<0) |(1<<3);

	register_irq(10, tiemr0_irq);
	
}


void tiemr0_irq(unsigned      	int irq ) {
	unsigned int val = EINTPEND;
	
	do_led_1();
	
	EINTPEND = 	val;
	
}


