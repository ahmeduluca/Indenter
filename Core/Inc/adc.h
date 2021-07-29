/*
 * adc.h
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "timer.h"


#endif /* INC_ADC_H_ */
void actadc(uint16_t* adcarray);
long map(long x, long in_min, long in_max, long out_min, long out_max);
