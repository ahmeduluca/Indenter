/*
 * timer.c
 *
 *  Created on: Jan 30, 2021
 *      Author: ahmeduluca
 */
#include "timer.h"
#include "pc_uart.h"
#include "heater.h"
#include "main.h"

extern int posinf;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;

extern UART_HandleTypeDef huart3;
extern UARTCOM * _uartcom3;
extern UARTCOM * _uartcom1;
extern enum approach app;
extern UARTCOM pcInit;
extern uint32_t Rx_Timer1;
extern uint32_t Rx_Timer3;
extern int pccom;


extern HX711 instant;
extern int isHXcom;
extern int contsend;
extern int loadFeed;
extern int loadnow;

//indent ve cal parametreleri
extern char voltage [11];
extern uint8_t isAutoApproach;
extern int oscdown;
extern int autoretract;
extern int eqstep;//timer2 control eqstep activate
extern int sendexp;//timer2 control execute exp
extern int step;//exp step sayısı
extern int returncheck;//calibration forward backward
extern int caltime;
extern long stepdepth;
extern long stepvolt;
extern int stepnum;
extern long firstpt;
extern int eqstepcounter;
extern int stepno;
extern int expcount;
extern int firstdir;

extern int contact;
extern char datasender[30];
extern int extcon;
extern int loadcon;

extern int uart2say;
extern int repsay;
extern char readingpc [100];
extern char writingpc [100];
extern char adding [5];
extern char * mesaj;
extern char voltageval[11];

//oscillation params..
extern char hivolt [10];
extern char lovolt[10];
extern int osc;//timer2 control oscillation activate
extern int numosc;
extern int osccount;
extern int oscpass;
extern int holdcount;

//genel sayac ve parametreler
extern int initalize;//uc45 initialize array cont
extern int contproc;//uc45 com start cont
extern int sendinit;//timer2 control initparams activate
extern int S;
extern int emergency;
extern long voltnow;
extern int givecount;
extern int holder;

extern int xnow;
extern int ynow;
extern int xco;
extern int yco;
extern int movxy;
extern int xmov;
extern int ymov;

//motor params
extern char tmr[10];
extern char stepper[12];
extern char reader [100];
extern char motsender[100];
extern int sayacmot;
extern int durummot;
extern int stepsay;
extern int dir;
extern int stopmot;
extern int tryuc;
extern int tim4say0;
extern int bossayac;
extern int motorcon;
extern int motorapp;
extern int sqrdur;
extern int sensdist;
extern int automot;
extern int endstop;
extern int posinf;
extern int speedmode;
extern int retracting;
extern int upp;
extern int motpos;
extern int motgain;
extern int control;

extern INITIALCOM INIT0;
extern EXPERIMENT expin[100];
extern EXPERIMENTrx exprx[100];
extern CALIBRATION calib;
extern CALIBRATIONrx calibrx;

extern int joyen;
extern int joyint1;
extern int joyint2;
extern int delaymic;

uint32_t x_enc[2];
uint32_t y_enc[2];
uint32_t i_enc[2];
extern int xsteps;
extern int ysteps;
int xcount=0;
int ycount=0;
char xysend[10];
extern int encodMove;

extern int first;
extern int delta;//1 full actuator scan range 5um
extern int increment;//100ms incs for approximately 20nm/s

extern int heatDuty[3];
extern int sampleChannel;
extern int heatFeed[3];
extern int appOrret;
extern int loadIndent;
extern int sendingPc;
extern int initialPos;
extern int holdPos;
extern int pcDecision;
extern long thresholdApp;
extern int onlyAct;
extern int speedApp;
extern int readPc;

int motorTouchpt=0;
int actTouchpt=0;
int holdRet=0;
int holdRetCnt=0;
int holdRetDur=0;
int calHoldCount=0;
int initialMotor=0;
int approaching=0;
int oscsqrCount=0;
extern int sqrmod;
extern int dxy;
extern int stepfin;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM9) {
		Timer9();
	}
	else if(htim->Instance == TIM10)	{
		Timer10();
	}
	else if(htim->Instance == TIM11){
		Timer11();
	}
	else if(htim->Instance == TIM12)
	{
		Timer12();
	}
	else if(htim->Instance == TIM13){
		Timer13();
	}
	else if(htim->Instance == TIM2){
		y_enc[1]+=1;
	}
	else if(htim->Instance == TIM8){
		x_enc[1]+=1;
	}
	else if(htim->Instance == TIM3){
		i_enc[1]+=1;
	}
}

