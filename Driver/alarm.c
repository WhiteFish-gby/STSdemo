#include "alarm.h"
#include "STS.h"
#include "buzzer.h"
#include "uart.h"
void Alarm_server(unsigned int Task_ID)
{
    // sprintf(tmpchar, "\nalarm\n");
    // HAL_UART_Transmit(&huart1, tmpchar, strlen(tmpchar), 1);
    setBZSignal(BZSignal_Continue1);
}
void drv_init_alarm(void)
{

    taskIDs[STS_TaskID_Alarm] = STS_Add_Task(STS_Task_Trigger_Mode_RoughTime, 30*60.0f, 0, Alarm_server);
}