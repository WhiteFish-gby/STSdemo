#pragma once

#include "timebase.h"

// Simple Task Scheduler
// baiyug 20220813

#define STS_MAX_TASK 20

typedef enum
{
    STS_Task_Trigger_Mode_RoughTime,
    STS_Task_Trigger_Mode_PreciseTime,
    STS_Task_Trigger_Mode_Custom,
} STS_Task_Trigger_Mode;

// users edit here
typedef enum
{
    STS_TaskID_LED,
    STS_TaskID_BUZZER,
    STS_TaskID_KEY,
    STS_TaskID_Alarm,
    STS_TaskID_Uart,
} STS_TaskID;

extern uint8_t taskIDs[STS_MAX_TASK];

/*
parameters:
    t is set when mode is time trigger,otherwise, t set 0,
    trigger_func is customized when mode is Custom trigger,
    main_func is the function running tasks

template for example:
    your task name is defined as STS_TaskID_LED in enum STS_TaskID,then you can do this to create task

    taskIDs[STS_TaskID_LED] = STS_Add_Task(STS_Task_Trigger_Mode_RoughTime, 1.0, 0, LED_server);
*/
unsigned int STS_Add_Task(STS_Task_Trigger_Mode mode, float t, bool (*trigger_func)(unsigned int Task_ID), void (*main_func)(unsigned int Task_ID));

/*
    taskName is defined by user in STS_TaskID,and it is index of the array taskIDs when use function STS_Add_Task.
*/
unsigned int STS_Get_TaskID(STS_TaskID taskName);
/*
    Change Task's trigger mode
*/
bool STS_Change_Task_Mode(unsigned int Task_ID, STS_Task_Trigger_Mode mode, float t, bool (*trigger_func)(unsigned int Task_ID));

/*
    Change Task's running function
*/
bool STS_Change_Task_MainFunc(unsigned int Task_ID, void (*main_func)(unsigned int Task_ID));

/*
    Remove task from tasklist
*/
bool STS_Remove_Task(unsigned int Task_ID);

/*
    Hide task from tasklist
*/
bool STS_Hide_Task(unsigned int Task_ID);

/*
    Reveal the hidden task from tasklist
*/
bool STS_Reveal_Task(unsigned int Task_ID);

/*
    running Simple Task Scheduler
*/
void STS_Run();