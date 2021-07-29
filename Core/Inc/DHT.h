/*
 * DHT.h
 *
 *  Created on: Feb 9, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include "stm32f4xx_hal.h"
#include "main.h"

typedef struct
{
	uint32_t Temperature;
	uint32_t Humidity;
	uint32_t Temperaturef;
	uint32_t Humidityf;
}DHT_DataTypedef;

#define RESET_CYCLE_COUNTER()  do { \
        CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk; \
        ITM->LAR = 0xC5ACCE55; \
        DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; \
        DWT->CYCCNT = 0; \
        DWT->CTRL = DWT_CTRL_CYCCNTENA_Msk; \
    }while(0)

void DHT_Start (void);
uint8_t Check_Response (void);
uint8_t DHT_Read (void);
void set_gpio_mode(uint8_t pMode);
void DWT_Init(void);
void DWT_Delay(uint32_t us); // microseconds
void DHT_GetData (DHT_DataTypedef *DHT_Data);

#endif /* INC_DHT_H_ */
