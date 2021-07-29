/*
 * DHT.c
 *
 *  Created on: Feb 9, 2021
 *      Author: ahmeduluca
 */

#include "DHT.h"

#define INPUT 0
#define OUTPUT 1
#define TYPE_DHT11    // define according to DHT11/or22

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM; uint8_t Presence = 0;

void DHT_Start (void)
{
	set_gpio_mode(OUTPUT);  // set the pin as output
	HAL_GPIO_WritePin (DHT_GPIO_Port, DHT_Pin, 0);   // pull the pin low
	HAL_Delay(18);   // wait for 18ms
	HAL_GPIO_WritePin (DHT_GPIO_Port, DHT_Pin, 1);   // pull the pin low
	DWT_Delay(20);
	set_gpio_mode(INPUT);
}
void DHT_GetData (DHT_DataTypedef *DHT_Data)
{
    DHT_Start ();
	Presence = Check_Response();
	if(Presence)
	{
		Rh_byte1 = DHT_Read();
		Rh_byte2 = DHT_Read();
		Temp_byte1 = DHT_Read();
		Temp_byte2 = DHT_Read();
		SUM = DHT_Read();

		if (SUM == (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2))
		{
			#if defined(TYPE_DHT11)
				DHT_Data->Temperature = Temp_byte1;
				DHT_Data->Humidity = Rh_byte1;
			#endif

			#if defined(TYPE_DHT22)
				DHT_Data->Temperature =Temp_byte1;
				DHT_Data->Temperaturef=Temp_byte2;
				DHT_Data->Humidity =Rh_byte1;
				DHT_Data->Humidityf=Rh_byte2;
			#endif
		}
	}
}
uint8_t DHT_Read (void)
{
	uint8_t i=0,j=0;
	uint32_t time;
	for (j=0;j<8;j++)
	{
		time = HAL_GetTick();
		while ((!HAL_GPIO_ReadPin (DHT_GPIO_Port, DHT_Pin)) && (time >= HAL_GetTick()) ){
			// wait for the pin to go high
		}
		DWT_Delay(40);   // wait for 40 us!!!!
		if (!(HAL_GPIO_ReadPin (DHT_GPIO_Port, DHT_Pin)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		time = HAL_GetTick();
		while ((HAL_GPIO_ReadPin (DHT_GPIO_Port, DHT_Pin)) && (time >= HAL_GetTick())){
// wait for the pin to go low
		}
	}
	return i;
}
uint8_t Check_Response (void)
{
	uint8_t Response = 0;
	uint32_t time;
	if (!(HAL_GPIO_ReadPin (DHT_GPIO_Port, DHT_Pin)))
	{
		DWT_Delay(80);//!!!
		if ((HAL_GPIO_ReadPin (DHT_GPIO_Port, DHT_Pin))){
			Response = 1;
		}
		else {
			Response = 0;
		}
	}
	time = HAL_GetTick();
	while ((HAL_GPIO_ReadPin (DHT_GPIO_Port, DHT_Pin)) && (time >= HAL_GetTick())){
// wait for the pin to go low
	}
	return Response;
}
void set_gpio_mode(uint8_t pMode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	//if direction parameter OUTPUT
	if(pMode == OUTPUT)
	{
	/*Configure GPIO pins : LD3_Pin LD2_Pin */
	  GPIO_InitStruct.Pin = DHT_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);
	}else if(pMode == INPUT)   //else if direction parameter INPUT
	{
	  GPIO_InitStruct.Pin = DHT_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);
	}
}
void DWT_Delay(uint32_t us) // microseconds
{
	RESET_CYCLE_COUNTER();
    uint32_t startTick = DWT->CYCCNT,
             delayTicks = us * (SystemCoreClock/1000000);

    while (DWT->CYCCNT - startTick < delayTicks);
}

void DWT_Init(void)
{
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
}
