#include "s3c2440_soc.h"
#include "led.h"

#define TIMER_NUM  10
#define NULL ((void *)0)


typedef void (* timer_fun)(void);

typedef struct timer_desc {
    char *name;
    timer_fun fp;
}timer_desc, *p_timer_desc;

timer_desc timer_array[TIMER_NUM];


int register_timer(char *name, timer_fun fp) {
    int i;

    for(i=0; i<TIMER_NUM; i++){
        if(!timer_array[i].fp) {
            timer_array[i].name = name;
            timer_array[i].fp = fp;
            return 0;
        }
    }

    return -1;
}

int unregister_timer(char *name) {
   int i;

    for(i=0; i<TIMER_NUM; i++){
        if((timer_array[i].fp)&&(!strcmp(timer_array[i].name, name))) {
            timer_array[i].name = NULL;
            timer_array[i].fp = NULL;
            return 0;
        }
    }

    return -1;
}

void tiemr0_irq(unsigned int irq) {

    int i;
	
    for(i=0; i<TIMER_NUM; i++){
        if(timer_array[i].fp) {
            timer_array[i].fp();
        }
    }
	
}

void timer_init(void) {
	/* 设置timer0时钟 */
	/* timerclk = PCLK/(prescaler value + 1)/(divider value) */
	TCFG0 = 99;
	TCFG1 &= ~(0xf);
	TCFG1 |= 3;

	/* 设置timer0初值 */
	TCNTB0 = 416;

	/* 加载初值，启动timer0 */
	TCON |= (1<<1);
	
	/* 设置为自动加载并启动 */
	TCON  &= ~(1<<1);
	TCON |= (1<<0) |(1<<3);

	register_irq(10, tiemr0_irq);
	
}




