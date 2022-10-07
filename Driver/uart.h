#ifndef _UART_H_
#define _UART_H_
#include "stm32f4xx_hal.h"
extern uint8_t tmpchar[64];
extern UART_HandleTypeDef huart1;
void drv_init_uart();
#endif