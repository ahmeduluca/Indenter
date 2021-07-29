/*
 * uart.c
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */
#include "uart.h"

//#include "struct.h"


//haberleşme kontrol ve parametre
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;
uint32_t Tx_Timer1=0;
uint32_t Tx_Timer3=0;
uint32_t Rx_Timer1=0;
uint32_t Rx_Timer3=0;
uint8_t sending;
int uart2say=0;
int repsay=0;
char readingpc [100];
char writingpc [100];
char adding [5];
char * mesaj;
char voltageval[11]={'W','\0'};
int reps=0;
int pccom=0;
extern UARTCOM _uartcom1;
extern UARTCOM _uartcom2;
extern UARTCOM _uartcom3;
uint8_t _TxBuf1[TXBUFSIZE1];
uint8_t _RxBuf1[RXBUFSIZE1];
uint8_t _TxBuf3[TXBUFSIZE3];
uint8_t _RxBuf3[RXBUFSIZE3];
uint8_t _TxBuf2[TXBUFSIZE2];
uint8_t _RxBuf2[RXBUFSIZE2];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART1){
		Uc45Rx();
	}
	else if(huart->Instance==USART3){
		PcRx();
	}
	else if(huart->Instance==USART2){
		TmcRx();
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART1){
		Uc45Tx();
	}
	else if(huart->Instance==USART3){
		PcTx();
	}
	else if(huart->Instance==USART2){
		TmcTx();
	}
}

void UartInit(void){
	  _uartcom2.TxBuf=_TxBuf2;		//TxBuffer
	  _uartcom2.TxSize=TXBUFSIZE2;			//TxBufferSize
	  _uartcom2.TxCounter=0;					//TxCounter
	  _uartcom2.RxBuf=_RxBuf2;			//RxBuffer
	  _uartcom2.RxSize=RXBUFSIZE2;			//RxBufferSize
	  _uartcom2.RxCounter=0;					//RxCounter
	  _uartcom2.ComId=0;					//ComId
	  _uartcom2.UResult=0;					//Result
	  _uartcom2.ResAck=0;					//ReceiveAcknowledge

	  _uartcom1.TxBuf=_TxBuf1;		//TxBuffer
	  _uartcom1.TxSize=TXBUFSIZE1;			//TxBufferSize
	  _uartcom1.TxCounter=0;					//TxCounter
	  _uartcom1.RxBuf=_RxBuf1;			//RxBuffer
	  _uartcom1.RxSize=RXBUFSIZE1;			//RxBufferSize
	  _uartcom1.RxCounter=0;					//RxCounter
	  _uartcom1.ComId=0;					//ComId
	  _uartcom1.UResult=0;					//Result
	  _uartcom1.ResAck=0;					//ReceiveAcknowledge

	  _uartcom3.TxBuf=_TxBuf3;		//TxBuffer
	  _uartcom3.TxSize=TXBUFSIZE3;			//TxBufferSize
	  _uartcom3.TxCounter=0;					//TxCounter
	  _uartcom3.RxBuf=_RxBuf3;			//RxBuffer
	  _uartcom3.RxSize=RXBUFSIZE3;			//RxBufferSize
	  _uartcom3.RxCounter=0;					//RxCounter
	  _uartcom3.ComId=0;					//ComId
	  _uartcom3.UResult=0;					//Result
	  _uartcom3.ResAck=0;					//ReceiveAcknowledge
	  HAL_UART_Receive_IT(&huart3,_uartcom3.RxBuf,1);
}

uint16_t SizeOfArray(char * array)
{
	uint16_t i=0;
	while(array[i] != 0)
		i++;
	return i;
}





