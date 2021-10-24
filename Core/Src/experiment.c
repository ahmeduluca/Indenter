/*
 * experiment.c
 *
 *  Created on: Feb 9, 2021
 *      Author: ahmeduluca
 */


#include "experiment.h"
#include "heater.h"
//#include "pc_uart.h"
//extern UARTCOM pcInit;

extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;

//extern UART_HandleTypeDef huart2;
//extern uint8_t _TxBuf2[TXBUFSIZE2];
//extern uint8_t _RxBuf2[RXBUFSIZE2];
extern uint32_t Tx_Timer2;
extern char writingpc [100];

extern HX711 instant;

INITIALCOM INIT0;
EXPERIMENT expin[100];
EXPERIMENTrx exprx[100];
CALIBRATION calib;
CALIBRATIONrx calibrx;
enum approach app;


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

//genel sayac ve parametreler
int initalize=0;//uc45 initialize array cont
int contproc=0;//uc45 com start cont
int sendinit=0;//timer2 control initparams activate
int S=0;
int emergency=0;
long voltnow=7500000;
int givecount=0;
int holder=0;

//indent ve cal parametreleri
char voltage []= {0,0,'1','.',0x32,0x33,0x34,0x35,0x36,0x37,0};
uint8_t isAutoApproach=0;
int oscdown=0;
int autoretract=0;
int eqstep=0;//timer2 control eqstep activate
int sendexp=-1;//timer2 control execute exp
int step=0;//exp step sayısı
int returncheck=0;//calibration forward backward
int caltime=100;
long stepdepth=0;
long stepvolt=0;
int stepnum=0;
long firstpt=7500000;
int eqstepcounter=0;
int stepno=0;
int expcount=-1;
int firstdir=0;
int expLoad=0;
int loadFeed=HXLOAD;//HXLOAD;ADCLOAD or NILOAD->through PC control in need!
int loadnow=0;

//oscillation params..
char hivolt [10]={'5','.','2','5','0','0','0','0',0,0};
char lovolt[10]={'-','0','.','2','5','0','0','0','0',0};
int osc=0;//timer2 control oscillation activate
int numosc=0;
int osccount=0;
int oscpass=0;
int holdcount=0;
int sqrmod=0;
//motor params
char tmr[10];
char stepper[12];
char reader [100];
char motsender[100];
int sayacmot=0;
int durummot=0;
int stepsay=0;
int dir=0;
int stopmot=0;
int tryuc=-1;
int tim4say0=0;
int bossayac=1;
int motorcon=0;
int motorapp=0;
int sqrdur=0;
int sensdist=1;
int automot=0;
int endstop=0;
int posinf=1;
int speedmode=0;
int retracting=0;
int upp=0;
int motpos=0;
int motgain=1;
int control=0;
int controlLoad=0;

//XY pst params
int xnow=0;
int ynow=0;
int xco=1;
int yco=1;
int movxy=0;
int xmov=0;
int ymov=0;
int xsteps=0;
int ysteps=0;
extern int joyint2;
int encodMove=0;
extern int joyint1;
extern int xmov;
extern int ymov;

extern int holdRetDur;
extern int holdRet;
int initialPos=7500000;
int holdPos=0;
int first=0;
int delta=1000000;//1 full actuator scan range 5um
int increment=200;//100ms incs for approximately 20nm/s
int appOrret=0;
int loadIndent=0;
int dxy=0;
extern int initialMotor;
extern int approaching;
//heater params
int sampleChannel=0;
extern int heatDuty[3];
extern int heatFeed[3];
extern long thresholdApp;

