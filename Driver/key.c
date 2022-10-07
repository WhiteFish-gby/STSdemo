#include "key.h"
#include "STS.h"
#include "buzzer.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
void KEY_server(unsigned int Task_ID)
{
    static uint8_t keyValue = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        if ((~GPIOA->IDR) & (1 << 0))
        {
            keyValue |= 1 << i;
        }
        else
        {
            keyValue = 0;
        }
    }
    if (!((~keyValue) & 0b11111111))
    {
        setBZSignal(BZSignal_Stop);
    }
}

void drv_init_key(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
    MODIFY_REG(GPIOA->MODER, 0b11 << 0U, 0b00 << 0U);
    MODIFY_REG(GPIOA->OTYPER, 1 << 0U, 0 << 0U);
    MODIFY_REG(GPIOA->PUPDR, 0b11 << 0U, 0b01 << 0U);

    taskIDs[STS_TaskID_KEY] = STS_Add_Task(STS_Task_Trigger_Mode_RoughTime, 0.005f, 0, KEY_server);
}