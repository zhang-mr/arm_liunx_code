
objs = start.o main.o uart.o my_printf.o lib1funcs.o init.o led.o interrupt.o timer.o nor_flash.o string_utils.o nand_flash.o
expt = touch_test

objs +=  lcd/framebuffer.o
objs +=  lcd/lcd.o
objs +=  lcd/lcd_4_3.o
objs +=  lcd/lcd_controller.o
objs +=  lcd/lcd_test.o
objs +=  lcd/s3c2440_lcd_controller.o
objs +=  lcd/framebuffer.o
objs +=  lcd/font_8x16.o
objs +=  lcd/geometry.o
objs +=  lcd/font.o

objs += adc_touch/adc.o 
objs += adc_touch/adc_test.o 
objs += adc_touch/touch_screen.o 
objs += adc_touch/touch_screen_test.o 

CC = arm-linux-
#CC = 

dep_files := $(patsubst %,.%.d,$(objs))
dep_files := $(wildcard $(dep_files))


CFLAGS = -Werror -Iinclude

$(expt):$(objs)
	$(CC)ld -T sdram.lds  $^ libgcc.a -o $(expt).elf 
	$(CC)objcopy -O binary -S $(expt).elf $(expt).bin
	$(CC)objdump -D $(expt).elf > $(expt).dis
	@echo dep_files = $(dep_files)

ifneq ($(dep_files),)	
include $(dep_files)
endif

%.o:%.c 
	$(CC)gcc $(CFLAGS) -march=armv4   -c -o $@ $< #-MD -MF .$@.d

%.o:%.S
	$(CC)gcc  -march=armv4 -c -o $@ $<
	
clean:
	rm -rf *.o $(expt) *.bin *.elf *.dis .*.*.d
	
.PHONY:clean






