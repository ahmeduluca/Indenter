/*
 * ext_it.c
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */
#include "ext_it.h"
#include "pc_uart.h"
#include "main.h"

static uint32_t tmcIndexCnt=0;
extern int extcon;
extern int joyen;
extern int pccom;
extern int speedmode;
extern int xsteps;
extern int ysteps;
extern int ymov;
extern int xmov;
extern char motsender [100];
extern char BUFSS [2];
extern int uart2say;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == TMC_IND_Pin){
		TmcIndex();
	}
	else if(GPIO_Pin == JS1_Pin){
		JoyBut1();
	}
	else if(GPIO_Pin == JS2_Pin){
		JoyBut2();
	}
	else if(GPIO_Pin == ESX_P_Pin ){
		EndXplus();
	}
	else if(GPIO_Pin == ESX_N_Pin){
		EndXmin();
	}
	else if(GPIO_Pin == ESY_P_Pin ){
		EndYplus();
	}
	else if(GPIO_Pin == ESY_N_Pin){
		EndYmin();
	}
}

uint32_t TmcGetIndex(void){
	return tmcIndexCnt;
}

void TmcSetIndex(uint32_t index){
	tmcIndexCnt=index;
}

void TmcIndex(void){
	tmcIndexCnt++;
}
void JoyBut1(void){
	if(pccom!=1){
		if(joyen==1){
			speedmode=speedmode+1;
			if(speedmode>5){
				speedmode=2;
			}
		}
		else{
			joyen=1;
			extcon=1;
		}
	}
	else{
		if(joyen==1){
			speedmode=speedmode+1;
			if(speedmode>5){
				speedmode=2;
			}
			motsender[0]=0;
			BUFSS[0]=0;
			uart2say=0;
			itoa(speedmode,BUFSS,10);
			strcat(motsender,"MSP\0");
			strcat(motsender,BUFSS);
			SendPc(motsender, 5, 0);
		}
	}
}
void JoyBut2(void){
	if(pccom!=1){
		if(joyen==1){
			joyen=0;
			extcon=0;
		}
		else{
			joyen=1;
			extcon=1;
		}
	}
}
void EndXplus(void){
	uart2say=0;
	if(HAL_GPIO_ReadPin(ESX_P_GPIO_Port, ESX_P_Pin)){
		HAL_GPIO_WritePin(L293DD_ENABLE2_GPIO_Port, L293DD_ENABLE2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(YN_GPIO_Port, YN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(YP_GPIO_Port, YP_Pin, GPIO_PIN_RESET);
		xsteps=0;
		xmov=0;
		SendPc("OutYP", 5, 0);
	}
	else{
		SendPc("InYP", 5, 0);
	}

}
void EndYplus(void){
	uart2say=0;
	if(HAL_GPIO_ReadPin(ESY_P_GPIO_Port, ESY_P_Pin)){
		HAL_GPIO_WritePin(L293DD_ENABLE1_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(XN_GPIO_Port, XN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, GPIO_PIN_RESET);
		ysteps=0;
		ymov=0;
		SendPc("OutXP", 5, 0);
	}
	else{
		SendPc("InXP", 5, 0);
	}

}
void EndXmin(void){
	uart2say=0;
	if(HAL_GPIO_ReadPin(ESY_P_GPIO_Port, ESY_P_Pin)){
		HAL_GPIO_WritePin(L293DD_ENABLE2_GPIO_Port, L293DD_ENABLE2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(YN_GPIO_Port, YN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(YP_GPIO_Port, YP_Pin, GPIO_PIN_RESET);
		xsteps=0;
		ysteps=0;
		xmov=0;
		SendPc("OutYM", 5, 0);
	}
	else{
		SendPc("InYM", 5, 0);
	}

}
void EndYmin(void){
	uart2say=0;
	if(HAL_GPIO_ReadPin(ESY_P_GPIO_Port, ESY_P_Pin)){
		HAL_GPIO_WritePin(L293DD_ENABLE1_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(XN_GPIO_Port, XN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, GPIO_PIN_RESET);
		ysteps=0;
		ymov=0;
		SendPc("OutXM", 5, 0);
	}
	else{
		SendPc("InXM", 5, 0);
	}

}
