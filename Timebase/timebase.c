/*
*****config sysclock and define Time function 
*/
#include "timebase.h"
#include "stm32f4xx_hal.h"

static volatile uint32_t current_time_1 = 0;

const float TIMsec1 = 1.0f / (float)SYS_Frequence;
static const float TIMsec2 = (float)(4294967296) / (float)SYS_Frequence;


#pragma region TIME

	TIME get_TIME_now()
	{
		TIME t_now;

		uint32_t part1;
		uint32_t part2;

		do
		{
			part1 = current_time_1;
			part2 = SysTick->LOAD - SysTick->VAL;
		} while (part1 != current_time_1);

		t_now.t = (uint64_t)part1 * (uint64_t)(SysTick->LOAD + 1) + (uint64_t)part2;
		return t_now;
	}

	float get_pass_time(TIME last_time)
	{
		TIME current_time = get_TIME_now();
		typedef union
		{
			uint64_t long_data;
			unsigned int int_data[2];
		} long_int_union;
		if (current_time.t > last_time.t)
		{
			uint64_t time_diff = current_time.t - last_time.t;
			long_int_union *convert = (long_int_union *)&time_diff;
			return TIMsec2 * convert->int_data[1] + TIMsec1 * convert->int_data[0];
		}
		else
		{
			uint64_t time_diff = last_time.t - current_time.t;
			long_int_union *convert = (long_int_union *)&time_diff;
			return -TIMsec2 * convert->int_data[1] - TIMsec1 * convert->int_data[0];
		}
	}

	float get_pass_time_precise(TIME *last_time)
	{
		TIME current_time = get_TIME_now();
		typedef union
		{
			uint64_t long_data;
			unsigned int int_data[2];
		} long_int_union;
		if (current_time.t > last_time->t)
		{
			uint64_t time_diff = current_time.t - last_time->t;
			last_time->t = current_time.t;
			long_int_union *convert = (long_int_union *)&time_diff;
			return TIMsec2 * convert->int_data[1] + TIMsec1 * convert->int_data[0];
		}
		else
		{
			uint64_t time_diff = last_time->t - current_time.t;
			last_time->t = current_time.t;
			long_int_union *convert = (long_int_union *)&time_diff;
			return -TIMsec2 * convert->int_data[1] - TIMsec1 * convert->int_data[0];
		}
	}

	float get_time_difference(TIME time_a, TIME time_b)
	{
		typedef union
		{
			uint64_t long_data;
			unsigned int int_data[2];
		}long_int_union;
		if( time_a.t > time_b.t )
		{
			uint64_t time_diff = time_a.t - time_b.t;
			long_int_union* convert = (long_int_union*)&time_diff;
			return -TIMsec2 * convert->int_data[1] - TIMsec1 * convert->int_data[0];
		}
		else
		{
			uint64_t time_diff = time_b.t - time_a.t;
			long_int_union* convert = (long_int_union*)&time_diff;
			return -TIMsec2 * convert->int_data[1] - TIMsec1 * convert->int_data[0];
		}
	}

	float get_system_runtime()
	{
		TIME current_time = get_TIME_now();
		typedef union
		{
			uint64_t long_data;
			unsigned int int_data[2];
		}long_int_union;
		
		uint64_t time_diff = current_time.t;
		long_int_union* convert = (long_int_union*)&time_diff;
		return -TIMsec2 * convert->int_data[1] - TIMsec1 * convert->int_data[0];
	}

	void Time_set_inValid( TIME* t )
	{
		t->t = 0;
	}
	bool Time_isValid( TIME t )
	{
		return (t.t != 0);
	}

	void delay( float t )
	{
		TIME start_time = get_TIME_now();
		while( get_pass_time(start_time) <= t );
	}
#pragma endregion TIME


/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 84M (CPU Clock)
 *            HCLK(Hz)                       = 84M (AXI and AHBs Clock)
 *            AHB Prescaler                  = 1
 *            HSE Frequency(Hz)              = 25M
 *            PLL_M                          = 25
 *            PLL_N                          = 168
 *            PLL_P                          = 2
 *            PLL_Q                          = 4
 *            VDD(V)                         = 3.3
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	//__HAL_RCC_PWR_CLK_ENABLE();
	// __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		while (1)
			;
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		while (1)
			;
	}
}
void SysInit()
{
	HAL_Init();
	SystemClock_Config();
}
void SysTick_Handler(void)
{
	++current_time_1;
}