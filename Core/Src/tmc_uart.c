/*
 * tmc_uart.c
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */
#include "tmc_uart.h"

//extern UARTCOM * _uartcom3;
UARTCOM * _uartcom2;
extern uint8_t _TxBuf2[TXBUFSIZE2];
extern uint8_t _RxBuf2[RXBUFSIZE2];
UARTCOM tmcInit={
		_TxBuf2,			//TxBuffer
		0,					//TxBufferSize
		0,					//TxCounter
		_RxBuf2,			//RxBuffer
		220,				//RxBufferSize
		0,					//RxCounter
		1,					//ComId
		0,					//Result
		0					//ReceiveAcknowledge
};


void TmcRx(void){

}

void TmcTx(void){

}

