#ifndef _led_H_
#define _led_H_
#include "stm32f401xc.h"
/*
    param:
        1 :led on
        0 :led off
*/
void Led_Config(uint8_t sw);

/*
    initial led driver;
    create ledtask
*/
void drv_init_LED();
#endif