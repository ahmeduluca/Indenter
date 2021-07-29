/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_LED_Pin GPIO_PIN_2
#define USER_LED_GPIO_Port GPIOE
#define RUN_LED_Pin GPIO_PIN_3
#define RUN_LED_GPIO_Port GPIOE
#define TCRT5000_VCC_EN_Pin GPIO_PIN_4
#define TCRT5000_VCC_EN_GPIO_Port GPIOE
#define XP_Pin GPIO_PIN_5
#define XP_GPIO_Port GPIOE
#define XN_Pin GPIO_PIN_6
#define XN_GPIO_Port GPIOE
#define DHT_Pin GPIO_PIN_13
#define DHT_GPIO_Port GPIOC
#define ENDSTOP_INPUT_Pin GPIO_PIN_14
#define ENDSTOP_INPUT_GPIO_Port GPIOC
#define HXSCK_Pin GPIO_PIN_15
#define HXSCK_GPIO_Port GPIOC
#define P_M_ADC1_Pin GPIO_PIN_0
#define P_M_ADC1_GPIO_Port GPIOC
#define P_M_ADC2_Pin GPIO_PIN_1
#define P_M_ADC2_GPIO_Port GPIOC
#define AY_Pin GPIO_PIN_0
#define AY_GPIO_Port GPIOA
#define BY_Pin GPIO_PIN_1
#define BY_GPIO_Port GPIOA
#define TCRT5000_INPUT_Pin GPIO_PIN_2
#define TCRT5000_INPUT_GPIO_Port GPIOA
#define TC_ADC1_Pin GPIO_PIN_3
#define TC_ADC1_GPIO_Port GPIOA
#define TC_ADC2_Pin GPIO_PIN_4
#define TC_ADC2_GPIO_Port GPIOA
#define TC_ADC3_Pin GPIO_PIN_5
#define TC_ADC3_GPIO_Port GPIOA
#define PROXIMITY_Pin GPIO_PIN_6
#define PROXIMITY_GPIO_Port GPIOA
#define SG_ADC_Pin GPIO_PIN_4
#define SG_ADC_GPIO_Port GPIOC
#define JSZ_Pin GPIO_PIN_5
#define JSZ_GPIO_Port GPIOC
#define JSX_Pin GPIO_PIN_0
#define JSX_GPIO_Port GPIOB
#define JSY_Pin GPIO_PIN_1
#define JSY_GPIO_Port GPIOB
#define JS1_Pin GPIO_PIN_2
#define JS1_GPIO_Port GPIOB
#define JS1_EXTI_IRQn EXTI2_IRQn
#define HXDAT_Pin GPIO_PIN_7
#define HXDAT_GPIO_Port GPIOE
#define JS2_Pin GPIO_PIN_8
#define JS2_GPIO_Port GPIOE
#define JS2_EXTI_IRQn EXTI9_5_IRQn
#define SDIO_CD_Pin GPIO_PIN_15
#define SDIO_CD_GPIO_Port GPIOE
#define L293DD_ENABLE1_Pin GPIO_PIN_12
#define L293DD_ENABLE1_GPIO_Port GPIOB
#define Y_CW_Pin GPIO_PIN_14
#define Y_CW_GPIO_Port GPIOB
#define Y_CCW_Pin GPIO_PIN_15
#define Y_CCW_GPIO_Port GPIOB
#define MPU6050_AD0_Pin GPIO_PIN_8
#define MPU6050_AD0_GPIO_Port GPIOD
#define MPU6050_AD1_Pin GPIO_PIN_9
#define MPU6050_AD1_GPIO_Port GPIOD
#define MPU6050_AD2_Pin GPIO_PIN_10
#define MPU6050_AD2_GPIO_Port GPIOD
#define ESY_N_Pin GPIO_PIN_11
#define ESY_N_GPIO_Port GPIOD
#define ESY_N_EXTI_IRQn EXTI15_10_IRQn
#define ESX_N_Pin GPIO_PIN_12
#define ESX_N_GPIO_Port GPIOD
#define ESX_N_EXTI_IRQn EXTI15_10_IRQn
#define YP_Pin GPIO_PIN_13
#define YP_GPIO_Port GPIOD
#define YN_Pin GPIO_PIN_14
#define YN_GPIO_Port GPIOD
#define L293DD_ENABLE2_Pin GPIO_PIN_15
#define L293DD_ENABLE2_GPIO_Port GPIOD
#define BX_Pin GPIO_PIN_6
#define BX_GPIO_Port GPIOC
#define AX_Pin GPIO_PIN_7
#define AX_GPIO_Port GPIOC
#define UC45TX_Pin GPIO_PIN_9
#define UC45TX_GPIO_Port GPIOA
#define UC45RX_Pin GPIO_PIN_10
#define UC45RX_GPIO_Port GPIOA
#define P_M_ADC2_SIGN_Pin GPIO_PIN_11
#define P_M_ADC2_SIGN_GPIO_Port GPIOA
#define P_M_ADC1_SIGN_Pin GPIO_PIN_12
#define P_M_ADC1_SIGN_GPIO_Port GPIOA
#define TMC_IND_Pin GPIO_PIN_15
#define TMC_IND_GPIO_Port GPIOA
#define TMC_IND_EXTI_IRQn EXTI15_10_IRQn
#define PCTX_Pin GPIO_PIN_10
#define PCTX_GPIO_Port GPIOC
#define PCRX_Pin GPIO_PIN_11
#define PCRX_GPIO_Port GPIOC
#define TMC_DIAG_Pin GPIO_PIN_0
#define TMC_DIAG_GPIO_Port GPIOD
#define TMC_EN_Pin GPIO_PIN_1
#define TMC_EN_GPIO_Port GPIOD
#define DIR_Pin GPIO_PIN_3
#define DIR_GPIO_Port GPIOD
#define MS2_Pin GPIO_PIN_4
#define MS2_GPIO_Port GPIOD
#define TMC_TX_Pin GPIO_PIN_5
#define TMC_TX_GPIO_Port GPIOD
#define TMC_RX_Pin GPIO_PIN_6
#define TMC_RX_GPIO_Port GPIOD
#define STEP_Pin GPIO_PIN_7
#define STEP_GPIO_Port GPIOD
#define MS1_Pin GPIO_PIN_3
#define MS1_GPIO_Port GPIOB
#define IX_Pin GPIO_PIN_4
#define IX_GPIO_Port GPIOB
#define IY_Pin GPIO_PIN_5
#define IY_GPIO_Port GPIOB
#define ESX_P_Pin GPIO_PIN_6
#define ESX_P_GPIO_Port GPIOB
#define ESX_P_EXTI_IRQn EXTI9_5_IRQn
#define ESY_P_Pin GPIO_PIN_7
#define ESY_P_GPIO_Port GPIOB
#define ESY_P_EXTI_IRQn EXTI9_5_IRQn
#define SCL_Pin GPIO_PIN_8
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_9
#define SDA_GPIO_Port GPIOB
#define X_CCW_Pin GPIO_PIN_0
#define X_CCW_GPIO_Port GPIOE
#define X_CW_Pin GPIO_PIN_1
#define X_CW_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