void Timer9(void){
	//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin) ;
	sayacmot++;
	endstop=HAL_GPIO_ReadPin(ENDSTOP_INPUT_GPIO_Port, ENDSTOP_INPUT_Pin);
	if(endstop==0&&dir==1){
		stopmot=1;
	}
	if(extcon==1){
		if(stopmot==1){//sensdist vb. de olmalı Zmaxda dursun diye
			HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_SET);
			HAL_TIM_Base_Stop_IT(&htim9);
			extcon=0;
			automot=0;
		}
		else{
			HAL_GPIO_TogglePin(STEP_GPIO_Port, STEP_Pin);
			if(HAL_GPIO_ReadPin(STEP_GPIO_Port, STEP_Pin)==1){
				if(dir==0){
					motpos+=motgain;
				}
				else{
					motpos-=motgain;
				}
			}
		}
	}
	else{
		if(sqrdur==1)
		{
			datasender[0]=0;
			itoa(loadnow,datasender,10);
			strcat(datasender,"LM\0");
			SendPc(datasender, 5, 0);
			TimeSet(&htim9, expin[expcount].freq/20);
			delaymic++;
			if(delaymic==10){
				sqrdur=2;
				delaymic=0;
			}
			HAL_TIM_Base_Start_IT(&htim9);
		}
		else if(sqrdur==2){
			datasender[0]=0;
			itoa(loadnow,datasender,10);
			strcat(datasender,"LM\0");
			SendPc(datasender, 5, 0);
			sayacmot=0;
			TimeSet(&htim9, 50);
			sqrdur=0;
			HAL_TIM_Base_Start_IT(&htim9);
		}
		else if(sqrdur==3){
			datasender[0]=0;
			itoa(loadnow,datasender,10);
			strcat(datasender,"LM\0");
			SendPc(datasender, 5, 0);
			calHoldCount++;
			if(calHoldCount>=calib.holdDur){
				calHoldCount=1;
				sqrdur=0;
				TimeSet(&htim9, caltime*1000);
				StepD(dir);
			}
			sayacmot--;
		}
		else{
			motsender[0]=0;
			if(stopmot==1||(sensdist==0&&app==none&&dir==0&&automot==1))
			{
				stopmot=1;
				upp=1;
				automot=0;
				joyint1=0;
				durummot=0;
				if(approaching||retracting==1){
					HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_SET);
				}
				HAL_TIM_Base_Stop_IT(&htim9);
				if(endstop==0){
					motsender[0]=0;
					SendPc("UPMOT\0", 5, 2);
					motpos=0;
					uart2say=1;
					TimeSet(&htim12, 100000);
				}
				else if(sensdist==0&&app==none){
					motsender[0]=0;
					itoa(sayacmot,motsender,10);
					strcat(motsender,"CLOSe\0");
					SendPc(motsender, 5, 2);
					uart2say=1;
					TimeSet(&htim12, 100000);
				}
				else if(app==none){
					_uartcom3->ComId=2;
					if(dir==0){
						motsender[0]=0;
						itoa(sayacmot,motsender,10);
						strcat(motsender,"PM\0");
						SendPc(motsender, 5, 0);
					}
					else{
						sayacmot=-sayacmot;
						motsender[0]=0;
						itoa(sayacmot,motsender,10);
						strcat(motsender,"PM\0");
						SendPc(motsender, 5, 0);
					}
					uart2say=1;
					TimeSet(&htim12, 100000);
				}
				sayacmot=0;
			}
			else if(upp==0)
			{
				HAL_GPIO_TogglePin(STEP_GPIO_Port, STEP_Pin);
				if(HAL_GPIO_ReadPin(STEP_GPIO_Port, STEP_Pin)==1){
					if(dir==0){
						motpos+=motgain;
					}
					else{
						motpos-=motgain;
					}
				}
				if(sayacmot %posinf==0&&stopmot==0){
					if(automot==1||(sendexp==1&&isAutoApproach==0)||eqstep==1||(expin[expcount].osc==osc_tri&&osc==1)){//automot in trial to change with aprroaching&retracting
						//_uartcom3->ComId=2; //!maybe later
						if(dir==0){
							datasender[0]=0;
							motsender[0]=0;
							itoa(posinf,motsender,10);
							strcat(motsender,"PM\0");
							itoa(loadnow,datasender,10);
							strcat(datasender,"LM\0");
							strcat(motsender,datasender);
							SendPc(motsender, 5, 0);
						}
						else{
							motsender[0]=0;
							datasender[0]=0;
							itoa(-posinf,motsender,10);
							strcat(motsender,"PM\0");
							itoa(loadnow,datasender,10);
							strcat(datasender,"LM\0");
							strcat(motsender,datasender);
							SendPc(motsender, 5, 0);
						}
					}
				}
			}
		}
		if(sayacmot >= stepsay*2&&automot==0&&stopmot==0)
		{
			durummot=0;
			if(retracting==1||approaching){
				HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_SET);
				HAL_TIM_Base_Stop_IT(&htim9);
				if(approaching&&motorapp){
					isAutoApproach=5;
					SendPc("Approach_Complete\0", 5, 4);
					uart2say=1;
					TimeSet(&htim12, 100000);
					approaching=0;
				}
				else if(!motorapp&&approaching){
					motsender[0]=0;
					itoa(sayacmot,motsender,10);
					strcat(motsender,"PM\0");
					SendPc(motsender, 5, 4);
					isAutoApproach=3;
				}
				else{
					retracting=0;
					thresholdApp = contact + loadnow;
					dxy=0;
					isAutoApproach=1;
				}
				sayacmot=0;
			}
			else if(sendexp==-1&&eqstep!=1)
			{
				HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_SET);
				HAL_TIM_Base_Stop_IT(&htim9);
				if(dir==0){
					motsender[0]=0;
					itoa(sayacmot,motsender,10);
					strcat(motsender,"PM\0");
					SendPc(motsender, 5, ID_FIND);
				}
				else{
					sayacmot=-sayacmot;
					motsender[0]=0;
					itoa(sayacmot,motsender,10);
					strcat(motsender,"PM\0");
					SendPc(motsender, 5, ID_FIND);
				}
				uart2say=1;
				TimeSet(&htim12, 100000);
				sayacmot=0;
			}
			else if(sendexp==0&&osc==1)
			{
				HAL_GPIO_TogglePin(DIR_GPIO_Port, DIR_Pin);
				if(expin[expcount].osc==osc_sqr)
				{
					sqrdur=1;
					delaymic=0;
					if(osccount==0){
						stepsay=2*stepsay;
					}
					else if(osccount==numosc*2-1){
						stepsay=stepsay/2;
					}
					TimeSet(&htim9,expin[expcount].freq/2);
					HAL_TIM_Base_Start_IT(&htim9);
					osccount++;
					if(dir==0){
						datasender[0]=0;
						motsender[0]=0;
						itoa(sayacmot,motsender,10);
						strcat(motsender,"PM\0");
						itoa(loadnow,datasender,10);
						strcat(datasender,"LM\0");
						strcat(motsender,datasender);
						SendPc(motsender, 5, 0);
					}
					else{
						sayacmot=-sayacmot;
						motsender[0]=0;
						datasender[0]=0;
						itoa(sayacmot,motsender,10);
						strcat(motsender,"PM\0");
						itoa(loadnow,datasender,10);
						strcat(datasender,"LM\0");
						strcat(motsender,datasender);
						SendPc(motsender, 5, 0);
					}
					sayacmot=0;
				}
				else
				{
					if(osccount==0){
						stepsay=2*stepsay;
					}
					else if(osccount==numosc*2-1){
						stepsay=stepsay/2;
					}
					osccount++;
					sayacmot=0;
				}
				if(osccount==numosc*2||numosc==0)
				{
					sayacmot=0;
					HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_SET);
					HAL_TIM_Base_Stop_IT(&htim9);
					osc=0;
					oscpass=2;
					stepfin=0;
					osccount=0;
					sqrdur=0;
					sendexp=1;
					HAL_TIM_Base_Start_IT(&htim10);
				}
				dir=HAL_GPIO_ReadPin(DIR_GPIO_Port, DIR_Pin);
			}
			else if(sendexp==1||eqstep==1)
			{
				sayacmot=0;
				if(returncheck!=0)
				{
					if(calHoldCount==0){
						stepsay=calib.depth;
						if(calib.holdDur!=0){
							TimeSet(&htim9, 1000);
							sqrdur=3;
						}
						else{
							returncheck=0;
						}
						if(firstdir==1){
							HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);
							dir=0;
							StepD(0);
						}
						else{
							HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_RESET);
							dir=1;
							StepD(1);
						}
					}
					else{
						calHoldCount=0;
						HAL_TIM_Base_Stop_IT(&htim9);
						eqstep=1;
						eqstepcounter=2*stepnum;
						HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_SET);
						HAL_TIM_Base_Start_IT(&htim10);
					}
				}
				else
				{
					HAL_TIM_Base_Stop_IT(&htim9);
					if(sendexp!=1){
						eqstep=1;
						eqstepcounter=2*stepnum;
					}
					HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_SET);
					HAL_TIM_Base_Start_IT(&htim10);
				}
				firstdir=0;
			}
		}

	}

}