void ftos(long a, char*s){
	char buf1;
	char buf2;
	int i=1;
	int eksis=0;
	int eks=0;
	itoa(a,s,10);
	int size=SizeOfArray(s);
	if(s[0]=='-'){
		if(size==8){

		}
		if(size==7){
			char buf4 [11]={'-','0',0,0,0,0,0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				if(buf4[k]=='-'&&eksis==0){
					s[k]=buf4[k];
					eksis=1;
				}
				else if(buf4[k]!='-'){
					s[k-eks]=buf4[k];
				}
				else{
					eks=1;
				}
			}
		}
		else if(size==6){
			char buf4 [11]={'-','0','0',0,0,0,0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				if(buf4[k]=='-'&&eksis==0){
					s[k]=buf4[k];
					eksis=1;
				}
				else if(buf4[k]!='-'){
					s[k-eks]=buf4[k];
				}
				else{
					eks=1;
				}
			}
		}
		else if(size==5){
			char buf4 [11]={'-','0','0','0',0,0,0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				if(buf4[k]=='-'&&eksis==0){
					s[k]=buf4[k];
					eksis=1;
				}
				else if(buf4[k]!='-'){
					s[k-eks]=buf4[k];
				}
				else{
					eks=1;
				}
			}
		}
		else if(size==4){
			char buf4 [11]={'-','0','0','0','0',0,0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				if(buf4[k]=='-'&&eksis==0){
					s[k]=buf4[k];
					eksis=1;
				}
				else if(buf4[k]!='-'){
					s[k-eks]=buf4[k];
				}
				else{
					eks=1;
				}
			}
		}
		else if(size==3){
			char buf4 [11]={'-','0','0','0','0','0',0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				if(buf4[k]=='-'&&eksis==0){
					s[k]=buf4[k];
					eksis=1;
				}
				else if(buf4[k]!='-'){
					s[k-eks]=buf4[k];
				}
				else{
					eks=1;
				}
			}
		}
		else if(size==2){
			char buf4 [11]={'-','0','0','0','0','0','0',0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				if(buf4[k]=='-'&&eksis==0){
					s[k]=buf4[k];
					eksis=1;
				}
				else if(buf4[k]!='-'){
					s[k-eks]=buf4[k];
				}
				else{
					eks=1;
				}
			}
		}
		i=2;
	}
	else{
		if(size==7){
		}
		if(size==6){
			char buf4 [10]={'0',0,0,0,0,0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				s[k]=buf4[k];
			}
		}
		else if(size==5){
			char buf4 [10]={'0','0',0,0,0,0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				s[k]=buf4[k];
			}
		}
		else if(size==4){
			char buf4 [10]={'0','0','0',0,0,0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				s[k]=buf4[k];
			}
		}
		else if(size==3){
			char buf4 [10]={'0','0','0','0',0,0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				s[k]=buf4[k];
			}
		}
		else if(size==2){
			char buf4 [10]={'0','0','0','0','0',0,0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				s[k]=buf4[k];
			}
		}
		else if(size==1){
			char buf4 [10]={'0','0','0','0','0','0',0,0,0,0};
			strcat(buf4,s);
			for(int k=0;k<10;k++){
				s[k]=buf4[k];
			}
		}
	}
	buf1=s[i];//2
	s[i]='.';//1.345
	for(int j=i;j<i+6;j++){//12345
		buf2=s[j+1];//34
		s[j+1]=buf1;//1.234
		buf1=buf2;
	}
	s[i+7]=0;
}
void ParaInt(uint8_t * param){
	int i =0;
	if(param[0]=='W'){
		int j=1;
		i=1;
		while(param[i]!=0&&param[i]!='E'){
			INIT0.voltage[i-1]=param[i];
			if(param[j]!='.'&&param[j]!='E'){
				voltage[i-1]=param[j];
			}
			else if(param[j]=='.'){
				j++;
				voltage[i-1]=param[j];
			}
			j++;
			i++;
		}
		voltage[i-2]=0;
		voltnow=atoi(voltage);
	}
	else if(param[0]=='P'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.kp[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='I'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.ki[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='D'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.kd[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='C'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.filtertype[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='F'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.cutoff1[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='S'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.cutoff2[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='T'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.opmode[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='M'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.vmax[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='N'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.vmin[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='G'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.deltaS[i]=param[i];
			i++;
		}
	}
	else if(param[0]=='B'){
		while(param[i]!=0&&param[i]!='E'){
			INIT0.clc[i]=param[i];
			i++;
		}
	}


}

//Experiment Functions:
void Cal(long depth, int steps, long inter){
	caltime=50;
	stepnum=steps;
	firstpt=voltnow;
	stepdepth=depth/stepnum;
	if(inter>50||(motorcon==1&&inter>0)){
		caltime=inter/2;
	}
	if(osc!=1){
		eqstep=1;
	}
	if(motorcon==0){
		TimeSet(&htim10, caltime*1000);
	}
	else{
		HAL_TIM_Base_Stop_IT(&htim10);
		eqstep=1;
		TimeSet(&htim9, caltime*1000);
		stepsay=depth;
		if(firstdir==1){
			StepD(1);
		}
		else{
			StepD(0);
		}
	}
}
void slopExt(int type, int xinter, int yinter, int threshold, int pull, int zinc){
	int first=voltnow;
	int touchpt[9];
	int cnt=0;
	if(type==0){//Actuator
		app=1;
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				while(loadnow<threshold&&emergency==0){
					voltnow-=zinc;
					ftos(voltnow,voltage);
					motsender[0]=0;
					datasender[0]=0;
					itoa(instant.scalelive,motsender,10);
					strcat(motsender,"LM\0");
					strcat(datasender,voltage);
					strcat(datasender,"E\0");
					strcat(motsender,datasender);
					SendPc(motsender, 5, 2);
					GiveVolt(voltage);
					DWT_Delay(200000);
					if(voltnow-zinc<-1000000){
						SendPc("OUTRANG", 5, 0);
						break;
					}
				}
				touchpt[cnt]=voltnow;
				SendPc("TOUCH", 5, 0);// allow PC to keep the point (x,y,z) info
				while(pull>voltnow+zinc&&emergency==0){
					voltnow+=zinc;
					ftos(voltnow,voltage);
					motsender[0]=0;
					datasender[0]=0;
					itoa(instant.scalelive,motsender,10);
					strcat(motsender,"LM\0");
					strcat(datasender,voltage);
					strcat(datasender,"E\0");
					strcat(motsender,datasender);
					SendPc(motsender, 5, 2);
					GiveVolt(voltage);
					DWT_Delay(100000);
				}
				if(emergency==0){
					moveXY(0, 0, xinter,1);
					cnt++;
				}
			}
			if(emergency==0){
				moveXY(0, 1, xinter*3,1);
				moveXY(1, 0, yinter,1);
			}
		}
		if(emergency==0){
			moveXY(1, 1, yinter*3,1);
		}
		//after obtain 9 point height (touch point) info; with help of strain data determine slope
		//for now, strain gage data is read through NI PCIe & PC so UI should handle after this point..
		//to prepare algorithm here, using given voltage and piezo constant, an inner slope extraction will be tried.

		app=0;
	}
	else if(type==1){//step motor
		speedmode=5;//2209'da 5 olsun!
		app=1;
		TimeSet(&htim9, 100000);
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				automot=1;
				StepD(0);
				while(instant.scalelive<threshold){
					if(emergency!=0){
						stopmot=1;
						break;
					}
				}
				stopmot=1;
				touchpt[cnt]=motpos;
				automot=1;
				StepD(0);
				while(first+pull!=motpos){
					if(emergency!=0){
						stopmot=1;
						break;
					}
				}
				stopmot=1;
				if(emergency==0){
					moveXY(0, 0, xinter,1);
					cnt++;
				}
			}
			if(emergency==0){
				moveXY(0, 1, xinter*3,1);
				moveXY(1, 0, yinter,1);
			}
		}
		if(emergency==0){
			moveXY(1, 1, yinter*3,1);
		}
		app=0;
	}
	else if(type==2){//step motor
		speedmode=3;//5 de olmalı
		app=1;
		TimeSet(&htim9, 100000);
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				automot=1;
				StepD(0);
				while(instant.scalelive<threshold){
					if(emergency!=0){
						stopmot=1;
						break;
					}
				}
				stopmot=1;
				touchpt[cnt]=motpos;
				automot=1;
				StepD(0);
				while(first+pull!=motpos){
					if(emergency!=0){
						stopmot=1;
						break;
					}
				}
				stopmot=1;
				if(emergency==0){
					moveXY(0, 0, xinter,1);
					cnt++;
				}
			}
			if(emergency==0){
				moveXY(0, 1, xinter*3,1);
				moveXY(1, 0, yinter,1);
			}
		}
		if(emergency==0){
			moveXY(1, 1, yinter*3,1);
		}
		app=0;
	}

}
void Indent(long depth, long speed, int yon){
//her bir step için yapılacak dizi de açılabilir.
	if (speed!=0&&motorcon==0){
		stepno=depth/(speed*0.05);
		returncheck=0;
		if(yon==2){
			firstdir=1;
		}
		else{
			firstdir=0;
		}
		if(expLoad==1){
			firstpt=voltnow;
			sendexp=0;
			stepdepth=depth/stepno;
			TimeSet(&htim13, 100000);
			loadIndent=1;
		}
		else{
			eqstep=1;
			sendexp=0;
			Cal(depth, stepno, 0);
			//set heater during indent!
		}
	}
	else if(motorcon==1){
		HAL_TIM_Base_Stop_IT(&htim10);
		uart2say=htim10.Instance->DMAR;
		while(uart2say!=0){
			uart2say=htim10.Instance->DMAR;
			HAL_TIM_Base_Stop_IT(&htim10);
		}
		if(speed!=0){
			TimeSet(&htim9, speed*500);
		}
		else{
			TimeSet(&htim9, 200);
		}
		stepsay=depth;
		//Setheater during indent
		if(yon==1){
			StepD(0);
		}
		else{
			StepD(1);
		}
	}
	else if(speed==0&&motorcon==0) {
		setHeater(sampleChannel, heatDuty[sampleChannel], 0);//set heater during indent
		sendexp=1;
		if(yon==2){
			voltnow+=depth;
		}
		else{
			voltnow-=depth;
		}
		ftos(voltnow,voltage);
		motsender[0]=0;
		strcat(motsender,voltage);
		strcat(motsender,"E\0");
		SendPc(motsender, 5, 0);
		GiveVolt(voltage);
	}
	else{
		sendexp=1;
	}
}
void Hold(long hold){
	if(hold>50){
		holder=hold/50;
		TimeSet(&htim10, 50000);
	}
	else{
		holder=0;
		TimeSet(&htim10, 50000);
	}
	holdcount++;
}
void autoApproach(int typ){
	first=voltnow;
	if(loadnow>thresholdApp && typ==2){
		retracting=1;
	}
	if(retracting==1 && typ!=3 && typ != -1 ){
		TimeSet(&htim13, 200000);
		if(app==0){
			increment= expin[expcount].speed * 0.2;//convert volt/sec to 200ms steps of actuator
		}
		if(expin[expcount].stepret!=0){
			if(expLoad){
				holdPos=expin[expcount].removePer*expin[expcount].depth/100;
			}
			else if(loadcon){
				holdPos=expin[expcount].removePer*loadnow/100;
			}
			else{
				holdPos=voltnow+(100-expin[expcount].removePer)*initialPos/100;
			}
			holdRetDur=expin[expcount].removeHold/200;
			holdRet=0;
			dxy=2;
		}
		else{
			dxy=1;
			holdRet=2;
		}
		appOrret=0;
		isAutoApproach=3;
	}
	else if(typ!=3){//directly enter after any retract step; after xy move function used with typ=3 state
		if(expin[expcount].xpst!=0){
			if(expin[expcount].xpst>6){
				isAutoApproach=2;
				moveXY(0, 0, 1, abs(expin[expcount].xpst));//should not be 0-6!!!
			}
			else if(expin[expcount].xpst<-6){
				isAutoApproach=2;
				moveXY(0, 1, 1, abs(expin[expcount].xpst));
			}
		}
		if(expin[expcount].ypst!=0){
			if(expin[expcount].ypst>6){
				isAutoApproach=2;
				moveXY(1, 0, 1, abs(expin[expcount].ypst));
			}
			else if(expin[expcount].ypst<-6){
				isAutoApproach=2;
				moveXY(1, 1, 1, abs(expin[expcount].ypst));
			}
		}
		if(expin[expcount].xpst==0 && expin[expcount].ypst==0 && expcount!=0){// after retract; without any xy movement
			if(app!=0||retracting==2){
				SendPc("Retract_Complete\0", 5, 4);
				uart2say=1;
				TimeSet(&htim12, 100000);
		    	appOrret=1;
			}
			else{
				isAutoApproach=0;
				SendPc("Retract_Complete\0", 5, 4);
				uart2say=1;
				TimeSet(&htim12, 100000);;
			}
		}
		else if(expcount==0 && app!=0){//just for first step flow does not wait for REFIN message!
			TimeSet(&htim13, 200000);
			appOrret=1;
			isAutoApproach=3;
		}
	}
	else if(app!=0){// after retract + xy movement send completion of retract message
		// and wait for REFIN message to start approach
		TimeSet(&htim13, 200000);
		appOrret=1;
		if(expcount!=0){
			SendPc("Retract_Complete\0", 5, 4);
			uart2say=1;
			TimeSet(&htim12, 100000);
		}
	}
	else{// after retract + xy movement send retract completed and wait for REFIN message
		// its for lack of approach routine usage!
		if(expcount!=0){
			SendPc("Retract_Complete\0", 5, 4);
			uart2say=1;
			TimeSet(&htim12, 100000);
		}
		else{
			isAutoApproach=0;
			HAL_TIM_Base_Start_IT(&htim10);
			sendexp=1;
			initialPos=voltnow;
			initialMotor=motpos;
			approaching=0;
		}
	}
	//app || retracting || int dir ||---sendexp--eqstep--osc--automot-:-
}
void Osc(long amp, long period, long dur, int type){
	if(type==1){
		if(oscdown==0){
			firstpt=voltnow;
			ftos(firstpt+amp,hivolt);
			ftos(firstpt-amp,lovolt);
			numosc=2*dur/period;
		}
		else{
			firstpt=voltnow;
			ftos(firstpt,hivolt);
			ftos(firstpt-amp*2,lovolt);
			numosc=2*dur/period;
		}
		sqrmod = 10*(period-0.075*period);//for 50 us cycle of timer, change voltage for sqr wave at half period at each (period/(2*0.05)) th cycle
		// there is a phase between giving voltage change order and application of voltage via UC45 about 50 ms.
		// It should not change for any cycle bcs its communication time for UC45; care should be taken for not to give smaller period than 50 ms
		//-0.075*period is set to achieve exact timing//there is a delay for application of voltage and order via USART
		// 0.075 parameter is calibrated for ~3um/0.9V(*20) pp sqr wave case and will be later justified for all voltage range.
		TimeSet(&htim10, 50);//50 us timer set for square wave oscillation
	}
	else if(type==2){
		numosc=dur/period;
		Cal(2*amp,period/200,0);
	}
}

//Movement Control Functions:

void moveXY(int direk, int delta, int speed, int stepsize){
	//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin) ;
	int time=1;
	switch(speed){
	case 1:
		if(direk){
			time=200;
		}
		else{
			time=20;
		}
		break;
	case 2:
		if(direk){
			time=500;
		}
		else{
			time=50;
		}
		break;
	case 3:
		if(direk){
			time=1000;
		}
		else{
			time=100;
		}
		break;
	case 4:
		if(direk){
			time=5000;
		}
		else{
			time=500;
		}
		break;
	case 5:
		if(direk){
			time=10000;
		}
		else{
			time=1000;
		}
		break;
	default:
		time=speed;
		break;
	}
	switch(stepsize){
	case 1:
		if(direk){
			xsteps=2;
		}
		else{
			ysteps=2;
		}
		break;
	case 2:
		if(direk){
			xsteps=10;
		}
		else{
			ysteps=10;
		}
		break;
	case 3:
		if(direk){
			xsteps=20;
		}
		else{
			ysteps=20;
		}
		break;
	case 4:
		if(direk){
			xsteps=50;
		}
		else{
			ysteps=50;
		}
		break;
	case 5:
		if(direk){
			xsteps=100;
		}
		else{
			ysteps=100;
		}
		break;
	default:
		if(direk){
			xsteps=stepsize;
			encodMove=encodMove+1;
		}
		else{
			ysteps=stepsize;
			encodMove=encodMove+2;
		}
		break;
	}
	if(direk==1){//Xdir
		TIM8->CNT=32767;
		if(delta==0){//+X
			joyint2 = joyint2+12;
			if(movxy==0){
				movxy=1;
				TimeSet(&htim13, time);
			}
			xmov=1;
		}
		else{//-X
			joyint2 = joyint2+6;
			if(movxy==0){
				movxy=1;
				TimeSet(&htim13, time);
			}
			xmov=3;
		}
	}
	else{
		TIM2->CNT=32767;
		if(delta==0){//+Y
			joyint2 = joyint2+2;
			if(movxy==0){
				movxy=1;
				TimeSet(&htim13, time);
			}
			ymov=1;
		}
		else{//-Y
			joyint2 = joyint2+3;
			if(movxy==0){
				movxy=1;
				TimeSet(&htim13, time);
			}
			ymov=3;
		}
	}
}

void StepD(int yon){
	stopmot=0;
	endstop=HAL_GPIO_ReadPin(ENDSTOP_INPUT_GPIO_Port, ENDSTOP_INPUT_Pin);
	if(yon==1){
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
		if(endstop==0){
			stopmot=1;
		}
		dir=1;
	}
	else{
		HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);
		dir=0;
	}
	durummot=1;
	//sensdist=HAL_GPIO_ReadPin(SENSOR_GPIO_Port,SENSOR_Pin);
	if(sensdist==1&&speedmode==0){
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
		motgain=32;
	}
	else if(sensdist==1&&speedmode==1){
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		motgain=16;
	}
	else if(speedmode==2){
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
		motgain=8;
	}
	else if(speedmode==3){
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		motgain=4;
	}
	else if(speedmode==4){
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
		motgain=2;
	}
	else if(speedmode==5){
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		motgain=1;
	}
	else{
		HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
		motgain=4;
	}
	HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_RESET);
	upp=0;
	HAL_TIM_Base_Start_IT(&htim9);
}
void GiveVolt(char * value){
	for (int i=1;i<11;i++){
		voltageval[i]='\0';
	}
	SendAct(voltageval, value);
}
void Emergency(int type){
	movxy=0;
	loadIndent=0;
	isAutoApproach=0;
	HAL_TIM_Base_Stop_IT(&htim10);
	HAL_GPIO_WritePin(L293DD_ENABLE1_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L293DD_ENABLE2_GPIO_Port, L293DD_ENABLE1_Pin, GPIO_PIN_RESET);
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
	S=0;
	eqstepcounter=0;
	givecount=0;
	stepnum=0;
	if(type==0){
		HAL_TIM_Base_Stop_IT(&htim11);
		stopmot=1;
		SendPc("ELECTRICAL PROTECTION", 5, 2);
		GiveVolt("0.000000");
		SendPc("0.000000E", 5, 2);
		uart2say=htim9.Instance->DMAR;
		while(uart2say!=0){
			uart2say=htim9.Instance->DMAR;
			stopmot=1;
			HAL_TIM_Base_Stop_IT(&htim9);
		}
	}
	else{
		HAL_GPIO_WritePin(TMC_EN_GPIO_Port,TMC_EN_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin,GPIO_PIN_RESET);
		stepsay=1000;
		TimeSet(&htim9, 200);
		StepD(1);
		SendPc("TIP&SURFACE PROTECTION", 5, 2);
		GiveVolt("7.500000");
		SendPc("7.500000E", 5, 2);
	}
	emergency=0;
	ProcessRx(3);
}
