/*
 * uart.h
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdlib.h"
#include "uc45_uart.h"
#include "pc_uart.h"
#include "tmc_uart.h"

#define TXBUFSIZE3 255
#define RXBUFSIZE3 255
#define TXBUFSIZE1 20
#define RXBUFSIZE1 1
#define TXBUFSIZE2 255
#define RXBUFSIZE2 255


typedef enum{
	Rx_Cplt=1,
	Tx_Cplt=2,
	RxTx_Cplt=3,
	Rx_Tout=4,
	Tx_Tout=8,
	Tx_Cplt_Rx_Tout=6,
	Rx_Cplt_Tx_Tout=9,
	RxTx_Tout=12

}URes;


typedef struct
{
	uint8_t * TxBuf;
	uint32_t TxSize;
	uint32_t TxCounter;
	uint8_t * RxBuf;
	uint32_t RxSize;
	uint32_t RxCounter;
	uint8_t ComId;
	URes UResult;
	uint8_t ResAck;
}UARTCOM;

void PcRx(void);
void PcTx(void);
void Uc45Rx(void);
void Uc45Tx(void);
void TmcRx(void);
void TmcTx(void);
void UartInit(void);
uint16_t SizeOfArray(char * array);

#endif /* INC_UART_H_ */
