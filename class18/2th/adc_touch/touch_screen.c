#include "../s3c2440_soc.h"

unsigned int x_dot = 0;
unsigned int y_dot = 0;
unsigned char flag_reflash = 0;
static unsigned int isanxia = 0;


#define ADC_INT_BIT         (10) 
#define TC_INT_BIT          (9)

#define INT_ADC_TC          (31)

#define  WAIT_PEN_DAWN      (0<<8)
#define  WAIT_PEN_UP        (1<<8)

#define YM_ENABLE           (1<<7)
#define YM_DISABLE          (0<<7)

#define YP_ENABLE           (0<<6)
#define YP_DISABLE          (1<<6)

#define XM_ENABLE           (1<<5)
#define XM_DISABLE          (0<<5)

#define XP_ENABLE           (0<<4)
#define XP_DISABLE          (1<<4)

#define PULLUP_ENABLE       (0<<3)
#define PULLUP_DISABLE      (1<<3)

#define AUTO_PST            (1<<2) 

#define WAIT_INT_MODE       (3)
#define NO_OPR_MODE         (0)


void  IRQ_AdcTouch(void)
{	
    unsigned int  point_adc;
	int  i;

    INTSUBMSK |= ((1<<9)|(1<<10));     // ��ֹADC�жϺʹ������ж�
	
	if(ADCTSC & 0x100)
	{	
	    ADCTSC &= 0x0ff;                       // ���õȴ��±��ж�
	}
	else
	{	
		ADCTSC = (0<<8)|(0<<7)|(1<<6)|(1<<5)|(0<<4)|(1<<3)|(0<<2)|(1<<0);	// �Ȳ���x����
		for(i=0; i<10; i++);		
		point_adc = 0;
		for(i=0;i<4;i++)		// ����4��ת������
		{	
		    ADCCON = ADCCON | (1<<0);		// ����ADC    
    		while(ADCCON & 0x01);			// �ȴ�ADC����        
    		while(!(ADCCON & 0x8000));		// �ȴ�ADC���
    		point_adc = point_adc + (ADCDAT0 & 0x3FF);
    	}
    	x_dot = point_adc>>2; 			// ����ƽ��ֵ
    	
		printf("x:%d\r\n",x_dot);
		ADCTSC = (0<<8)|(1<<7)|(0<<6)|(0<<5)|(1<<4)|(1<<3)|(0<<2)|(2<<0);	// ����y����
		for(i=0; i<10; i++);								
		point_adc = 0;
		for(i=0;i<4;i++)		// ����ת������
		{	
		    ADCCON = ADCCON | (1<<0);		// ����ADC    
    		while(ADCCON & 0x1);			// �ȴ�ADC����        
    		while(!(ADCCON & 0x8000));		// �ȴ�ADC���
    		point_adc = point_adc + (ADCDAT1 & 0x3FF);
    	}
    	y_dot = point_adc>>2; 			// ����ƽ��ֵ
													
		printf("y:%d\r\n",y_dot);
		// �ٴν���ȴ�ģʽ (����̧���ж�)
		ADCTSC = (1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3<<0);
        flag_reflash = 1;
	}

	// ����жϱ�־	
	SUBSRCPND = SUBSRCPND | (1<<9);	
	INTSUBMSK =~ (1<<9); 
}


void touch_screen_irq(void) {
   
    IRQ_AdcTouch();

} 


void Isr_adc(void) {
    int x = ADCDAT0;
    int y = ADCDAT1;

    if(!(x & (1<<15))) {
        x &= 0x3ff;
        y &= 0x3ff;
    
        printf("x = %d, y = %d,\r\n", x, y);
    }
}

void adcTs_Int_Handle(unsigned int irq) {
    if(SUBSRCPND & (1<<9)) {
       IRQ_AdcTouch();
       //register_timer("touch_screen", touch_screen_irq);
    }else {
        // unregister_timer("touch_screen");
    }
    if(SUBSRCPND & (1<<10)) {
        //Isr_adc();
    }
   
    SUBSRCPND = (1<<9) | (1<<10) ;
}

void adc_ts_int_init(void) {
    SUBSRCPND = (1<<9) | (1<<10) ;

    register_irq(31, adcTs_Int_Handle);

    INTSUBMSK &= ~((1<<9) | (1<<10));
    INTMSK    &= ~(1<<31);
}

void adc_ts_reg_init(void) {
    GPGUP = GPGUP | 0xF000;
    GPGCON = GPGCON | 0xFF000000;
    ADCDLY = 10000;
    ADCCON = (1<<14) | (65<<6) | (0<<3);
    ADCTSC = (0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3<<0);
}

void touchscreen_init(void) {

    adc_ts_reg_init();
    
    adc_ts_int_init();
   
}


