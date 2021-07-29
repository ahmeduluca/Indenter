/*
 * pc_uart.c
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */
#include "pc_uart.h"
#include "string.h"
#include "uart.h"
#include "heater.h"
#include "experiment.h"
#include "main.h"
#include "adc.h"

//extern UARTCOM * _uartcom3;
extern UARTCOM * _uartcom1;

extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

//extern uint8_t _TxBuf2[TXBUFSIZE2];
//extern uint8_t _RxBuf2[RXBUFSIZE2];
extern uint32_t Tx_Timer3;

extern int isHXcom;
extern char writingpc [100];
extern int pccom;
extern int emergency;
extern int uart2say;
extern int Rx_Timer3;
extern int joyint2;
extern int strain;

extern int eqstep;
extern int expcount;
extern int osc;
extern int osccount;
extern int oscpass;
extern int oscdown;
extern int sendexp;
extern int stopmot;
extern int S;
extern int eqstepcounter;
extern int givecount;
extern int stepnum;
extern int autoretract;
extern char motsender[100];
extern char datasender[30];
extern int loadFeed;
extern int expLoad;
extern int holdPos;

extern int joyen;
extern char reader [100];
extern uint16_t mpu[3];
extern int motpos;
extern int contsend;
extern uint8_t isAutoApproach;
extern int adcVal1;
extern int adcVal2;
extern enum approach app;
extern int automot;
extern int bossayac;

extern INITIALCOM INIT0;
extern EXPERIMENT expin[100];
extern EXPERIMENTrx exprx[100];
extern CALIBRATION calib;
extern CALIBRATIONrx calibrx;

extern int contact;
extern int contproc;
extern int control;
extern int dir;
extern int extcon;
extern int firstdir;
extern int initalize;

extern DHT_DataTypedef instanthum;
extern HX711 instant;

extern int motorcon;
extern int retracting;
extern int returncheck;
extern int sayacmot;
extern int step;
extern char stepper[12];
extern int speedmode;
extern char tmr[10];

extern int tryuc;
extern int loadcon;
extern int motorapp;
extern int stepsay;
extern int temperset[3];
extern int sampleChannel;
extern int loadnow;
extern int movxy;
extern int joyint1;
extern int xmov;
extern int ymov;
extern int increment;
extern int loadIndent;

uint32_t temptemp=0;
int heatFeed[3]={0,0,0};
int heatDuty[3]={0,0,0};
int heaterid;
char BUFGEN[2];
char BUFSM[2];
char BUFSS[2];
char appBuf[7]={0};
long thresholdApp=0;
long speedApp=0;
int pcDecision=0;
int onlyAct=0;

//extern UARTCOM pcInit;
//extern UARTCOM uc45Sender;
int sendingPc=0;
char ucBuffer[15];
char ucPreBuf[15];
extern uint8_t _TxBuf3[TXBUFSIZE3];
extern uint8_t _RxBuf3[RXBUFSIZE3];
UARTCOM * _uartcom3;
UARTCOM pcInit={
		_TxBuf3,			//TxBuffer
		0,					//TxBufferSize
		0,					//TxCounter
		_RxBuf3,			//RxBuffer
		220,				//RxBufferSize
		0,					//RxCounter
		1,					//ComId
		0,					//Result
		0					//ReceiveAcknowledge
};

