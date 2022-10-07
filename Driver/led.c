#include "led.h"
#include "STS.h"


void Led_Config(uint8_t sw)
{
    //set GPIOC13 as high/low state， 即设置高低电平
    GPIOC->ODR &= 0b0<<13U;
    GPIOC->ODR |= (sw == 0) ? (0b1<<13U) : (0b0<<13U);
    // while(1);
}


static void Led_Toggle(void)
{
    uint32_t odr;
    odr = GPIOC->ODR;
    GPIOC->BSRR = (odr & 0x2000) << 16 | ~odr & 0x2000; // 0x2000 is GPIO_PIN_13
}

static void LED_server(unsigned int Task_ID)
{
    Led_Toggle();
}

void drv_init_LED()
{
    //IO port C clock enable
	RCC->AHB1ENR |= 0b1<<2U;
    // set GPIOC13 as General purpose output mode，即通用输出模式
    GPIOC->MODER &= 0b00<<26U ;  
    GPIOC->MODER |= 0b01<<26U;
    //set GPIOC13 as Output push-pull mode，即推挽输出模式
    GPIOC->OTYPER &= 0b0<<13U;
    GPIOC->OTYPER |= 0b0<<13U;
    //set GPIOC13 output speed as Low speed mode ，即最大8Mhz
    GPIOC->OSPEEDR &= 0b00<<26U;
    GPIOC->OSPEEDR |= 0b00<<26U;
    //set GPIOC13 as pull-down mode，即下拉
    GPIOC->PUPDR &= 0b00<<26U;
    GPIOC->PUPDR |= 0b10<<26U;


    Led_Config(1);
    
    taskIDs[STS_TaskID_LED] = STS_Add_Task(STS_Task_Trigger_Mode_RoughTime, 1.0, 0, LED_server);
}