/*
 * time.c
 *
 *  Created on: Nov 5, 2021
 *      Author: 0
 */

#include <stdint.h>
#include "stdbool.h"

extern uint32_t	RT_Count_Systick_u32;

// Return : ms - time period (count Systick Timer)
uint32_t Cal_Time(uint32_t Millstone_Time, uint32_t Systick_now)
{
	if (Systick_now < Millstone_Time)
		return (0xFFFFFFFF - Millstone_Time + Systick_now);
	return (Systick_now - Millstone_Time);
}

// return = 1 : time_run >= time period
//				= 0 : don't need check time || not time out
bool Check_Time_Out(uint32_t Millstone_Time, uint32_t Time_Period_ms)
{
	if (Millstone_Time == 0) return 0;

	if (Cal_Time(Millstone_Time,RT_Count_Systick_u32) >= Time_Period_ms) return 1;

	return 0;
}