void PcRx(void){
	_uartcom3->RxCounter++;
	if(contsend==1){
		contsend=2;
	}
	if(_uartcom3->RxSize > _uartcom3->RxCounter)
	{
		HAL_UART_Receive_IT(&huart3, &_uartcom3->RxBuf[_uartcom3->RxCounter], 1);
		if(_uartcom3->RxBuf[_uartcom3->RxCounter]=='Q'&&_uartcom3->RxSize!=5){//Emergency control **sıfırlama
			emergency=1;
			Emergency(0);
			uart2say=0;
		}
		else if(_uartcom3->RxBuf[_uartcom3->RxCounter]=='*'&&_uartcom3->RxSize!=5){
			emergency=2;
			Emergency(1);
			uart2say=0;
		}
	}
	else
	{
		_uartcom3->UResult|=Rx_Cplt;
		Rx_Timer3 = 0;
		_uartcom3->RxCounter = 0;
		if(_uartcom3->RxBuf[0]=='Q'){//Emergency control **sıfırlama
			emergency=1;
			Emergency(0);
			uart2say=0;
		}
		else if(_uartcom3->RxBuf[0]=='*'){
			emergency=2;
			Emergency(1);
			uart2say=0;
		}
		else if(_uartcom3->RxBuf[0]=='Z'){
			movxy=0;
			loadIndent=0;
			HAL_GPIO_WritePin(L293DD_ENABLE1_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(L293DD_ENABLE2_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_RESET);
			isAutoApproach=0;
			xmov=0;
			ymov=0;
			joyint1=0;
			joyint2=0;
			eqstep=0;
			expcount=-1;
			osc=0;
			osccount=0;
			oscpass=0;
			oscdown=-1;
			sendexp=-1;
			stopmot=1;
			S=0;
			eqstepcounter=0;
			givecount=0;
			stepnum=1;
			HAL_TIM_Base_Stop_IT(&htim11);
			HAL_TIM_Base_Stop_IT(&htim10);
			emergency=1;
			if(htim9.Instance->DMAR==0){
				SendPc("Stopped", 5, ID_FIND);
			}
			uart2say=0;
			autoretract=0;
			ProcessRx(3);
		}
		else if(_uartcom3->RxBuf[0]=='R'&&_uartcom3->RxBuf[1]=='e'){
			SendPc(writingpc,_uartcom3->RxSize, 0);
		}
		else if(_uartcom3->RxBuf[2]=='F'&&_uartcom3->RxBuf[3]=='I'&&_uartcom3->RxBuf[4]=='N'){
			if(_uartcom3->RxBuf[0]=='H'&&_uartcom3->RxBuf[1]=='X'){
				contsend=0;
				HAL_TIM_Base_Start_IT(&htim10);
			}
			else if(_uartcom3->RxBuf[0]=='R'&&_uartcom3->RxBuf[1]=='E'){
				sendexp=1;
				HAL_TIM_Base_Start_IT(&htim10);
			}
			else{
				HAL_TIM_Base_Start_IT(&htim10);
			}
		}
		else if(_uartcom3->RxBuf[0]=='M'&&_uartcom3->RxBuf[4]=='D'){
			motsender[0]=0;
			itoa(motpos,motsender,10);
			strcat(motsender,"PM\0");
			SendPc(motsender, 5, ID_FIND);
			uart2say=1;
			TimeSet(&htim12, 100000);
		}
		else if(_uartcom3->RxBuf[0]=='V'){
			BUFGEN[0]=0;
			reader[0]=0;
			BUFSM[0]=0;
			BUFSS[0]=0;
			memcpy(&BUFGEN,&_uartcom3->RxBuf[1],1);
			memcpy(&reader,&_uartcom3->RxBuf[2],1);
			memcpy(&BUFSM,&_uartcom3->RxBuf[3],1);
			memcpy(&BUFSS,&_uartcom3->RxBuf[4],1);
			isHXcom=1;
			moveXY(atoi(BUFGEN), atoi(reader), atoi(BUFSM),atoi(BUFSS));
		}
		else if(_uartcom3->RxBuf[0]=='J'){
			if(_uartcom3->RxBuf[2]=='E'){
				joyen=1;
				SendPc("Joystick Enabled!", 5, ID_FIND);
			}
			else{
				joyen=0;
				SendPc("Joystick Unenabled!", 5, ID_FIND);
			}
		}
		else if(_uartcom3->RxBuf[0]=='C'&&_uartcom3->RxBuf[1]=='D'&&_uartcom3->RxBuf[2]=='A'){
			if(_uartcom3->RxBuf[4]=='1'){
				datasender[0]=0;
				itoa(mpu[1],datasender,10);
				strcat(datasender,"YGY\0");
				SendPc(datasender, 5, ID_FIND);
				uart2say=1;
				TimeSet(&htim12,100000);
			}
			else{
				datasender[0]=0;
				itoa(mpu[2],datasender,10);
				strcat(datasender,"ZGY\0");
				SendPc(datasender, 5, ID_FIND);
				uart2say=1;
				TimeSet(&htim12,100000);
			}
		}
		else if(_uartcom3->RxBuf[0]=='L'&&_uartcom3->RxBuf[1]=='P'){
			if(_uartcom3->RxBuf[2]=='+'){
				pcDecision=1;
			}
			else if(_uartcom3->RxBuf[2]=='-'){
				pcDecision=2;
			}
			else if(_uartcom3->RxBuf[2]=='H'){
				pcDecision=3;
			}
			else{
				pcDecision=0;
			}
			SendPc("Load Control via PC", 5, OLD_ID);
		}
		else if(_uartcom3->RxBuf[0]=='T'&&_uartcom3->RxSize==6){
			memcpy(&BUFGEN,&_uartcom3->RxBuf[1],1);
			contact = 10000 * atoi(BUFGEN);
			memcpy(&BUFGEN,&_uartcom3->RxBuf[2],1);
			contact = contact + 1000 * atoi(BUFGEN);
			memcpy(&BUFGEN,&_uartcom3->RxBuf[3],1);
			contact = contact + 100 * atoi(BUFGEN);
			memcpy(&BUFGEN,&_uartcom3->RxBuf[4],1);
			contact = contact + 10 * atoi(BUFGEN);
			memcpy(&BUFGEN,&_uartcom3->RxBuf[5],1);
			contact = contact + atoi(BUFGEN);
			if(loadFeed==0){
				contact=contact+instant.scalelive; //extra conversion multiplication may be used
			}
			if(loadFeed==1){
				contact=contact+strain; //extra conversion multiplication may be used
			}
			SendPc("Contact Point Saved", 5, OLD_ID);
		}
		else{
			reader[0]=0;
			emergency=0;
			memcpy(&reader,&_uartcom3->RxBuf[0],_uartcom3->RxSize+1);
			reader[_uartcom3->RxSize]=0;
			ProcessData(reader,_uartcom3->ComId);
		}
		if(contsend==2){
			contsend=1;
		}
	}
}

void PcTx(void){
	_uartcom3->TxCounter++;
	if(_uartcom3->TxSize > _uartcom3->TxCounter){
		HAL_UART_Transmit_IT(&huart3, &_uartcom3->TxBuf[_uartcom3->TxCounter], 1);
	}else{
		_uartcom3->UResult|=Tx_Cplt;
		Tx_Timer3 = 0;
		_uartcom3->TxCounter = 0;
	}
}
void DummyRead2(){
	unsigned char dummyRead;
	dummyRead= USART3->DR;
	UNUSED(dummyRead);
}
void SendPc(char* mes, int rxsize, uint8_t comId){
	sendingPc=1;
	char  sender [50];
	sender[0]='$';
	sender[1]=0;
	if(pccom==1){
		uint16_t i=0;
		while(mes[i] != writingpc[i])
			i++;
		if(i!=0){
			writingpc[0]=0;
			memcpy(&writingpc,&mes[0],SizeOfArray(mes));
			writingpc[SizeOfArray(mes)]=0;
		}
		strcat(mes,"\0");
		strcat(sender,mes);
		memcpy(pcInit.TxBuf,sender,SizeOfArray(sender)+1);
		if(comId!=OLD_ID){
			pcInit.ComId=comId;
		}
		pcInit.TxSize=SizeOfArray(sender);
		pcInit.RxSize=rxsize;
		pcInit.ResAck=0;
		pcInit.UResult = 0;
		pcInit.RxCounter=0;
		_uartcom3=&pcInit;
		DummyRead2();
		HAL_UART_Transmit_IT(&huart3, _uartcom3->TxBuf, 1);
		HAL_UART_Receive_IT(&huart3, _uartcom3->RxBuf, 1);
	}
	sendingPc=0;
}
void ProcessRx(int type){
	if(type==1){
		for (int i=0;i<step;i++){
			expin[i].amp=atoi(exprx[i].amprx);
			expin[i].dir=atoi(exprx[i].dirrx);
			if(expLoad){
				expin[i].depth=atoi(exprx[i].depthrx)+contact;//*conversion constant need at +contact term!!!
			}
			else{
				expin[i].depth=atoi(exprx[i].depthrx);
			}
			expin[i].dur=atoi(exprx[i].durrx);
			expin[i].freq=atoi(exprx[i].freqrx);
			expin[i].hold=atoi(exprx[i].holdrx);
			expin[i].osc=atoi(exprx[i].oscrx);
			expin[i].speed=atoi(exprx[i].speedrx);
			expin[i].stepId=atoi(exprx[i].stepIdrx);
			expin[i].xpst=atoi(exprx[i].xpstrx);
			expin[i].ypst=atoi(exprx[i].ypstrx);
			expin[i].stepret=atoi(exprx[i].retrx);
			expin[i].tramp=atoi(exprx[i].dtrx);
			expin[i].tspeed=atoi(exprx[i].dtsrx);
			expin[i].tamp=atoi(exprx[i].dtamprx);
			expin[i].tfreq=atoi(exprx[i].dtfreqrx);
			expin[i].tlag=atoi(exprx[i].dtphaserx);
			expin[i].ttime=atoi(exprx[i].dtwhenrx);
			expin[i].removeHold=atoi(exprx[i].removalHold);
			expin[i].removePer=atoi(exprx[i].removalPer);
		}
	}
	else if(type==2){
		if(expLoad){
			calib.depth=atoi(calibrx.depthrx)+contact;//multiplication factor!!!
		}
		else{
			calib.depth=atoi(calibrx.depthrx);
		}
		calib.interval=atoi(calibrx.intervalrx);
		calib.nofstep=atoi(calibrx.nofsteprx);
		calib.holdDur=atoi(calibrx.hold);
	}
	else{
		for (int i=0;i<step;i++){
			exprx[i].amprx[0]=0;
			exprx[i].dirrx[0]=0;
			exprx[i].depthrx[0]=0;
			exprx[i].durrx[0]=0;
			exprx[i].freqrx[0]=0;
			exprx[i].holdrx[0]=0;
			exprx[i].oscrx[0]=0;
			exprx[i].speedrx[0]=0;
			exprx[i].stepIdrx[0]=0;
			exprx[i].xpstrx[0]=0;
			exprx[i].ypstrx[0]=0;
			exprx[i].retrx[0]=0;
			exprx[i].dtrx[0]=0;
			exprx[i].dtsrx[0]=0;
			exprx[i].dtamprx[0]=0;
			exprx[i].dtfreqrx[0]=0;
			exprx[i].dtphaserx[0]=0;
			exprx[i].dtwhenrx[0]=0;
			exprx[i].removalHold[0]=0;
			exprx[i].removalPer[0]=0;
		}
		calibrx.depthrx[0]=0;
		calibrx.intervalrx[0]=0;
		calibrx.nofsteprx[0]=0;
		calibrx.hold[0]=0;
		step=-1;
		app=none;
		returncheck=0;
		firstdir=0;
		oscdown=0;
		autoretract=0;
		stepsay=0;
	}
}

void ProcessData(char incom[],int iD)
{
	switch(iD)
	{
	case ID_UART_START:
		if(incom[0]=='I'){
			pccom=1;
			extcon=0;
			SendPc("OK\0",2, ID_UART_START);
		}
		else if(incom[0]=='O'&&incom[1]=='K'){
			if(initalize<6){
				//MX_USART1_UART_Init();//!!
				initalize=0;
				bossayac=1;
			}
			SendPc("CONNECTED\0", 5, ID_FIND);
		}
		else{
			SendPc("TRY_AGAIN\0",1, ID_UART_START);
			pccom=0;
		}
		break;
	case ID_FIND:
		BUFGEN[0]=0;
		if(incom[0]=='4'){
			if(incom[1]=='0'){//4_0_A_SS_
				memcpy(&BUFGEN,&incom[2],1);
				app=atoi(BUFGEN);//approach type al.
				memcpy(&BUFGEN,&incom[3],1);
				step=10*atoi(BUFGEN);
				memcpy(&BUFGEN,&incom[4],1);
				step=step+atoi(BUFGEN);//step sayısını al.
				SendPc("R0\0",149, EXP_START);
				uart2say=1;
				TimeSet(&htim12, 100000);
			}
			else if(incom[1]=='1'){//4_1_App_return_dir_
				memcpy(&BUFGEN,&incom[2],1);//APPROACH TYPE
				app=atoi(BUFGEN);
				memcpy(&BUFGEN,&incom[3],1);//Returncheck
				returncheck=atoi(BUFGEN);
				memcpy(&BUFGEN,&incom[4],1);//Direction
				firstdir=atoi(BUFGEN);
				SendPc("R1\0",41, EXP_START);
				uart2say=1;
				TimeSet(&htim12, 100000);
			}
			else if(incom[1]=='3'){
				memcpy(&BUFGEN,&incom[2],1);//experiment displacement|2|/load|1|/voltage|0| control
				expLoad=atoi(BUFGEN);
				memcpy(&BUFGEN,&incom[3],1);//load feed type //HX|0|ADC|1|NI|2|
				loadFeed=atoi(BUFGEN);
				memcpy(&BUFGEN,&incom[4],1);//
				SendPc("ExpType", 5, ID_FIND);
			}
		}
		else if(incom[0]=='3'){
			memcpy(&BUFGEN,&incom[1],1);
			dir=atoi(BUFGEN);
			memcpy(&BUFGEN,&incom[2],1);
			automot=atoi(BUFGEN);
			memcpy(&BUFGEN,&incom[3],1);
			speedmode=atoi(BUFGEN);
			SendPc("R3\0", 20, MOT_COM);
		}
		else if(incom[0]=='5'){//5_SS_00
			memcpy(&BUFGEN,&incom[1],1);
			int bufsiz=10*atoi(BUFGEN);
			memcpy(&BUFGEN,&incom[2],1);
			bufsiz=bufsiz+atoi(BUFGEN);//DIR_COM BUFFER SIZE
			SendPc("R5\0",bufsiz, DIRECT_COM);
			uart2say=1;
			TimeSet(&htim12, 100000);
		}
		else if(incom[0]=='6'&&_uartcom3->RxSize==5){
			if(initalize>5){
				tryuc=101;
				char ucrest[15];
				ucrest[0]='P';
				SendAct(ucrest, INIT0.kp);
			}
			else{
				tryuc=0;
				bossayac=1;
				initalize=0;
				contproc=0;
				HAL_TIM_Base_Start_IT(&htim11);
				SendPc("Trying to Communicate UC45...\0", 5, ID_FIND);
			}
		}
		else if(incom[0]=='7'){
			memcpy(&BUFGEN,&incom[1],1);
			control = 1000 * atoi(BUFGEN);
			memcpy(&BUFGEN,&incom[2],1);
			control = control + 100 * atoi(BUFGEN);
			memcpy(&BUFGEN,&incom[3],1);
			control = control + 10 * atoi(BUFGEN);
			memcpy(&BUFGEN,&incom[4],1);
			control = control + atoi(BUFGEN);
			motsender[0]=0;
			itoa(adcVal1,motsender,10);
			strcat(motsender,"C\0");
			SendPc(motsender, 5, OLD_ID);
			uart2say=1;
			TimeSet(&htim12,100000);
		}
		else if(incom[0]=='8'){
			if(incom[1]=='0'){//9pts deviaton find
				if(incom[2]=='3'){//from NI-PCIe sensor data

				}
				else if(incom[2]=='0'){//from internal ADC data--using actuator !limit should be given
					slopExt(0, 5, 5, 5, 100, 10);
				}
				else if(incom[2]=='1'){//using step motor
					slopExt(1, 5, 5, 5, 100, 10);
				}
				else if(incom[2]=='2'){//using step motor && NI data
					slopExt(2, 5, 5, 5, 100, 10);
				}
			}
			else if(incom[1]=='1'){//mpu6050from upper part
				HAL_GPIO_WritePin(MPU6050_AD0_GPIO_Port, MPU6050_AD0_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(MPU6050_AD1_GPIO_Port, MPU6050_AD1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(MPU6050_AD2_GPIO_Port, MPU6050_AD2_Pin, GPIO_PIN_SET);
				readMPU(0xD0, 0x00, 10);
				datasender[0]=0;
				itoa(mpu[0],datasender,10);
				strcat(datasender,"XGY\0");
				SendPc(datasender, 5, OLD_ID);
				uart2say=1;
				TimeSet(&htim12,100000);			}
			else if(incom[1]=='2'){//mpu6050from moving part
				HAL_GPIO_WritePin(MPU6050_AD0_GPIO_Port, MPU6050_AD0_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(MPU6050_AD1_GPIO_Port, MPU6050_AD1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(MPU6050_AD2_GPIO_Port, MPU6050_AD2_Pin, GPIO_PIN_SET);
				readMPU(0xD0, 0x00, 10);
				datasender[0]=0;
				itoa(mpu[0],datasender,10);
				strcat(datasender,"XGY\0");
				SendPc(datasender, 5, OLD_ID);
				uart2say=1;
				TimeSet(&htim12,100000);
			}
			else if(incom[1]=='3'){//mpu6050from base part
				HAL_GPIO_WritePin(MPU6050_AD0_GPIO_Port, MPU6050_AD0_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(MPU6050_AD1_GPIO_Port, MPU6050_AD1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(MPU6050_AD2_GPIO_Port, MPU6050_AD2_Pin, GPIO_PIN_RESET);
				readMPU(0xD0, 0x00, 10);
				datasender[0]=0;
				itoa(mpu[0],datasender,10);
				strcat(datasender,"XGY\0");
				SendPc(datasender, 5, OLD_ID);
				uart2say=1;
				TimeSet(&htim12,100000);
			}
			else if(incom[1]=='4'){//humidity sensor
				//give instant data
				motsender[0]=0;
				datasender[0]=0;
				DHT_GetData(&instanthum);
				itoa(instanthum.Humidity,motsender,10);
				strcat(datasender,motsender);
				strcat(datasender,"H\0");
				itoa(instanthum.Temperature,motsender,10);
				strcat(datasender,motsender);
				strcat(datasender,"T\0");
				SendPc(datasender, 5, OLD_ID);
				uart2say=1;
				TimeSet(&htim12,100000);
			}
			else if(incom[1]=='5'){//hx711 read
				//Continuous or instant
				isHXcom=1;
				if(incom[2]=='0'){
					instant=HX711_Value(instant,1);
					motsender[0]=0;
					datasender[0]=0;
					itoa(instant.scalelive,datasender,10);
					strcat(datasender,"LM\0");
					strcat(motsender,datasender);
					SendPc(motsender, 5, OLD_ID);
					uart2say=1;
					TimeSet(&htim12,100000);
					isHXcom=0;
				}
				else if(incom[2]=='1'){
					SendPc("TAMAMDIR", 5, OLD_ID);
					isHXcom=0;
					contsend=1;
				}
				else if(incom[2]=='2'){
					instant=HX711_Tare(instant, 10);
					motsender[0]=0;
					datasender[0]=0;
					HAL_Delay(150);
					instant=HX711_Value(instant,1);
					itoa(instant.scalelive,datasender,10);
					strcat(datasender,"LM\0");
					strcat(motsender,datasender);
					SendPc(motsender, 5, OLD_ID);
					uart2say=1;
					TimeSet(&htim12,100000);
					contact=control+instant.scalelive;
					isHXcom=0;
				}
				else if(incom[2]=='3'){
					if(incom[3]=='1'){
						loadcon=1; //HX711
						SendPc("THRESHOLD", 6, OLD_ID);
					}
					else if(incom[3]=='2'){
						loadcon=2; //MCU ADC strain measurement
						SendPc("THRESHOLD", 6, OLD_ID);
					}
					else{
						loadcon=0;
						SendPc("TAMAMDIR", 5, OLD_ID);
					}
					isHXcom=0;
				}
			}

		}
		else if(incom[0]=='9'){//temperature set id
			memcpy(&BUFGEN,&incom[1],1);
			heaterid=atoi(BUFGEN);//set heater id..
			if(incom[2]=='1'){
				//take params and run heater

				if(incom[4]=='1'){
					//internal ADC control set
					heatFeed[heaterid]=1;
					SendPc("TSET", 20, TEMP_SET);
				}
				else{
					heatFeed[heaterid]=0;
					SendPc("TSET", 20, TEMP_SET);
					//external +/- or D --% take PWM duty
				}
			}
			else if(incom[2]=='D'){
				//take % incom[3]&incom[4]
				memcpy(&BUFGEN,&incom[3],1);
				heatDuty[heaterid] = 10 * atoi(BUFGEN);
				memcpy(&BUFGEN,&incom[4],1);
				heatDuty[heaterid] = (heatDuty[heaterid] + atoi(BUFGEN))*100;
				setHeater(heaterid, heatDuty[heaterid], 0);
				SendPc("HeaterDutySet", 5, ID_FIND);
			}
			else if(incom[2]=='+'){
				//increase duty cycle
				heatDuty[heaterid]=heatDuty[heaterid]+1;
				DummyRead2();
				_uartcom3->RxSize=5;
				HAL_UART_Receive_IT(&huart3, _RxBuf3, 1);
				if(heatDuty[heaterid] > 5000){
					heatDuty[heaterid]=5000;
				}
				else{
					setHeater(heaterid, heatDuty[heaterid], 0);
				}
			}
			else if(incom[2]=='-'){
				//decrease duty cycle
				heatDuty[heaterid]=heatDuty[heaterid]-1;
				DummyRead2();
				_uartcom3->RxSize=5;
				HAL_UART_Receive_IT(&huart3, _RxBuf3, 1);
				if(heatDuty[heaterid] < 300){
					heatDuty[heaterid]=300;
				}
				else{
					setHeater(heaterid, heatDuty[heaterid], 0);
				}
			}
			else{
				SendPc("Heater Deactivated",5,ID_FIND);
				heatDuty[heaterid]=0;
				setHeater(heaterid, heatDuty[heaterid], 0);
			}
			incom[0]=0;
		}
		else if(incom[0]=='O'||incom[0]=='I'){
			pccom=0;
			if(initalize<5){
				initalize=0;
				contproc=0;
				bossayac=1;
			}
			uart2say=1;
			HAL_TIM_Base_Start_IT(&htim11);
		}
		else{
			SendPc("NO_PROCESS\0",5, ID_FIND);
		}
		BUFGEN[0]=0;
		break;
	case EXP_START://set "i" for ID_FIND's step count and "app" approach type; if equal step=> set for cal func..
		if(incom[0]=='D'){//komut gelince deneye basla..
			if(incom[4]=='1'){
				motorapp=1;
			}
			else{
				motorapp=0;
			}
			if(incom[5]=='1'){
				motorcon=1;
			}
			else{
				motorcon=0;
			}
			if(incom[6]=='1'){
				onlyAct=1;
			}
			else{
				onlyAct=0;
			}
			if(eqstep==1){
				Cal(calib.depth, calib.nofstep, calib.interval);
				SendPc("Executing_Calibration\0", 5, ID_FIND);
				uart2say=1;
				TimeSet(&htim12, 100000);
			}
			else{
				if(incom[7]=='1'){
					oscdown=1;
				}
				memcpy(&appBuf,&incom[8],6);
				thresholdApp=atoi(appBuf);
				if(thresholdApp==0){
					thresholdApp=contact;
				}
				appBuf[0]=0;
				memcpy(&appBuf,&incom[16],6);
				speedApp=atoi(appBuf);
				appBuf[0]=0;
				if(motorapp==0){
					increment=speedApp;
					speedApp=50000;
				}
				if(app==none){
					SendPc("Executing_Indentation\0", 5, EXP_START);
					if(expin[0].xpst!=0||expin[0].ypst!=0){
						autoApproach(-1);//set x&y positions for first step;
					}
					uart2say=1;
					TimeSet(&htim12, 100000);
				}
				else{
					SendPc("APPROACH\0", 5, EXP_START);
					if(motorapp==1){
						automot=1;
					}
					if(app==internal){
						autoApproach(0);
					}
					else if(app==external){
						autoApproach(1);
					}
					uart2say=1;
					TimeSet(&htim12, 100000);
				}
			}

		}
		else if(_uartcom3->RxSize==149){
			memcpy(&exprx[S].stepIdrx,&incom[0],2);
			memcpy(&exprx[S].dirrx,&incom[3],1);
			memcpy(&exprx[S].depthrx,&incom[4],9);
			memcpy(&exprx[S].speedrx,&incom[15],9);
			memcpy(&exprx[S].holdrx,&incom[26],9);
			memcpy(&exprx[S].oscrx,&incom[37],1);
			memcpy(&exprx[S].amprx,&incom[40],9);
			memcpy(&exprx[S].freqrx,&incom[51],9);
			memcpy(&exprx[S].durrx,&incom[62],9);
			memcpy(&exprx[S].xpstrx,&incom[73],9);
			memcpy(&exprx[S].ypstrx,&incom[84],9);
			memcpy(&exprx[S].dtrx,&incom[95],5);
			memcpy(&exprx[S].dtsrx,&incom[102],5);
			memcpy(&exprx[S].dtamprx,&incom[109],5);
			memcpy(&exprx[S].dtfreqrx,&incom[116],5);
			memcpy(&exprx[S].dtphaserx,&incom[123],5);
			memcpy(&exprx[S].dtwhenrx,&incom[130],1);
			memcpy(&exprx[S].retrx,&incom[133],1);
			memcpy(&exprx[S].removalHold,&incom[136],6);
			memcpy(&exprx[S].removalPer,&incom[144],3);
			S++;
			if(S<step){
				char stwrt[25];
				itoa(S,stwrt,10);
				strcat(stwrt,"._Step_Received\0");
				SendPc(stwrt,149, EXP_START);//comid set important
				uart2say=1;
				TimeSet(&htim12, 10000);				}
			if(S==step){
				char stwrt[30];
				itoa(S,stwrt,10);
				strcat(stwrt,"_StepReady\0");
				SendPc(stwrt,22, EXP_START);
				S=0;
				expcount=0;
				ProcessRx(1);
				uart2say=1;
				sendexp=1;
				TimeSet(&htim12, 100000);
			}
		}
		else if(_uartcom3->RxSize==41){
			memcpy(&calibrx.depthrx,&incom[0],9);
			memcpy(&calibrx.nofsteprx,&incom[11],9);
			memcpy(&calibrx.intervalrx,&incom[22],9);
			memcpy(&calibrx.hold,&incom[33],6);
			eqstep=1;
			ProcessRx(2);
			SendPc("Start_to_Calibration?\0",8, EXP_START);
			uart2say=0;
		}
		else if(incom[2]=='D'){
			//take % incom[3]&incom[4]
			memcpy(&BUFGEN,&incom[3],1);
			heatDuty[heaterid] = 10 * atoi(BUFGEN);
			memcpy(&BUFGEN,&incom[4],1);
			heatDuty[heaterid] = (heatDuty[heaterid] + atoi(BUFGEN))*100;
			setHeater(heaterid, heatDuty[heaterid], 0);
			SendPc("HeaterDutySet", 5, ID_FIND);
		}
		else if(incom[0]=='S'&&incom[1]=='T'){
			HAL_TIM_Base_Stop_IT(&htim12);
			uart2say=0;
			automot=0;
			stopmot=0;
			TimeSet(&htim10, 100000);
		}
		else{
			SendPc("NO_PROCESS",5, ID_FIND);
		}
		break;
	case DIRECT_COM://direct com
		if(incom[_uartcom3->RxSize-1]=='E'){
			ucPreBuf[0]=0;
			ucBuffer[0]=0;
			memcpy(&ucPreBuf,&incom[0],1);
			memcpy(&ucBuffer,&incom[1],_uartcom3->RxSize-2);
			ucPreBuf[1]=0;
			ucBuffer[_uartcom3->RxSize-2]=0;
			SendAct(ucPreBuf, ucBuffer);
			SendPc("Direct_Communication", 5, ID_FIND);
			/*}
			else{
				if(retracting==0){
					SendPc("Approaching", 5, ID_FIND);
				}
				else{
					SendPc("Retracting", 5, ID_FIND);
				}
			}*/
			ParaInt(_uartcom1->TxBuf);
		}
		else{
			SendPc("NO_PROCESS", 5, ID_FIND);
		}
		break;
	case MOT_COM:
		uart2say=0;
		HAL_TIM_Base_Stop_IT(&htim12);
		memcpy(&tmr,&incom[0],10);
		TimeSet(&htim9, atoi(tmr)/2);
		stepsay=1;
		if(automot==0){
			memcpy(&stepper,&incom[10],10);
			stepsay=atoi(stepper);
		}
		SendPc("Motor Moving", 5, ID_FIND);
		StepD(dir);
		break;
	case TEMP_SET://first 6 is Tset -123.456 C- after 2 digit empty- then 12 digit parameter (empty for now)
		SendPc("Heater Activated!", 5, ID_FIND);
		if(incom[0]=='-')
		{

		}
		else{

		}
		memcpy(&BUFGEN,&incom[1],1);
		temptemp = 10000 * atoi(BUFGEN);
		memcpy(&BUFGEN,&incom[2],1);
		temptemp = temptemp + 1000*atoi(BUFGEN);
		memcpy(&BUFGEN,&incom[3],1);
		temptemp = temptemp + 100*atoi(BUFGEN);
		memcpy(&BUFGEN,&incom[4],1);
		temptemp = temptemp + 10*atoi(BUFGEN);
		memcpy(&BUFGEN,&incom[5],1);
		temptemp = temptemp + atoi(BUFGEN);
		temperset[heaterid]=temptemp/20000;
		if(heatDuty[heaterid]==0){
			heatDuty[heaterid]=200;
		}
		if(incom[8]=='1'){
			sampleChannel=heaterid;
		}
		setHeater(heaterid, heatDuty[heaterid], 0);
		break;

	default:
		SendPc("NO_PROCESS", 5 ,ID_FIND);
		break;
	}
}

