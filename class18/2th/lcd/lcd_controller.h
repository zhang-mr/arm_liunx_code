#ifndef __LCD_CONTROLLER_H
#define __LCD_CONTROLLER_H

#include "lcd.h"

typedef struct led_controller {
	char * name;
	void (*init)(p_lcd_params plcdparams);
	void (*enable)(void);
	void (*disable)(void);
	void (*init_palette)(void);	
}lcd_controller, *p_lcd_controller;

void lcd_controller_add(void);
void lcd_controller_disable(void);
void lcd_controller_enable(void);
int lcd_controller_init(p_lcd_params plcdparams);
int select_lcd_controller(char * name);
int register_lcd_controller(p_lcd_controller plcdcon);


#endif




