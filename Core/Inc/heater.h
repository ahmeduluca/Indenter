/*
 * heater.h
 *
 *  Created on: Feb 10, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_HEATER_H_
#define INC_HEATER_H_

#include "stm32f4xx_hal.h"

#define TIM_CHANNEL_1                      0x00000000U
#define TIM_CHANNEL_2                      0x00000004U
#define TIM_CHANNEL_3                      0x00000008U
#define TIM_CHANNEL_4                      0x0000000CU

 void setHeater(int channel, int duty, int opMode);

#endif /* INC_HEATER_H_ */
