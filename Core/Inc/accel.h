/*
 * accel.h
 *
 *  Created on: Feb 9, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_ACCEL_H_
#define INC_ACCEL_H_

#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdlib.h"

void readMPU(uint16_t id, uint8_t filter, int duration);
void readMMA845(uint16_t id, uint8_t filter, int duration);

#endif /* INC_ACCEL_H_ */