void Timer10(void){
	//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin) ;
	if(sendexp==1){//experiment start ..steps //calculated step voltages and if exist speed increments..
		if(isAutoApproach){
			//wait for xy positioning! & autoapproach
		}
		else if(expcount<step&&oscpass==0&&holdcount==0){
			if(expin[expcount].tramp!=0 && expin[expcount].ttime==0){//heat before indent
				//feed internal =>set adc and temp sits..
				//feed external take Temperature and set duty
				setHeater(sampleChannel, heatDuty[sampleChannel], 0);
				expin[expcount].tramp=0;
			}
			if(expcount==0){
				initialPos=voltnow;
				initialMotor=motpos;
			}
			Indent(expin[expcount].depth, expin[expcount].speed,expin[expcount].dir);
			holdcount=1;
		}
		else if(holdcount==1){
			if(expin[expcount].ttime==2){//heat after indent
				//feed internal =>set adc and temp sits..
				//feed external take Temperature and set duty
				setHeater(sampleChannel, heatDuty[sampleChannel], 0);
			}
			HAL_TIM_Base_Stop_IT(&htim10);
			Hold(expin[expcount].hold);
		}
		else if(holdcount>1){
			if(holdcount<=holder){
				holdcount++;
				motsender[0]=0;
				datasender[0]=0;
				itoa(loadnow,datasender,10);
				strcat(datasender,"LM\0");
				strcat(motsender,datasender);
				SendPc(motsender, 5, 0);
				stepfin=0;
			}
			else{
				if(stepfin==0){
					readingpc[0]=0;
					itoa(expcount+1,readingpc,10);
					strcat(readingpc,"._Indent_Finished\0");
					SendPc(readingpc, 5, 0);
					uart2say=1;
					TimeSet(&htim12, 100000);
				}
				else{
					sendexp=1;
					oscpass=1;
					holdcount=0;
					loadIndent=0;
					stepfin=0;
				}
			}
		}
		else if(oscpass==1){
			HAL_TIM_Base_Stop_IT(&htim12);
			uart2say=0;
			if(motorcon==1 && expin[expcount].osc!=osc_non){
				stepsay=expin[expcount].amp;
				numosc=expin[expcount].dur;
				if(expin[expcount].osc==osc_sqr){
					TimeSet(&htim9, 50);
				}
				else if(expin[expcount].osc==osc_tri){
					TimeSet(&htim9, expin[expcount].freq/(expin[expcount].amp*8));//1/2 from motor drive; 1/2 from osc
					//&& 1/2 from amplitude to peaktopeak conversions=1/8
				}
				StepD(0);
				osc=1;
				oscpass=0;
				sendexp=0;
				HAL_TIM_Base_Stop_IT(&htim10);
			}
			else if(motorcon==0 && expin[expcount].osc!=osc_non) {
				osc=1;//osc set
				Osc(expin[expcount].amp, expin[expcount].freq, expin[expcount].dur, expin[expcount].osc);
				sendexp=0;
				oscpass=0;
			}
			else if(expin[expcount].osc==0){
				oscpass=2;
				sendexp=1;
				stepfin=0;
			}

		}
		else if(oscpass==2){
			if(stepfin==0){
				readingpc[0]=0;
				itoa(expcount+1,readingpc,10);
				strcat(readingpc,"._Oscillation_Finished\0");
				SendPc(readingpc, 5, 0);
			}
			//autoretract for step ?if need-->x-y pst differ/or desired
			else if(expcount+1<step && stepfin==1){
				oscpass=0;
				stepfin=0;
				if(expin[expcount].stepret || (expin[expcount+1].xpst!=0||expin[expcount+1].ypst!=0)){
					HAL_TIM_Base_Stop_IT(&htim10);
					retracting=1;
					if(app==internal||app==0){
						autoApproach(2);
					}
					else{
						readingpc[0]=0;
						autoApproach(2);
						strcat(readingpc,"Retracting\0");
						SendPc(readingpc, 5, 4);
					}
				}
				expcount++;
			}
			else if(expin[expcount].stepret && expcount+1==step&&stepfin==1){
				HAL_TIM_Base_Stop_IT(&htim10);
				stepfin=0;
				oscpass=0;
				retracting=1;
				autoApproach(2);
				expcount++;
			}
			else{
				stepfin=0;
				oscpass=0;
				expcount++;
			}
			uart2say=1;
			TimeSet(&htim12, 100000);
		}
		else{
			readingpc[0]=0;
			sendexp=-1;
			strcat(readingpc,"Experiment_Finished\0");
			SendPc(readingpc, 5, 2);
			ProcessRx(3);
			uart2say=1;
			TimeSet(&htim12, 100000);
			HAL_TIM_Base_Stop_IT(&htim10);
			expcount=-1;
		}


	}
	else if(osc==1){//oscillation set
		if(expin[expcount].osc==osc_sqr){
			motsender[0]=0;
			datasender[0]=0;
			itoa(loadnow,motsender,10);
			strcat(motsender,"LM\0");
			if(oscsqrCount%sqrmod==0){
				if(osccount<numosc){
					if(osccount%2==0){
						strcat(datasender,hivolt);
						strcat(datasender,"E\0");
						strcat(motsender,datasender);
						SendPc(motsender, 5, 0);
						GiveVolt(hivolt);
					}
					else if(osccount%2!=0){
						strcat(datasender,lovolt);
						strcat(datasender,"E\0");
						strcat(motsender,datasender);
						SendPc(motsender, 5, 0);
						GiveVolt(lovolt);
					}
					osccount++;
				}
				else{
					ftos(firstpt,voltage);
					strcat(datasender,voltage);
					strcat(datasender,"E\0");
					strcat(motsender,datasender);
					SendPc(motsender, 5, 0);
					GiveVolt(voltage);
					osc=0;
					oscpass=2;
					stepfin=0;
					osccount=0;
					sendexp=1;
				}
			}
			else if(oscsqrCount%5==0){
				SendPc(motsender, 5, 0);
			}
			oscsqrCount++;
		}
		else{//burada cal fonksiyonundan gelen veriler kullanılarak üçgen dalga veriliyor_100ms çözünürlükte maks step sayısı ile.
			if(osccount<numosc){
				if(eqstepcounter<2*stepnum){
					if(oscdown==1){
						if(eqstepcounter<stepnum){
							if(givecount%2==0){
								stepvolt=firstpt-stepdepth*(eqstepcounter+1);
								voltnow=stepvolt;
								ftos(stepvolt,voltage);
								givecount++;
								datasender[0]=0;
								itoa(loadnow,datasender,10);
								strcat(datasender,"LM\0");
								SendPc(datasender, 5, 0);
							}
							else{
								motsender[0]=0;
								strcat(motsender,voltage);
								strcat(motsender,"E\0");
								SendPc(motsender, 5, 0);
								GiveVolt(voltage);
								givecount++;
								eqstepcounter++;
							}
						}
						else if(eqstepcounter>=stepnum){
							if(givecount%2==0){
								stepvolt=stepvolt+stepdepth;
								voltnow=stepvolt;
								ftos(stepvolt,voltage);
								givecount++;
								datasender[0]=0;
								itoa(loadnow,datasender,10);
								strcat(datasender,"LM\0");
								SendPc(datasender, 5, 0);
							}
							else{
								motsender[0]=0;
								strcat(motsender,voltage);
								strcat(motsender,"E\0");
								SendPc(motsender, 5, 0);
								GiveVolt(voltage);
								givecount++;
								eqstepcounter++;
							}
						}
					}
					else{
						if(eqstepcounter<stepnum){
							if(givecount%2==0){
								stepvolt=firstpt+expin[expcount].amp-stepdepth*(eqstepcounter+1);
								voltnow=stepvolt;
								ftos(stepvolt,voltage);
								givecount++;
								datasender[0]=0;
								itoa(loadnow,datasender,10);
								strcat(datasender,"LM\0");
								SendPc(datasender, 5, 0);
							}
							else{
								motsender[0]=0;
								strcat(motsender,voltage);
								strcat(motsender,"E\0");
								SendPc(motsender, 5, 0);
								GiveVolt(voltage);
								givecount++;
								eqstepcounter++;
							}
						}
						else if(eqstepcounter>=stepnum){
							if(givecount%2==0){
								stepvolt=stepvolt+stepdepth;
								voltnow=stepvolt;
								ftos(stepvolt,voltage);
								givecount++;
								datasender[0]=0;
								itoa(loadnow,datasender,10);
								strcat(datasender,"LM\0");
								SendPc(datasender, 5, 0);
							}
							else{
								motsender[0]=0;
								strcat(motsender,voltage);
								strcat(motsender,"E\0");
								SendPc(motsender, 5, 0);
								GiveVolt(voltage);
								givecount++;
								eqstepcounter++;
							}
						}
					}

				}
				else{
					eqstepcounter=0;
					givecount=0;
					osccount++;
				}
			}
			else{
				osc=0;
				osccount=0;
				stepfin=0;
				oscpass=2;
				eqstepcounter=0;
				givecount=0;
				sendexp=1;
				ftos(firstpt,voltage);
				motsender[0]=0;
				strcat(motsender,voltage);
				strcat(motsender,"E\0");
				SendPc(motsender, 5, 0);
				GiveVolt(voltage);
			}
		}
	}

	else if(eqstep==1){//equal step
		if(eqstepcounter<2*stepnum){
			if(eqstepcounter<stepnum){
				if(givecount%2==0){
					motsender[0]=0;
					datasender[0]=0;
					itoa(loadnow,datasender,10);
					strcat(datasender,"LM\0");
					strcat(motsender,datasender);
					SendPc(motsender, 5, 0);
					if(firstdir==0){
						stepvolt=firstpt-stepdepth*(eqstepcounter+1);//
					}
					else{
						stepvolt=firstpt+stepdepth*(eqstepcounter+1);//
					}
					voltnow=stepvolt;
					ftos(stepvolt,voltage);
				}
				else{
					motsender[0]=0;
					strcat(motsender,voltage);
					strcat(motsender,"E\0");
					SendPc(motsender, 5, 0);
					GiveVolt(voltage);
					eqstepcounter++;
				}
				givecount++;
			}
			else if(eqstepcounter==stepnum&&calib.holdDur!=0){
				calHoldCount++;
				motsender[0]=0;
				datasender[0]=0;
				itoa(loadnow,datasender,10);
				strcat(datasender,"LM\0");
				strcat(motsender,datasender);
				SendPc(motsender, 5, 0);
				if(calHoldCount==calib.holdDur){
					eqstepcounter++;
					TimeSet(&htim10, caltime*1000);
					if(returncheck==1){
						calHoldCount=2;
					}
					else{
						calHoldCount=0;
						calib.holdDur=0;
					}
				}
				if(calHoldCount==1){
					TimeSet(&htim10, 1000);
				}
			}
			else if(eqstepcounter>=stepnum-1&&returncheck==1){
				if(givecount%2==0){
					motsender[0]=0;
					datasender[0]=0;
					itoa(loadnow,datasender,10);
					strcat(datasender,"LM\0");
					strcat(motsender,datasender);
					SendPc(motsender, 5, 0);
					firstpt=voltnow;
					if(firstdir==1){
						stepvolt=firstpt-stepdepth;//
					}
					else{
						stepvolt=firstpt+stepdepth;//
					}
					voltnow=stepvolt;
					ftos(stepvolt,voltage);
					if(calib.holdDur!=0 && calHoldCount==2){
						stepnum=stepnum+1;
						calHoldCount=0;
					}
				}
				else{
					motsender[0]=0;
					strcat(motsender,voltage);
					strcat(motsender,"E\0");
					SendPc(motsender, 5, 0);
					GiveVolt(voltage);
					eqstepcounter++;
				}
				givecount++;
			}
			else if(eqstepcounter>=stepnum&&returncheck==0){
				eqstepcounter=2*stepnum;
			}

		}
		else{
			if(expcount!=-1){
				eqstep=0;
				eqstepcounter=0;
				givecount=0;
				firstdir=0;
				sendexp=1;
				TimeSet(&htim10, 100000);
			}
			else if(calib.holdDur!=0 && returncheck!=0){
				calHoldCount++;
				motsender[0]=0;
				datasender[0]=0;
				itoa(loadnow,datasender,10);
				strcat(datasender,"LM\0");
				strcat(motsender,datasender);
				SendPc(motsender, 5, 0);
				if(calHoldCount==calib.holdDur){
					calib.holdDur=0;
					calHoldCount=0;
				}
				if(calHoldCount==1){
					TimeSet(&htim10, 1000);
				}
			}
			else{
				eqstep=0;
				eqstepcounter=0;
				givecount=0;
				firstdir=0;
				HAL_TIM_Base_Stop_IT(&htim10);
				readingpc[0]=0;
				strcat(readingpc,"Calibration_Finished\0");
				SendPc(readingpc, 5, ID_FIND);
				uart2say=1;
				ProcessRx(3);
				TimeSet(&htim12, 100000);
			}
		}
	}
	else{
		HAL_TIM_Base_Stop_IT(&htim10);
		uart2say=0;
		SendPc("TAMAMDIR", 5, 0);
	}

}

