#include "lcd.h"
#include "lcd_controller.h"
#include "../s3c2440_soc.h"

#define HCLK 133

void jz2440_lcd_pin_init(void) {

    GPCUP = GPCUP | (0x7E0F<<1);
    GPCCON = (GPCCON & (~(0x3FFC00FF<<2))) | (0x2AA800AA<<2); 

    GPDUP = GPDUP | (0x3F3F<<2);
    GPDCON = (GPDCON & (~(0xFFF0FFF<<4))) | (0xAAA0AAA<<4);

   
}

void s3c2440_lcd_controller_init(p_lcd_params plcdparams) {
    int pixelplace; 
    unsigned int addr;

    jz2440_lcd_pin_init();

	/* [17:8]: clkval, vclk = HCLK / [(CLKVAL+1) x 2]
	 *                   9   = 100M /[(CLKVAL+1) x 2], clkval = 4.5 = 5
	 *                 CLKVAL = 100/vclk/2-1
	 * [6:5]: 0b11, tft lcd
	 * [4:1]: bpp mode
	 * [0]  : LCD video output and the logic enable/disable
	 */
    int clkval = (float)HCLK/plcdparams->time_seq.vclk/2 - 1 + 0.5;

    int bppmode = plcdparams->bpp == 8 ? 0xb : \
                  plcdparams->bpp == 16 ? 0xc : \
                  0xdl;
    LCDCON1 = (1<<8) | (3<<5) | (bppmode<<1) | 0;

    /* [31:24] : VBPD    = tvb - 1
	 * [23:14] : LINEVAL = line - 1
	 * [13:6]  : VFPD    = tvf - 1
	 * [5:0]   : VSPW    = tvp - 1
	 */
    LCDCON2 = ((plcdparams->time_seq.tvb - 1)<<24) | \
              ((plcdparams->yres - 1)<<14)           | \
              ((plcdparams->time_seq.tvf - 1)<<6)    | \
              ((plcdparams->time_seq.tvp - 1)<<0);

    /* [25:19] : HBPD    = thb - 1
     * [18:8]  : HOZVAL  = �� - 1
     * [7:0]   : HFPD    = thf - 1
     */
    LCDCON3 = ((plcdparams->time_seq.thb - 1)<<19)  | \
              ((plcdparams->xres - 1)<<8)             | \
              ((plcdparams->time_seq.thf - 1)<<0);

    /* 
	 * [7:0]   : HSPW	 = thp - 1
	 */ 
	LCDCON4 = ((plcdparams->time_seq.thp - 1)<<0);

    /* �����������ż���, ����16bpp, �����ڴ������ش�ŵĸ�ʽ
     * [12] : BPP24BL
	 * [11] : FRM565, 1-565
	 * [10] : INVVCLK, 0 = The video data is fetched at VCLK falling edge
	 * [9]  : HSYNC�Ƿ�ת
	 * [8]  : VSYNC�Ƿ�ת
	 * [7]  : INVVD, rgb�Ƿ�ת
	 * [6]  : INVVDEN
	 * [5]  : INVPWREN
	 * [4]  : INVLEND
	 * [3]  : PWREN, LCD_PWREN output signal enable/disable
	 * [2]  : ENLEND
	 * [1]  : BSWP
	 * [0]  : HWSWP
	 */
//		 pixelplace = plcdparams->bpp == 32 ? (0) : \
//		              plcdparams->bpp == 16 ? (1) : \
//		              (1<<1);
//	    LCDCON5 = (plcdparams->pins_pol.vclk << 10)  | \
//	              (plcdparams->pins_pol.rgb << 7)      | \
//	              (plcdparams->pins_pol.hsync << 9)    | \
//	              (plcdparams->pins_pol.vsync << 8)    | \
//	              (plcdparams->pins_pol.de << 6)       | \
//	              (plcdparams->pins_pol.pwren << 5)    | \
//	              (1<<11) | pixelplace;
    LCDCON5=(1<<11)|(0<<10)|(1<<9)|(1<<8)|(0<<7)|(0<<6)|(0<<5)|(0<<4)|(0<<3)|(0<<2)|(0<<1)|1;
    
    /* framebuffer��ַ */
	/*
	 * [29:21] : LCDBANK, A[30:22] of fb
	 * [20:0]  : LCDBASEU, A[21:1] of fb
	 */
	addr = plcdparams->fb_base & ~(1<<31);
    LCDSADDR1 = (addr >> 1);


    /* 
	 * [20:0] : LCDBASEL, A[21:1] of end addr
	 */
	addr = plcdparams->fb_base + plcdparams->xres * plcdparams->yres * plcdparams->bpp / 8;
    addr >>= 1;
    addr &= 0x1fffff;
    LCDSADDR2 = addr;

    printf("lcd init succed\r\n");
    
}


void s3c_lcd_controller_enable(void) {
    GPBDAT |= (1<<0);

    LCDCON5 |= (1<<3);

    LCDCON1 |= (1<<0);
}

void s3c_lcd_controller_disable(void) {
    GPBDAT &= ~(1<<0);

    LCDCON5 &= ~(1<<3);

    LCDCON1 &= ~(1<<0);
}

void s3c_2440_lcd_contorller_init_palette(void) {
    volatile unsigned int *palette_base = (volatile unsigned int *)0x4D000400;
    int i;

    int bit = LCDCON1 & (1<<0);

    if(bit) {
        LCDCON1 &= ~(1<<0);
    }
    for(i=0; i<256; i++) {
        *palette_base++ = i*256;
    }
    if(bit) {
        LCDCON1 |= (1<<0);
    }
}

lcd_controller s3c2440_lcd_controller = {
    .name = "s3c2440",
    .init = s3c2440_lcd_controller_init,
    .enable  = s3c_lcd_controller_enable,
    .disable =  s3c_lcd_controller_disable,
    .init_palette = s3c_2440_lcd_contorller_init_palette,
};

void s3c2440_lcd_controller_add(void) {
    register_lcd_controller(&s3c2440_lcd_controller);
    
}






