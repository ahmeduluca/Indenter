/*
 * ext_it.h
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_EXT_IT_H_
#define INC_EXT_IT_H_

#include "stm32f4xx_hal.h"

void TmcIndex(void);
uint32_t TmcGetIndex(void);
void TmcSetIndex(uint32_t index);
void JoyBut1(void);
void JoyBut2(void);
void EndXplus(void);
void EndYplus(void);
void EndXmin(void);
void EndYmin(void);



#endif /* INC_EXT_IT_H_ */