void Timer11(void){
	//HAL_GPIO_TogglePin(Y_CCW_GPIO_Port, Y_CCW_Pin);
	//stepsay++;
	//if(stepsay/(bossayac*10+10)>5){
	//	bossayac++;
	//	stepsay=0;
	//	TimeSet(&htim11, 1000000/bossayac);
	//}
	//if(bossayac==100000){
	//	bossayac=0;
	//}
	if(pccom==0){
		pcInit.RxSize=1;
		pcInit.ComId=1;
		_uartcom3=&pcInit;
		HAL_UART_Receive_IT(&huart3,_uartcom3->RxBuf , 1);
		if(initalize<5){
			initalize=-1;
		}
	}
	else{
	if (initalize==0&&tryuc%2==0){//Resack of com2 pc com..
		SendAct("", "");
	}
	if(contproc==1&&tryuc%2==0){
		adding[0]='$';
		adding[1]='\0';
		if(initalize>0&&initalize<3){
			mesaj="1\0";
			SendAct(adding,mesaj);
			HAL_TIM_Base_Stop_IT(&htim11);
		}
		else if(initalize>2&&initalize<5){
			mesaj="0\0";
			SendAct(adding,mesaj);
			HAL_TIM_Base_Stop_IT(&htim11);
		}
		else if(initalize==5){//initially set for internal control.
			adding[0]='T';
			mesaj="1\0";
			SendAct(adding, mesaj);
			HAL_TIM_Base_Stop_IT(&htim11);
			contproc=0;
			SendPc("Connected_to_UC45_",5, ID_FIND);
			initalize=6;
		}
	}
	}

}

