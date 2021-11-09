/*
 * time.h
 *
 *  Created on: Nov 5, 2021
 *      Author: 0
 */

#ifndef INC_TIME_H_
#define INC_TIME_H_

#include <stdint.h>
#include "stdbool.h"

bool Check_Time_Out(uint32_t Millstone_Time, uint32_t Time_Period_ms);
uint32_t Cal_Time(uint32_t Millstone_Time, uint32_t Systick_now);

#endif /* INC_TIME_H_ */
