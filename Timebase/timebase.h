#pragma once

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define SYS_Frequence 84e6


#pragma region TIME
    extern const float TIMsec1;

    typedef struct 
    {
        uint64_t t;//unit: 1/sysclock seconds
    }TIME;

    /*
        get current time
    */
    TIME get_TIME_now();

    /*
        get passed time from last_time    uint : 1 seconds
    */
    float get_pass_time(TIME last_time);

    /*
        get precise passed time from last_time
        set last_time as current time
        uint : 1 seconds
    */
    float get_pass_time_precise(TIME *last_time);

    /*
        get the difference between time_a and time_b
        uint : 1 seconds
    */
    float get_time_difference(TIME time_a, TIME time_b);

    /*
        get system runtime
        uint : 1 seconds
    */
    float get_system_runtime();

    /*
        set TIME useless
    */
    void time_inValid(TIME *t);

    /*
        is TIME useless? yes/no 1/0
    */
    bool time_isValid(TIME t);

    /*
        wait t time(by while)
    */
    void delay(float t);
#pragma endregion TIME