void Timer12(void){
	if (!(_uartcom1->UResult & Rx_Cplt)&&sendexp==-1&&eqstep==0&&tryuc%2==0)
	{
		Rx_Timer1++;
		if (100 <= Rx_Timer1)
		{
			_uartcom1->UResult |= Rx_Tout;
			Rx_Timer1 = 0;
			if(pccom==1&&tryuc%2==0)
			{
				pcInit.ComId=2;
				SendPc("UC45_NOT_RESPONDING!", 5, ID_FIND);
				HAL_TIM_Base_Start_IT(&htim11);
				tryuc++;
			}
			initalize=0;
			contproc=0;
		}
	}
	else if((_uartcom1->UResult & Rx_Cplt)){
		Rx_Timer1=0;
	}

	//	if ((!(_uartcom1->UResult & Tx_Cplt)) && (!(_uartcom1->UResult & Tx_Tout)))
	//	{
	//		Tx_Timer1++;
	//		if (500 <= Tx_Timer1)
	//		{
	//			_uartcom1->UResult |= Tx_Tout;
	//			Tx_Timer1 = 0;
	//		}
	//	}
	if (!(_uartcom3->UResult & Rx_Cplt))
	{
		Rx_Timer3++;
		if (10 <= Rx_Timer3)
		{
			DummyRead2();
			_uartcom3->UResult |= Rx_Tout;
			if((sendexp!=-1||retracting==1)&&writingpc[0]!=0 && uart2say==1 && sendingPc==0 && readPc==0){
				SendPc(writingpc, _uartcom3->RxSize, 0);
				Rx_Timer3 = 0;
			}
		}
	}
	//
	//	if ((!(_uartcom3->UResult & Tx_Cplt)) && (!(_uartcom3->UResult & Tx_Tout)))
	//	{
	//		Tx_Timer3++;
	//		if (5000 <= Tx_Timer3)
	//		{
	//			pccom=0;
	//			pcInit.ComId=1;
	//			_uartcom3->UResult |= Tx_Tout;
	//			Tx_Timer3 = 0;
	//			HAL_TIM_Base_Start_IT(&htim11);
	//		}
	//	}


}

