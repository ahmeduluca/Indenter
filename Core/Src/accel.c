/*
 * accel.c
 *
 *  Created on: Feb 9, 2021
 *      Author: ahmeduluca
 */

#include "accel.h"

extern I2C_HandleTypeDef hi2c1;
extern uint16_t mpu[3];

float Ax,Ay,Az,Temper,Gx,Gy,Gz;
uint8_t data=0;
uint8_t Rec_Data[14];

void readMPU(uint16_t id, uint8_t filter, int duration){
	uint16_t Accel_X_RAW,Accel_Y_RAW,Accel_Z_RAW,Temp_RAW,Gyro_X_RAW,Gyro_Y_RAW,Gyro_Z_RAW;
	HAL_I2C_Mem_Read(&hi2c1, id, 0x75, 1, &data, 1, HAL_MAX_DELAY);//Who am I reg
	data=0x00;//Z Gyro PLL as clock source;set 0 for internal 8MHz
	HAL_I2C_Mem_Write(&hi2c1, id, 0x6B, 1,&data, 1, HAL_MAX_DELAY);//Wake Up reg
	data=0x07;//8kHz/1+LSB
	HAL_I2C_Mem_Write(&hi2c1, id, 0x19, 1,&data, 1, HAL_MAX_DELAY);//Sample Rate Divider reg for gyro
	data=0x06;//0x06Low pass filter5Hz-19msDelay -max of filters-
	HAL_I2C_Mem_Write(&hi2c1, id, 0x1A, 1,&data, 1, HAL_MAX_DELAY);//CONFIGLow pass filter
	data=0x00;//+-250 interval set for gyro
	HAL_I2C_Mem_Write(&hi2c1, id, 0x1B, 1,&data, 1, HAL_MAX_DELAY);
	data=0x00;//+-2g interval set for accelerometer
	HAL_I2C_Mem_Write(&hi2c1, id, 0x1C, 1,&data, 1, HAL_MAX_DELAY);
	//read values:
	Ax=0.;
	Ay=0.;
	Az=0.;
	Gx=0.;
	Gy=0.;
	Gz=0.;
	Temper=0.;
	Rec_Data[0]=0;
	HAL_I2C_Mem_Read (&hi2c1, id, 0x3B, 1, Rec_Data, 14, HAL_MAX_DELAY);
	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);
	Temp_RAW = (int16_t)(Rec_Data[6] << 8 | Rec_Data [7]);
	Gyro_X_RAW = (int16_t)(Rec_Data[8] << 8 | Rec_Data [9]);
	Gyro_Y_RAW = (int16_t)(Rec_Data[10] << 8 | Rec_Data [11]);
	Gyro_Z_RAW = (int16_t)(Rec_Data[12] << 8 | Rec_Data [13]);
	Ax = Accel_X_RAW;
	Ay = Accel_Y_RAW;
	Az = Accel_Z_RAW;
	Gx = Gyro_X_RAW/131.0;
	Gy = Gyro_Y_RAW/131.0;
	Gz = Gyro_Z_RAW/131.0;
	Temper= (Temp_RAW)/340.0+36.53;
	mpu[0]=Accel_X_RAW;
	mpu[1]=Accel_Y_RAW;
	mpu[2]=Accel_Z_RAW;
}

void readMMA845(uint16_t id, uint8_t filter, int duration){
	uint16_t Accel_X_RAW,Accel_Y_RAW,Accel_Z_RAW;
	data=0x00;
	HAL_I2C_Mem_Read(&hi2c1, id, 0x0D, 1, &data, 1, HAL_MAX_DELAY);//Who am I reg
		//read values:
		Ax=0.;
		Ay=0.;
		Az=0.;
		Rec_Data[0]=0;
		HAL_I2C_Mem_Read (&hi2c1, id, 0x01, 1, Rec_Data, 6, HAL_MAX_DELAY);
		Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
		Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
		Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);
		Ax = Accel_X_RAW/8192.0;  // get the float g
		Ay = Accel_Y_RAW/8192.0;
		Az = Accel_Z_RAW/8192.0;
		mpu[0]=Accel_X_RAW;
		mpu[1]=Accel_Y_RAW;
		mpu[2]=Accel_Z_RAW;
}
