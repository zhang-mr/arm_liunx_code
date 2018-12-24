#include "lcd.h" 
#define LCD_FB_BASE 0x33000000

lcd_params lcd_4_3_params = {
   .name = "lcd_4.3",
   .pins_pol = {
        .de    = NORMAL,
        .pwren = NORMAL,
        .vclk  = NORMAL,
        .rgb   = NORMAL,
        .hsync = INVERT,
        .vsync = INVERT,
   },
   .time_seq = {
        .tvp = 2,
        .tvb = 33,
        .tvf = 9,

        .thp = 96,
        .thb = 48,
        .thf = 16,

        .vclk = 9,
   },
   .xres = 640,
   .yres = 480,
   .bpp = 16,
   .fb_base = LCD_FB_BASE,  
};


void lcd_4_3_add(void) {
    register_lcd(&lcd_4_3_params);
    
}
