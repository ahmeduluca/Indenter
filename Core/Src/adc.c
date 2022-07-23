/*
 * adc.c
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */
#include "adc.h"
#include "heater.h"

extern uint16_t adcBuff[1300];
extern uint16_t adcData[13];
extern int loadcon;
extern int sensdist;
extern int automot;
extern int joyen;
extern int joyint1;
extern int joyint2;
extern int control;
extern int endstop;
extern int stopmot;
extern int aprox;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim1;
extern int heatDuty[3];
extern int isHXcom;
extern int uart2say;
uint8_t convEnd = 0;
int adctim;
int adcVal2=0;
int adcVal1=0;
int adcsay=0;
int tempers[3]={0,0,0};
int temperset[3]={0,0,0};
int strain=0;
extern int heatFeed[3];
extern int xsteps;
extern int ysteps;
extern int loadnow;
extern int loadFeed;
extern int extcon;
/**
 * @brief  Conversion complete callback in non blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		for(int i = 1299; i > 1286; i--)
		{
			adcData[i-1287] = adcBuff[i];				//copy data to second buffer
		}
		convEnd = 1;
		adctim++;
		if(adctim==2){
			actadc(adcData);
			adctim=0;
		}
	}
}
void actadc(uint16_t* adcarray){
	tempers[0]=adcData[1];
	tempers[1]=adcData[2];
	tempers[2]=adcData[3];
	strain=adcData[9];
	adcVal1=adcData[4];
	int vref=adcData[12];
	int zcon=adcData[10];
	int ycon=adcData[6];
	int xcon=adcData[5];
	adcsay=1;
	if(adcVal1<aprox){
		adcVal2=1;
		if(loadcon==0){
			sensdist=0;
		}
	}
	else{
		adcVal2=0;
		if(loadcon==0){
			sensdist=1;
		}
	}
	if(joyen==1){
		if(zcon<1850){
			if(joyint1==0){
				automot=1;
				extcon=1;
				TimeSet(&htim9, 10000);
				StepD(0);
				joyint1=1;
			}
		}
		else if(zcon>2300){
			if(joyint1==0){
				automot=1;
				extcon=1;
				TimeSet(&htim9, 10000);
				StepD(1);
				joyint1=2;
			}
		}
		else{
			joyint1=0;
			stopmot=1;
		}
		if(ycon<1850){
			if(joyint2 %2== 0){
				if(joyint2<2){
					moveXY(0, 0, 1000,1);
					isHXcom=1;
				}
			}
		}
		else if(ycon>2300){
			if(joyint2 %3== 0){
				if(joyint2<3){
					moveXY(0, 1, 1000,1);
					isHXcom=1;
				}
			}
		}
		else {
			ysteps=0;
		}
		if(xcon<1850){
			if(joyint2 < 12){
				if(joyint2<6){
					moveXY(1, 0, 3000,1);
					isHXcom=1;
				}
			}
		}
		else if(xcon>2300){
			if(joyint2 > 9 ||joyint2<6){
				if(joyint2<12){
					moveXY(1, 1, 3000,1);
					isHXcom=1;
				}
			}
		}
		else{
			xsteps=0;
		}
	}
	for (int i=0;i<3;i++){
		if(heatDuty[i]!=0 &&heatFeed[i]==1){
			if(tempers[i]<temperset[i]-10){
				heatDuty[i]=heatDuty[i]+1;
				if(heatDuty[i]>5000){
					heatDuty[i]=5000;
					SendPc("ExceedHeat", 5, 2);
				}
				setHeater(i, heatDuty[i], 0);
			}
			else if(tempers[i]>temperset[i]+10){
				heatDuty[i]=heatDuty[i]-1;
				if(heatDuty[i]<100)
				{
					heatDuty[i]=100;
					SendPc("NotCool", 5, 2);
				}
				setHeater(i, heatDuty[i], 0);
			}
		}
	}
	if(loadFeed==1){
		loadnow=strain*1;
	}
}
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


