/*
 * timer.h
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "experiment.h"
#include "uart.h"
#include "DHT.h"
#include "accel.h"
#include "adc.h"
#include "ext_it.h"
#include "HX711.h"
#include "pc_uart.h"

typedef struct{
	TIM_TypeDef                 *Instance;     /*!< Register base address             */
	  TIM_Base_InitTypeDef        Init;          /*!< TIM Time Base required parameters */
	  HAL_TIM_ActiveChannel       Channel;       /*!< Active channel                    */
	  DMA_HandleTypeDef           *hdma[7];      /*!< DMA Handlers array
	                                                  This array is accessed by a @ref DMA_Handle_index */
	  HAL_LockTypeDef             Lock;          /*!< Locking object                    */
	  __IO HAL_TIM_StateTypeDef   State;         /*!< TIM operation state               */
}TIMERS;

void Timer9(void);
void Timer10(void);
void Timer11(void);
void Timer12(void);
void Timer13(void);
void TimeSet(TIM_HandleTypeDef *htim, int duration);

#endif /* INC_TIMER_H_ */