void Timer13(void){
	if(movxy==1){
		if(xmov==1){
			HAL_GPIO_WritePin(L293DD_ENABLE1_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(XN_GPIO_Port, XN_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, GPIO_PIN_SET);
			xmov=2;
		}
		else if(xmov==2){
			HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, GPIO_PIN_RESET);
			y_enc[0]=TIM2->CNT;
			x_enc[0]=TIM8->CNT;
			if(xcount>=xsteps){
				HAL_GPIO_WritePin(L293DD_ENABLE1_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_RESET);
				xmov=0;
				if(ymov==0){
					movxy=0;
				}
				xysend[0]=0;
				itoa(x_enc[0],xysend,10);
				strcat(xysend,"+Y\0");
				SendPc(xysend, 5, 0);
				if(joyint2>9){
					joyint2=joyint2-12;
				}
				else if(joyint2>5){
					joyint2 = joyint2 - 6;
				}
				if(joyint2==0){
					//TimeSet(&htim13, 100000);
					isHXcom=0;
					if(isAutoApproach==2){
						autoApproach(3);
						encodMove=encodMove-1;
					}
				}
			}
			else{
				xmov=1;
				if(encodMove %2==1){
					xcount=x_enc[0]-32767;
				}
				else{
					xcount++;
				}
			}
		}
		else if(xmov==3){
			HAL_GPIO_WritePin(L293DD_ENABLE1_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(XP_GPIO_Port, XP_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(XN_GPIO_Port, XN_Pin, GPIO_PIN_SET);
			xmov=4;
		}
		else if(xmov==4){
			HAL_GPIO_WritePin(XN_GPIO_Port, XN_Pin, GPIO_PIN_RESET);
			y_enc[0]=TIM2->CNT;
			x_enc[0]=TIM8->CNT;
			if(xcount>=xsteps){
				HAL_GPIO_WritePin(L293DD_ENABLE1_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_RESET);
				xmov=0;
				if(ymov==0){
					movxy=0;
				}
				xysend[0]=0;
				itoa(x_enc[0],xysend,10);
				strcat(xysend,"-Y\0");
				SendPc(xysend, 5, 0);
				if(joyint2>9){
					joyint2=joyint2-12;
				}
				else if(joyint2>5){
					joyint2 = joyint2 - 6;
				}
				if(joyint2==0){
					//TimeSet(&htim13, 100000);
					isHXcom=0;
					if(isAutoApproach==2){
						encodMove=encodMove-1;
						autoApproach(3);
					}
				}
			}
			else{
				xmov=3;
				if(encodMove %2==1){
					xcount=32767-x_enc[0];
				}
				else{
					xcount++;
				}
			}

		}
		if(ymov==1){
			HAL_GPIO_WritePin(L293DD_ENABLE2_GPIO_Port, L293DD_ENABLE2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(YN_GPIO_Port, YN_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(YP_GPIO_Port, YP_Pin, GPIO_PIN_SET);
			ymov=2;
		}
		else if(ymov==2){
			HAL_GPIO_WritePin(YP_GPIO_Port, YP_Pin, GPIO_PIN_RESET);
			y_enc[0]=TIM2->CNT;
			x_enc[0]=TIM8->CNT;
			if(ycount>=ysteps){
				HAL_GPIO_WritePin(L293DD_ENABLE2_GPIO_Port, L293DD_ENABLE2_Pin, GPIO_PIN_RESET);
				ymov=0;
				if(xmov==0){
					movxy=0;
				}
				xysend[0]=0;
				itoa(y_enc[0],xysend,10);
				strcat(xysend,"+X\0");
				SendPc(xysend, 5, 0);
				if(joyint2 %2 ==0){
					joyint2 = joyint2-2;
				}
				else if(joyint2 %3 == 0){
					joyint2 = joyint2-3;
				}
				if(joyint2==0){
					//TimeSet(&htim13, 100000);
					isHXcom=0;
					if(isAutoApproach==2){
						encodMove=encodMove-2;
						autoApproach(3);
					}
				}
			}
			else{
				ymov=1;
				if(encodMove >=2){
					ycount=y_enc[0]-32767;
				}
				else{
					ycount++;
				}
			}
		}
		else if(ymov==3){
			HAL_GPIO_WritePin(L293DD_ENABLE2_GPIO_Port, L293DD_ENABLE2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(YP_GPIO_Port, YP_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(YN_GPIO_Port, YN_Pin, GPIO_PIN_SET);
			ymov=4;
		}
		else if(ymov==4){
			HAL_GPIO_WritePin(YN_GPIO_Port, YN_Pin, GPIO_PIN_RESET);
			y_enc[0]=TIM2->CNT;
			x_enc[0]=TIM8->CNT;
			if(ycount>=ysteps){
				HAL_GPIO_WritePin(L293DD_ENABLE2_GPIO_Port, L293DD_ENABLE2_Pin, GPIO_PIN_RESET);
				ymov=0;
				if(xmov==0){
					movxy=0;
				}
				ycount=0;
				ysteps=0;
				xysend[0]=0;
				itoa(y_enc[0],xysend,10);
				strcat(xysend,"-X\0");
				SendPc(xysend, 5, 0);
				if(joyint2 %2 ==0){
					joyint2 = joyint2-2;
				}
				else if(joyint2 %3 == 0){
					joyint2 = joyint2-3;
				}
				if(joyint2==0){
					//TimeSet(&htim13, 100000);
					isHXcom=0;
					if(isAutoApproach==2){
						encodMove=encodMove-2;
						autoApproach(3);
					}
				}
			}
			else{
				ymov=3;
				if(encodMove >=2){
					ycount=32767-y_enc[0];
				}
				else{
					ycount++;
				}
			}
		}
	}

	if(isHXcom==0){
		if(HX711_Available()){
			instant=HX711_Value(instant,1);
		}
		if(loadFeed==0){
			loadnow=instant.scalelive;
		}
	}

	if(approaching==1&&automot==1&&isAutoApproach==4){//!!automot cond control need
		if(loadcon==1&&(loadnow>=contact||(app==2&&pcDecision==0))){
			automot=0;
			stepsay=0;
			stopmot=0;
			motorTouchpt=motpos;
		}
	}
	else if(automot==1&&dir==0&&approaching==0){
		if(loadcon==1&&(loadnow>=contact||(app==2&&pcDecision==0))){
			sensdist=0;
			motorTouchpt=motpos;
		}
		else{
			sensdist=1;
		}
	}
	else if(retracting==1&&automot==1&&isAutoApproach==4){//!!automot cond control need
		if(loadcon==1&&(loadnow<=contact||(app==2&&pcDecision==0))){
			automot=0;
			stepsay=0;
			stopmot=0;
		}
	}
	if(contsend==1&&sendingPc==0&&readPc==0&&automot==0){
		datasender[0]=0;
		itoa(loadnow,datasender,10);
		strcat(datasender,"LM\0");
		SendPc(datasender, 5, 2);
	}
	if(isAutoApproach==3)
	{
		if(app==1||app==2){
			if(appOrret){
				if(((loadnow<thresholdApp&&app==1)||(pcDecision==1&&app==2)) && (voltnow >= -1000000 && motorcon==0) && emergency==0){
					approaching=1;
					if((onlyAct==0 && voltnow <= first-delta)||motorapp==1){
						if(motorapp==0){//!(10um) move -->& scan > ~10um
							voltnow=voltnow+delta;
							ftos(voltnow,voltage);
							motsender[0]=0;
							datasender[0]=0;
							itoa(loadnow,motsender,10);
							strcat(motsender,"LM\0");
							strcat(datasender,voltage);
							strcat(datasender,"E\0");
							strcat(motsender,datasender);
							SendPc(motsender, 5, 4);
							GiveVolt(voltage);
							if(speedmode==3){
								stepsay=2*2;
							}
							else if(speedmode==4){
								stepsay=4*2;
							}
							else if(speedmode==5){
								stepsay=8*2;
							}
							automot=0;
							TimeSet(&htim9, 10000);
						}
						else{
							speedmode=5;
							automot=1;
							TimeSet(&htim9, 200000);
						}
						isAutoApproach=4;
						StepD(0);
					}
					else if(((loadnow<thresholdApp&&app==1)||(pcDecision==1&&app==2)) && voltnow-increment < -1000000 ){
						SendPc("OutRange", 5, 4);
						HAL_TIM_Base_Stop_IT(&htim10);
						isAutoApproach=0;
					}
					else if(motorapp!=1 && durummot==0){
						voltnow=voltnow-increment;
						ftos(voltnow,voltage);
						motsender[0]=0;
						datasender[0]=0;
						itoa(loadnow,motsender,10);
						strcat(motsender,"LM\0");
						strcat(datasender,voltage);
						strcat(datasender,"E\0");
						strcat(motsender,datasender);
						SendPc(motsender, 5, 0);
						GiveVolt(voltage);
					}
				}
				else if((loadnow < thresholdApp && app==1)){
					isAutoApproach=0;
					SendPc("Not_Approached\0", 5, 4);
					uart2say=1;
					sendexp=1;
					TimeSet(&htim12, 100000);
					approaching=0;
				}
				else if((emergency==0&&app==1)){
					stopmot=1;
					isAutoApproach=5;
					SendPc("Approach_Complete\0", 5, 4);
					uart2say=1;
					sendexp=1;
					TimeSet(&htim12, 100000);
					actTouchpt=voltnow;
					initialMotor=motpos;
					approaching=0;
					//Save actuator and motor psts for retract etc;
				}
				pcDecision=0;
				emergency=0;
			}
			else{
				if(dxy){
					if(motorapp){
						speedmode=5;
						stepsay=motpos-initialMotor;
						automot=0;
						TimeSet(&htim9, 200000);
						StepD(1);
					}
					else{
						if(voltnow < actTouchpt+3*increment && voltnow+increment<7500000){
							voltnow=voltnow+increment;
							ftos(voltnow,voltage);
							motsender[0]=0;
							datasender[0]=0;
							itoa(loadnow,motsender,10);
							strcat(motsender,"LM\0");
							strcat(datasender,voltage);
							strcat(datasender,"E\0");
							strcat(motsender,datasender);
							if(sendingPc==0){
								SendPc(motsender, 5, 0);
							}
							GiveVolt(voltage);
						}
						else{
							dxy=0;
							retracting=0;
							thresholdApp = contact + loadnow;
							if(expcount<step){
								isAutoApproach=1;
							}
							else{//if its last step
								isAutoApproach=5;
								uart2say=1;
								TimeSet(&htim12, 100000);
								SendPc("Retract_Complete\0", 5, 4);
							}
						}
					}
				}
				else if(emergency==0 && holdRet!=1 && (((loadnow > thresholdApp && app==1) || (pcDecision==2 && app==2)) && voltnow+increment < 7500000 && motorapp==0)){
					if(holdRet!=2 && ((loadnow<=holdPos&&app==1)||pcDecision==3)){//pcDecision for hold at retract state
						holdRet=1;
					}
					else{
						voltnow=voltnow+increment;
						ftos(voltnow,voltage);
						motsender[0]=0;
						datasender[0]=0;
						itoa(loadnow,motsender,10);
						strcat(motsender,"LM\0");
						strcat(datasender,voltage);
						strcat(datasender,"E\0");
						strcat(motsender,datasender);
						if(sendingPc==0){
							SendPc(motsender, 5, 0);
						}
						GiveVolt(voltage);
					}
				}
				else if(holdRet==1){
					holdRetCnt++;
					if(holdRetCnt>=holdRetDur){
						holdRetCnt=0;
						holdRet=2;
					}
				}
				else if(emergency == 0 && (motorapp==1 || (voltnow+increment >= 7500000 && onlyAct==0 && ( (loadnow > thresholdApp && app==1) || (pcDecision==2&&app==2) ) ) )){
					isAutoApproach=4;
					automot=1;//burada sürekli çekiyoruz artık motorla ..
					TimeSet(&htim9, 200000);
					StepD(1);
					//first=voltnow+delta;
					pcDecision=0;
				}
				else if((loadnow <= thresholdApp && app==1) && emergency == 0){//!!
					retracting=2;
					uart2say=1;
					TimeSet(&htim12, 100000);
					if(expcount<step){
						isAutoApproach=1;
					}
					else{//if its last step
						retracting=0;
						isAutoApproach=5;
						uart2say=1;
						TimeSet(&htim12, 100000);
						SendPc("Retract_Complete\0", 5, 4);
					}
				}
				emergency=0;
			}
		}
		else if(app==0){
			if(!appOrret){
				//get to initial pst !motor con only motor step !not motor con only actuator step
				if(motorcon==0){
					retracting=1;
					if(voltnow < initialPos + 5*increment && voltnow+increment < 7500000){
						voltnow=voltnow+increment;
						ftos(voltnow,voltage);
						motsender[0]=0;
						datasender[0]=0;
						itoa(loadnow,motsender,10);
						strcat(motsender,"LM\0");
						strcat(datasender,voltage);
						strcat(datasender,"E\0");
						strcat(motsender,datasender);
						if(sendingPc==0){
							SendPc(motsender, 5, 0);
						}
						GiveVolt(voltage);
					}
					else{
						thresholdApp = contact + loadnow;
						uart2say=1;
						TimeSet(&htim12, 100000);
						retracting=0;
						isAutoApproach=1;
					}

				}
				else{
					isAutoApproach=4;
					retracting=1;
					automot=1;
					speedmode=5;
					stepsay=motpos-initialMotor;
					TimeSet(&htim9, 200000);
					StepD(1);
				}
			}
/*			else if(holdRet!=1){
				//retract to (initial)+some threshold pst !before xy pst !depending on control -motor->1fullstep or actuator->7500000.
				if(motorcon==0){
					if(voltnow+increment<initialPos){
						voltnow=voltnow+increment;
						if(holdRet!=2 && voltnow>=holdPos){//parametre değişecek
							holdRet=1;
						}
						ftos(voltnow,voltage);
						motsender[0]=0;
						datasender[0]=0;
						itoa(loadnow,motsender,10);
						strcat(motsender,"LM\0");
						strcat(datasender,voltage);
						strcat(datasender,"E\0");
						strcat(motsender,datasender);
						if(sendingPc==0){
							SendPc(motsender, 5, 0);
						}
						GiveVolt(voltage);
					}
					else{
						retracting=0;
						SendPc("Retract_Complete\0", 5, 4);
						uart2say=1;
						TimeSet(&htim12, 100000);
						if(expcount+1<step){
							isAutoApproach=1;
						}
						else{
							isAutoApproach=0;
						}
					}

				}
				else{//.. step motor secure position for exp..~initialPos+10-20step
					speedmode=5;
					stepsay=16;
					TimeSet(&htim9, 100000);
					StepD(1);
				}
			}
			else if(holdRet==1){
				holdRetCnt++;
				if(holdRetCnt>=holdRetDur){
					holdRetCnt=0;
					holdRet=2;
				}
			}*/
		}

	}
	else if(loadIndent==1){
		if((expin[expcount].depth-thresholdApp>loadnow)||pcDecision==1){
			if(motorcon){
				//motor step & auto sets..
				if(joyint1!=1){
					stopmot=1;
					TimeSet(&htim9, 200000);
					speedmode=5;
					automot=1;
					StepD(0);
					joyint1=1;
				}
			}
			else{
				if(givecount%2==0){
					if(holdcount<2){
						motsender[0]=0;
						datasender[0]=0;
						itoa(loadnow,datasender,10);
						strcat(datasender,"LM\0");
						strcat(motsender,datasender);
						if(sendingPc==0){
							SendPc(motsender, 5, 0);
						}
					}
					stepvolt=firstpt-stepdepth*(eqstepcounter+1);
					voltnow=stepvolt;
					ftos(stepvolt,voltage);
				}
				else{
					motsender[0]=0;
					if(voltnow<=-1000000){
						loadIndent=0;
						SendPc("OutRange", 5, 4);
					}
					else{
						strcat(motsender,voltage);
						strcat(motsender,"E\0");
						if(sendingPc==0){
							SendPc(motsender, 5, 0);
						}
						GiveVolt(voltage);
						eqstepcounter++;
					}
				}
				givecount++;
			}
			pcDecision=0;
		}
		else if(expin[expcount].depth+thresholdApp<loadnow || pcDecision==2){
			if(motorcon){
				if(joyint1!=2){
					stopmot=1;
					TimeSet(&htim9, 200000);
					speedmode=5;
					automot=1;
					StepD(1);
					joyint1=2;
				}
			}
			else{
				if(givecount%2==0){
					if(holdcount<2){
						motsender[0]=0;
						datasender[0]=0;
						itoa(loadnow,datasender,10);
						strcat(datasender,"LM\0");
						strcat(motsender,datasender);
						if(sendingPc==0){
							SendPc(motsender, 5, 0);
						}
					}
					stepvolt=firstpt+stepdepth*(eqstepcounter+1);
					voltnow=stepvolt;
					ftos(stepvolt,voltage);
				}
				else{
					motsender[0]=0;
					if(voltnow>=7500000){
						loadIndent=0;
						SendPc("OutRange", 5, 4);
						TimeSet(&htim9, 100000);
						speedmode=5;
						automot=1;
						StepD(1);
					}
					else{
						strcat(motsender,voltage);
						strcat(motsender,"E\0");
						if(sendingPc==0){
							SendPc(motsender, 5, 0);
						}
						GiveVolt(voltage);
						eqstepcounter++;
					}
				}
				givecount++;
			}
			pcDecision=0;
		}
		else{
			if(motorcon){
				stopmot=1;
				joyint1=0;
			}
			else{

			}
			sendexp=1;
		}
	}
}
void TimeSet(TIM_HandleTypeDef *htim, int duration){
	HAL_TIM_Base_Stop_IT(htim);
	if(duration>=100000){ //greater than 100ms up to 6.5 sec
		htim->Instance->PSC=16799;
		htim->Instance->ARR=(duration/1000)-1;
	}
	else if(duration<=1000){ //smaller than 1ms
		htim->Instance->PSC=84;
		htim->Instance->ARR=(duration/5)-1;
	}
	else{ //1ms-100ms range--
		htim->Instance->PSC=1679;
		htim->Instance->ARR=(duration/100)-1;
	}
	htim->Init.Period=htim->Instance->ARR;
	htim->Init.Prescaler=htim->Instance->PSC;
	htim->Instance->CNT=0;
	if(htim!=&htim9){
		HAL_TIM_Base_Start_IT(htim);
	}
	else{
		if(duration>=10000){
			posinf=2;
		}
		else{
			posinf=(10000/duration);
		}
	}
}
