#include "lcd.h"
#include "framebuffer.h"
#include "font.h"
#include "geometry.h"


void lcd_test(void) {
    unsigned int fb_base;
    int xres, yres, bpp;
    int x, y;
    unsigned char *p0;
    unsigned short *p;
    unsigned int *p2;

    lcd_init();

    lcd_enable();

    get_lcd_params(&fb_base, &xres, &yres, &bpp);

    fp_get_lcd_params();
    
    font_init();

    if (bpp == 8)
	{
		/* 让LCD输出整屏的红色 */

		/* bpp: palette[12] */

		p0 = (unsigned char *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p0++ = 12;

		/* palette[47] */
		p0 = (unsigned char *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p0++ = 47;

		/* palette[88] */
		p0 = (unsigned char *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p0++ = 88;

		/* palette[0] */
		p0 = (unsigned char *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p0++ = 0;
			
	}
	else if (bpp == 16)
	{
		/* 让LCD输出整屏的红色 */

		/* 565: 0xf800 */

		p = (unsigned short *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p++ = 0xf800;

		/* green */
		p = (unsigned short *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p++ = 0x7e0;

		/* blue */
		p = (unsigned short *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p++ = 0x1f;

		/* black */
		p = (unsigned short *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p++ = 0;
			
	}
	else if (bpp == 32)
	{
		/* 让LCD输出整屏的红色 */

		/* 0xRRGGBB */

		p2 = (unsigned int *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p2++ = 0xff0000;

		/* green */
		p2 = (unsigned int *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p2++ = 0x00ff00;

		/* blue */
		p2 = (unsigned int *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p2++ = 0x0000ff;

		/* black */
		p2 = (unsigned int *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p2++ = 0;

	}
    draw_line(0,0,xres,yres,0xff0000);
    draw_line(0,yres,xres,0,0x00ff00);

    draw_circle(xres/2, yres/2, yres/4,0x0000ff);

    fb_print_string(1,1,"zhangxian hahahahah\r\nwindows inside", 0xf0f000);
    
}

