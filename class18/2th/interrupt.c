#include "s3c2440_soc.h"
#include "led.h"


typedef  void (*irq_func)(unsigned int irq);
irq_func irq_array[40];     // 放入各个中断的服务函数

void key_eint_irq(unsigned int irq);

void register_irq(unsigned        int irq, irq_func fp) {
	irq_array[irq] = fp;
	INTMSK &= ~(1<<irq);
}


void key_eint_init() {
	/* 配置GPF4为中断引脚 */	
	GPFCON &= ~(3<<8);
	GPFCON |= (2<<8);
	/* 设置中断触发方式，双边沿触发 */
	EXTINT0 |= (7<<16);
	/* 中断屏蔽寄存器 */
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
	/* 分辨中断源 */
	int bit = INTOFFSET;
	
	/* 调用不同的处理函数 */

	irq_array[bit](bit);
//		if(bit == 4) {
//			key_eint_irq(bit);     // 处理中断然后 清中断源EINTPEND
//		} else if(bit == 10) {
//			tiemr0_irq(bit);
//		}
	/* 清中断 从源头开始清*/
	SRCPND = (1<<bit);
	INTPND = (1<<bit);
	
}


