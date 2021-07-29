/*
 * experiment.h
 *
 *  Created on: Feb 9, 2021
 *      Author: ahmeduluca
 */

#ifndef INC_EXPERIMENT_H_
#define INC_EXPERIMENT_H_

#include "stm32f4xx_hal.h"
#include "DHT.h"
#include "HX711.h"
#include "uart.h"
#include "timer.h"
#include "pc_uart.h"

#define HXLOAD	0
#define ADCLOAD	1
#define	NILOAD	2

typedef enum
{
	osc_non, osc_sqr, osc_tri
} oscType;

enum approach
{
	none, internal, external
};

typedef struct
{
	char voltage [9];
	char clc [3]; //closeloopcontrol
	char kp [11];
	char ki [15];
	char kd [10];
	char filtertype [3];
	char cutoff1 [15];
	char cutoff2 [15];
	char vmin [11];
	char vmax [11];
	char deltaS [11];
	char opmode [3];
}INITIALCOM;


typedef struct
{
	uint8_t stepId;
	uint8_t dir;
	long xpst;
	long ypst;
	long depth;
	long speed;
	long hold;
	oscType osc;
	long amp;
	long freq;
	long dur;
	long tramp;
	long tspeed;
	long tamp;
	long tfreq;
	long tlag;
	uint8_t ttime;
	uint8_t stepret;
	long removeHold;
	uint8_t removePer;
} EXPERIMENT;


typedef struct
{
	char stepIdrx [3];
	char dirrx [2];
	char xpstrx[10];
	char ypstrx[10];
	char depthrx [10];
	char speedrx [10];//
	char holdrx [10];
	char oscrx [2];
	char amprx [10];
	char freqrx [10];
	char durrx [10];
	char dtrx [6];
	char dtsrx [6];//
	char dtamprx [6];
	char dtwhenrx [2];
	char dtfreqrx [6];
	char dtphaserx [6];
	char retrx [2];
	char removalHold[6];
	char removalPer[3];
} EXPERIMENTrx;


typedef struct
{
	long depth;
	int  nofstep;
	long interval;
	long holdDur;
} CALIBRATION;


typedef struct
{
	char depthrx [10];
	char  nofsteprx [10];
	char intervalrx [10];
	char hold[6];
} CALIBRATIONrx;


void GiveVolt(char * value);
void Osc(long amplitude, long period, long interval, int type);
void Cal(long depth, int steps, long inter);
void Indent(long depth, long speed, int yon);
void Hold(long hold);
void ftos(long a, char*s);
void ParaInt(uint8_t * param);
void StepD(int yon);
void moveXY(int dir, int delta, int speed, int stepsize);
void autoApproach(int dir);
void slopExt(int type, int xinter, int yinter, int threshold, int pull, int zinc);
void Emergency(int type);

#endif /* INC_EXPERIMENT_H_ */
