#include "s3c2440_soc.h"
#include "led.h"


typedef  void (*irq_func)(unsigned int irq);
irq_func irq_array[40];     // ��������жϵķ�����

void key_eint_irq(unsigned int irq);

void register_irq(unsigned        int irq, irq_func fp) {
	irq_array[irq] = fp;
	INTMSK &= ~(1<<irq);
}


void key_eint_init() {
	/* ����GPF4Ϊ�ж����� */	
	GPFCON &= ~(3<<8);
	GPFCON |= (2<<8);
	/* �����жϴ�����ʽ��˫���ش��� */
	EXTINT0 |= (7<<16);
	/* �ж����μĴ��� */
	EINTMASK &= ~(1<<4);

	register_irq(4, key_eint_irq);
}

void key_eint_irq(unsigned int irq) {
	unsigned int val = EINTPEND;
	
	
	if(irq == 0) {
		
	} else if(irq = 4) {
		if(val & (1<<4)) {
			do_led_1();
		}
	} 

	EINTPEND = 	val;
	
}

void handle_irq_c(void) {
	/* �ֱ��ж�Դ */
	int bit = INTOFFSET;
	
	/* ���ò�ͬ�Ĵ����� */

	irq_array[bit](bit);
//		if(bit == 4) {
//			key_eint_irq(bit);     // �����ж�Ȼ�� ���ж�ԴEINTPEND
//		} else if(bit == 10) {
//			tiemr0_irq(bit);
//		}
	/* ���ж� ��Դͷ��ʼ��*/
	SRCPND = (1<<bit);
	INTPND = (1<<bit);
	
}


