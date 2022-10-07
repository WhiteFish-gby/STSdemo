#include "buzzer.h"
#include "stm32f401xc.h"
#include "stm32f4xx.h"
#include <stdbool.h>
#include "STS.h"

static enum BuzzerSignal bzSignal = BZSignal_Stop;

static float bzTasktime = 0.2f;
void setBZSignal(enum BuzzerSignal signal)
{
	bzSignal = signal;
}

enum BuzzerSignal get_bzSignal()
{
	return bzSignal;
}
void set_BuzzerFreq(unsigned short freq)
{
	if (freq < 200)
		freq = 200;
	TIM3->ARR = 1e6 / freq;
	if (TIM3->CCR4 != 0)
		TIM3->CCR4 = TIM3->ARR / 2;
}

void set_BuzzerOnOff(bool on)
{
	if (on)
		TIM3->CCR4 = TIM3->ARR / 2;
	else
		TIM3->CCR4 = 0;
}
static inline void BZRefresh()
{
	switch (bzSignal)
	{
	case BZSignal_Start1:
	{
		set_BuzzerFreq(900);
		bzSignal++;
		break;
	}
	case BZSignal_Start2:
	{
		set_BuzzerOnOff(true);
		break;
	}
	case BZSignal_Continue1:
	{
		set_BuzzerFreq(1500);
		bzSignal++;
		break;
	}
	case BZSignal_Success1:
	{
		set_BuzzerOnOff(true);
		break;
	}
	case BZSignal_Stop:
	{
		set_BuzzerOnOff(false);
		break;
	}
	default:
	{ //
		break;
	}
	}
}
static void BUZZER_server(unsigned int Task_ID)
{
	BZRefresh();
}
void drv_init_BUZZER(void)
{
	/*
		Buzzer(TIM3_CH4) PB1
	*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN_Msk;

	// set GPIOB1 at Alternate function mode
	MODIFY_REG(GPIOB->MODER, 0b11 << 2U, 0b10 << 2U);

	// set GPIOB1 at Output push-pull mode
	MODIFY_REG(GPIOB->OTYPER, 1 << 1U, 0 << 1U);
	// configure the I/O GPIOB1 Pull-down
	MODIFY_REG(GPIOB->PUPDR, 0b11 << 2U, 0b10 << 2U);
	// set Alternate function selection AF02 for GPIOB1
	MODIFY_REG(GPIOB->AFR[0], 0b1111 << 4U, 0b0010 << 4U);

	// open TIM3 peripheral clock
	RCC->APB1ENR |= (1 << 1);

	/* Delay after an RCC peripheral clock enabling */
	__IO uint32_t tmpreg = 0x00U;
	tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1RSTR_TIM3RST_Msk);
	// UNUSED(tmpreg);

	// set TIM3 prescaler, TIM3 clock is 1Mhz
	TIM3->PSC = 84e6 / 1e6 - 1;
	TIM3->ARR = 1000;

	// config TIM3 as output  PWM mode1
	TIM3->CCMR1 = (0b0110 << 12) | (1 << 11) | (0 << 8) | (0b0110 << 4) | (1 << 3) | (0 << 0);
	TIM3->CCMR2 = (0b0110 << 12) | (1 << 11) | (0 << 8) | (0b0110 << 4) | (1 << 3) | (0 << 0);

	TIM3->CCR1 = TIM3->CCR2 = TIM3->CCR3 = TIM3->CCR4 = 0;

	TIM3->CCER = (1 << 12) | (1 << 8) | (1 << 4) | (1 << 0);
	TIM3->CR1 = (1 << 7) | (1 << 0);

	taskIDs[STS_TaskID_BUZZER] = STS_Add_Task(STS_Task_Trigger_Mode_RoughTime, bzTasktime, 0, BUZZER_server);
}
