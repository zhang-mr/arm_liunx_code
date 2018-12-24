#include "adc.h"
#include "../my_printf.h"
#include "../lcd/font.h"
#include "../adc_touch/touch_screen.h"

void touch_screen_test(void) {
    char str[100];
    touchscreen_init();

    while(1) {
         if(flag_reflash == 1) {
            flag_reflash = 0;
            sprintf(str, "x position %d ;\r\ny position %d ", x_dot, y_dot);        
            fb_print_string(0, 64, str, 0x00ff00);
        }
    }
}
