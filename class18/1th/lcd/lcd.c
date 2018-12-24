#include "lcd.h" 
#include "lcd_controller.h" 

#define LCD_NUM 10

static p_lcd_params p_array_lcd[LCD_NUM]={0} ;
static p_lcd_params g_p_lcd_selected;


int register_lcd(p_lcd_params plcd) {
    int i;

    for (i = 0; i < LCD_NUM; i++) {
        if(!p_array_lcd[i]) {
            p_array_lcd[i] = plcd;
            printf("add lcd suceed\r\n");
         return i; 
        }  
    }

    return -1;
}

int select_lcd(char *name) {
    int i;

    for (i = 0; i < LCD_NUM; i++) {
        if((p_array_lcd[i]) && (!strcmp(p_array_lcd[i]->name, name))) {
            g_p_lcd_selected = p_array_lcd[i];
            printf("selecte lcd suceed\r\n");
            return i; 
        }  
    }

    return -1;
}

void get_lcd_params(unsigned int * fb_base, int *xres, int *yres, int *bpp) {
    *fb_base    = g_p_lcd_selected->fb_base;
    *xres       = g_p_lcd_selected->xres;
    *yres       = g_p_lcd_selected->yres;
    *bpp        = g_p_lcd_selected->bpp;
}

void lcd_enable(void) {
    lcd_controller_enable();
}

void lcd_disable(void) {
    lcd_controller_disable();
}

int lcd_init(void) {
    lcd_4_3_add();
    lcd_controller_add();
    select_lcd("lcd_4.3");
    select_lcd_controller("s3c2440");
    lcd_controller_init(g_p_lcd_selected);
   
}


