#include "STS.h"
#include "uart.h"
typedef struct
{
    STS_Task_Trigger_Mode mode;
    TIME last_trigger_time;
    float t;
    bool (*trigger_func)(unsigned int Task_ID);
    bool hiden;
    void (*main_func)(unsigned int Task_ID);
} STS_Task;

uint8_t taskIDs[STS_MAX_TASK] = {0};
static STS_Task tasks[STS_MAX_TASK] = {0};
static int16_t STS_final_task = -1;

unsigned int STS_Add_Task(STS_Task_Trigger_Mode mode, float t, bool (*trigger_func)(unsigned int Task_ID), void (*main_func)(unsigned int Task_ID))
{
    int16_t available_id = -1;

    // add taskID to tasklist
    for (uint16_t i = 0; i < STS_MAX_TASK; ++i)
    {
        if (tasks[i].main_func == 0)
        {
            available_id = i;
            break;
        }
    }

    // tasklist is full
    if (available_id < 0)
        return 0;

    // custom use error
    if (mode == STS_Task_Trigger_Mode_Custom && trigger_func == 0)
        return 0;
    // time use error
    else if (t < 0)
        return 0;

    // update STS_final_task
    if (STS_final_task < available_id)
        STS_final_task = available_id;

    // config task
    tasks[available_id].mode = mode;
    tasks[available_id].t = t;
    tasks[available_id].trigger_func = trigger_func;
    tasks[available_id].main_func = main_func;
    tasks[available_id].hiden = false;
    return available_id;
}

unsigned int STS_Get_TaskID(STS_TaskID taskName)
{
    return taskIDs[taskName];
}

bool STS_Change_Task_Mode(unsigned int Task_ID, STS_Task_Trigger_Mode mode, float t, bool (*trigger_func)(unsigned int Task_ID))
{
    if (Task_ID >= STS_MAX_TASK)
        return false;
    if (tasks[Task_ID].main_func == 0)
        return false;

    tasks[Task_ID].mode = mode;
    tasks[Task_ID].t = t;
    tasks[Task_ID].trigger_func = trigger_func;
    tasks[Task_ID].hiden = false;
    return true;
}

bool STS_Change_Task_MainFunc(unsigned int Task_ID, void (*main_func)(unsigned int Task_ID))
{
    if (Task_ID >= STS_MAX_TASK)
        return false;
    if (tasks[Task_ID].main_func == 0)
        return false;

    tasks[Task_ID].main_func = main_func;
    return true;
}

bool STS_Remove_Task(unsigned int Task_ID)
{
    if (Task_ID >= STS_MAX_TASK)
        return false;
    if (tasks[Task_ID].main_func == 0)
        return false;

    if (STS_final_task == Task_ID)
    {
        int16_t final_task = -1;
        for (uint16_t i = 0; i < STS_MAX_TASK; ++i)
        {
            if (tasks[i].main_func == 0)
            {
                final_task = i;
            }
        }
        STS_final_task = final_task;
    }

    tasks[Task_ID].main_func = 0;
    return true;
}

bool STS_Hide_Task(unsigned int Task_ID)
{
    if (Task_ID >= STS_MAX_TASK)
        return false;
    if (tasks[Task_ID].main_func == 0)
        return false;
    tasks[Task_ID].hiden = true;
    return true;
}

bool STS_Reveal_Task(unsigned int Task_ID)
{
    if (Task_ID >= STS_MAX_TASK)
        return false;
    if (tasks[Task_ID].main_func == 0)
        return false;
    tasks[Task_ID].hiden = false;
    return true;
}

void STS_Run()
{
    while (1)
    {
        for (uint16_t i = 0; i <= STS_final_task; ++i)
        {
            if ((tasks[i].main_func != 0) && (tasks[i].hiden == false))
            {
                switch (tasks[i].mode)
                {
                case STS_Task_Trigger_Mode_RoughTime:
                {
                    float pass_time = get_pass_time(tasks[i].last_trigger_time);
                    // if (i == 0)
                    // {
                    //     // sprintf(tmpchar, "\nLAST=%f\n", tasks[i].last_trigger_time.t);
                    //     // HAL_UART_Transmit(&huart1, tmpchar, strlen(tmpchar), 1);
                    //     sprintf(tmpchar, "\n%f\n", pass_time);
                    //     HAL_UART_Transmit(&huart1, tmpchar, strlen(tmpchar), 1);
                    // }

                    if (pass_time >= tasks[i].t)
                    {
                        tasks[i].main_func(i);
                        tasks[i].last_trigger_time = get_TIME_now();
                    }
                    break;
                }

                case STS_Task_Trigger_Mode_PreciseTime:
                {
                    float pass_time = get_pass_time_precise(&tasks[i].last_trigger_time);
                    if (pass_time >= tasks[i].t)
                        tasks[i].main_func(i);
                    break;
                }

                case STS_Task_Trigger_Mode_Custom:
                {
                    if (tasks[i].trigger_func(i))
                    {
                        tasks[i].main_func(i);
                        tasks[i].last_trigger_time = get_TIME_now();
                    }
                    break;
                }
                }
            }
        }
    }
}