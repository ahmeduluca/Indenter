/*
 * pc_uart.h
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_PC_UART_H_
#define INC_PC_UART_H_

#include "uart.h"
//#include "uc45_uart.h"
#include "DHT.h"
#include "HX711.h"
#include "timer.h"
#include "experiment.h"

#define OLD_ID					0
#define ID_UART_START			1
#define ID_FIND					2
#define MOT_COM					3
#define EXP_START				4
#define DIRECT_COM				5
#define TEMP_SET				9






void SendPc(char* mes, int rxsize, uint8_t comId);
void DummyRead1(void);
void ProcessData(char incom[],int iD);
void ProcessRx(int type);

#endif /* INC_PC_UART_H_ */
