#ifndef _buzzer_H
#define _buzzer_H
#include <stdbool.h>
enum BuzzerSignal
{
    BZSignal_Start1,
    BZSignal_Start2,

    BZSignal_Continue1,
    BZSignal_Success1,

    BZSignal_Stop,
};



void drv_init_BUZZER(void);

enum BuzzerSignal get_bzSignal();
void setBZSignal(enum BuzzerSignal signal);
void set_BuzzerFreq(unsigned short freq);

void set_BuzzerOnOff(bool on);
#endif