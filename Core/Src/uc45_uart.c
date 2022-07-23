/*
 * uc45_uart.c
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */
#include "uc45_uart.h"
#include "pc_uart.h"
#include "uart.h"

//extern UARTCOM * _uartcom1;
extern UART_HandleTypeDef huart1;
//extern UARTCOM uc45Sender;
//extern UARTCOM uc45Init;
extern uint8_t _TxBuf1[TXBUFSIZE1];
extern uint8_t _RxBuf1[RXBUFSIZE1];
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;
extern int bossayac;
extern int contproc;
extern int pccom;
extern int initalize;
extern int tryuc;
extern uint32_t Tx_Timer1;
extern int uart2say;
extern int sendingPc;

UARTCOM * _uartcom1;
UARTCOM uc45Init={
		_TxBuf1,			//TxBuffer
		1,					//TxBufferSize
		0,					//TxCounter
		_RxBuf1,			//RxBuffer
		1,					//RxBufferSize
		0,					//RxCounter
		0,					//ComId
		0,					//Result
		0					//ReceiveAcknowledge
};
UARTCOM uc45Sender={
		_TxBuf1,			//TxBuffer
		1,					//TxBufferSize
		0,					//TxCounter
		_RxBuf1,			//RxBuffer
		1,					//RxBufferSize
		0,					//RxCounter
		1,					//ComId
		0,					//Result
		0					//ReceiveAcknowledge
};
void Uc45Rx(void){
	_uartcom1->RxCounter++;
			if(_uartcom1->RxSize > _uartcom1->RxCounter)
			{
				HAL_UART_Receive_IT(&huart1, &_uartcom1->RxBuf[_uartcom1->RxCounter], 1);
			}else{
				uart2say=0;
				_uartcom1->UResult|=Rx_Cplt;
				//Rx_Timer1 = 0;
				_uartcom1->RxCounter = 0;
				if(_uartcom1->ComId==1&&initalize>4){
					if(_uartcom1->RxBuf[0]=='X'){//UC45 normal haberleşme cevabı X olmak zorunda.
						_uartcom1->ResAck=1;
						if(tryuc==101){
							SendPc("Connected to UC45\0", 5, ID_FIND);
							tryuc=0;
						}
					}
					else{
						_uartcom1->ResAck=0;
						initalize=0;
						if(pccom==1&&tryuc%2==0){
							if(sendingPc==0){
								SendPc("Restart UC45",5,2);
								HAL_TIM_Base_Stop_IT(&htim11);
								tryuc++;
							}
						}
					}
				}
				else if(_uartcom1->ComId==1&&initalize<5){
					if(_uartcom1->RxBuf[0]=='X'){//UC45 normal haberleşme cevabı X olmak zorunda.
						_uartcom1->ResAck=1;
						initalize++;
						HAL_TIM_Base_Start_IT(&htim11);
					}
					else{
						_uartcom1->ResAck=0;
						initalize=0;
						if(pccom==1&&tryuc%2==0)
						{
							SendPc("Restart UC45",5,2);
							HAL_TIM_Base_Stop_IT(&htim11);
							tryuc++;
						}
					}		}
				else if(initalize==0){
					if(_uartcom1->RxBuf[0]=='Y'){
						_uartcom1->ResAck=1;
						initalize=1;
						uc45Sender.ComId=1;
						contproc=1;//gerekmeyebilir..
					}
					else{
						_uartcom1->ResAck=0;
						bossayac++;
						if(pccom==1&&bossayac%5==0&&tryuc%2==0)
						{
							SendPc("Restart UC45", 5, ID_FIND);
							tryuc++;
							HAL_TIM_Base_Stop_IT(&htim11);
						}
					}
				}
			}


}

void Uc45Tx(void){
	_uartcom1->TxCounter++;
	if(_uartcom1->TxSize > _uartcom1->TxCounter){
		HAL_UART_Transmit_IT(&huart1, &_uartcom1->TxBuf[_uartcom1->TxCounter], 1);
	}else{
		_uartcom1->UResult|=Tx_Cplt;
		Tx_Timer1 = 0;
		_uartcom1->TxCounter = 0;
	}
}
void DummyRead1(){
	unsigned char dummyRead;
	dummyRead= USART1->DR;
	memset(_RxBuf1,0,sizeof(_RxBuf1)/sizeof(_RxBuf1[0]));
	UNUSED(dummyRead);
}
void SendAct(char *pre,char *mess){
	if(pre[0]==0){
		uc45Sender.ComId=0;
		memcpy(uc45Sender.TxBuf,"E\0",1);
		uc45Sender.TxBuf[1]=0;
		uc45Sender.TxSize=1;
	}
	else{
		uc45Sender.ComId=1;
		strcat(pre,mess);
		strcat(pre,"E\0");
		memcpy(uc45Sender.TxBuf,pre,SizeOfArray(pre)+1);
		uc45Sender.TxBuf[SizeOfArray(pre)+1]=0;
		uc45Sender.TxSize=SizeOfArray(pre);
	}
	uc45Sender.RxSize=1;
	uc45Sender.ResAck=0;
	uc45Sender.UResult=0;
	DummyRead1();
	_uartcom1=&uc45Sender;
	HAL_UART_Transmit_IT(&huart1, _uartcom1->TxBuf,1);
	HAL_UART_Receive_IT(&huart1, _uartcom1->RxBuf, 1);
}

