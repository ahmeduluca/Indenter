/*
 * heater.c
 *
 *  Created on: Feb 10, 2021
 *      Author: ahmeduluca
 *
 *      PWM Heater control
 *      PID / on-off modes
 *      Peltier Current direction - cool/heat modes
 */


#include "heater.h"
extern TIM_HandleTypeDef htim1;
extern int heatDuty[3];
void setHeater(int channel, int duty, int opMode){
	uint32_t count;
	if(duty==0){
		count=0;
	}
	else{
		count=((htim1.Init.Period+1)*duty)/10000;//duty 0.01*duty!!
	}
	/*if(opMode){//ileride ısıtma soğutma için akım yönünü ayarlamak için kullanılacak

	}
	else{

	}*/
	switch(channel){
	case	0:
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,count);
		break;
	case	1:
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,count);
		break;
	case	2:
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,count);
		break;
	}
	htim1.Instance->CNT=0;
}
