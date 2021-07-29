/*
 * HX711.h
 *
 *  Created on: Feb 9, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_HX711_H_
#define INC_HX711_H_

#include "stm32f4xx_hal.h"
#include "main.h"

typedef struct _hx711
{
	GPIO_TypeDef* gpioSck;
	GPIO_TypeDef* gpioData;
	uint16_t pinSck;
	uint16_t pinData;
	int offset;
	int gain;
	int scalelive;
	// 1: channel A, gain factor 128
	// 2: channel B, gain factor 32
    // 3: channel A, gain factor 64
} HX711;


void HX711_Init(HX711 data);
HX711 HX711_Tare(HX711 data, uint8_t times);
HX711 HX711_Value(HX711 data, int av);
int HX711_Average_Value(HX711 data, uint8_t times);

int HX711_Available(void);


#endif /* INC_HX711_H_ */
