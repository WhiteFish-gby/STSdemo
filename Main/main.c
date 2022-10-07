#include "timebase.h"
#include "led.h"
#include "buzzer.h"
#include "key.h"
#include "alarm.h"
#include "uart.h"
int main(void)
{
    SysInit();
    drv_init_LED();
    drv_init_BUZZER();
    drv_init_alarm();
    drv_init_key();
    drv_init_uart();
    STS_Run();
}
