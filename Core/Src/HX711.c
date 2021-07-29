/*
 * HX711.c
 *
 *  Created on: Feb 9, 2021
 *      Author: ahmeduluca
 */
#include "HX711.h"


void HX711_Init(HX711 data)
{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.Pin = data.pinSck;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(data.gpioSck, &GPIO_InitStruct);
//
//	GPIO_InitStruct.Pin = data.pinData;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(data.gpioData, &GPIO_InitStruct);

	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);

}

int HX711_Average_Value(HX711 data, uint8_t times)
{
    int sum = 0,tmp;
    uint32_t time;
    time=HAL_GetTick();
    for (int i = 0; i < times; i++)
    {
    	data=HX711_Value(data,0);
    	tmp = data.scalelive;
    	while(tmp==-1 && time*2000 >= HAL_GetTick())
    	{
    		data=HX711_Value(data,0);
    		tmp = data.scalelive;
    	}
        sum += tmp;
    }

    return sum / times;
}

HX711 HX711_Value(HX711 data, int av)
{
    int buffer;
    buffer=-1;
    if(HAL_GPIO_ReadPin(data.gpioData, data.pinData)!=1)
   {
	   	//__disable_irq();
	   	buffer=0;
		for (uint8_t i = 0; i < 24; i++)
		{
			HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);

			buffer = buffer << 1 ;

			if (HAL_GPIO_ReadPin(data.gpioData, data.pinData))
			{
				buffer ++;
			}

			HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);
		}

		for (int i = 0; i < data.gain; i++)
		{
			HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_SET);
			HAL_GPIO_WritePin(data.gpioSck, data.pinSck, GPIO_PIN_RESET);
		}
		//__enable_irq();
		buffer = buffer ^ 0x800000;
		if(av){
			data.scalelive=buffer-data.offset;
		}
		else{
			data.scalelive=buffer;
		}
   	}
    if(buffer==-1&&!av){
    	data.scalelive=buffer;
    }
    return data;
}

HX711 HX711_Tare(HX711 data, uint8_t times)
{
    int sum = HX711_Average_Value(data, times);
    data.offset = sum;
    return data;
}

int HX711_Available(void)
{

	if (HAL_GPIO_ReadPin(HXDAT_GPIO_Port, HXDAT_Pin)!=1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

