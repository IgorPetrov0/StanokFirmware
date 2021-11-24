/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern void readMessage();
extern void initDrives();
extern void usbInit();
char incomingMessageFlag;
char errorsArray[20];
char errorsCount;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define X_A_Pin GPIO_PIN_12
#define X_A_GPIO_Port GPIOB
#define X_B_Pin GPIO_PIN_13
#define X_B_GPIO_Port GPIOB
#define X_C_Pin GPIO_PIN_14
#define X_C_GPIO_Port GPIOB
#define X_D_Pin GPIO_PIN_15
#define X_D_GPIO_Port GPIOB
#define Y_A_Pin GPIO_PIN_8
#define Y_A_GPIO_Port GPIOA
#define Y_B_Pin GPIO_PIN_9
#define Y_B_GPIO_Port GPIOA
#define Y_C_Pin GPIO_PIN_10
#define Y_C_GPIO_Port GPIOA
#define Z_B_Pin GPIO_PIN_15
#define Z_B_GPIO_Port GPIOA
#define Z_C_Pin GPIO_PIN_3
#define Z_C_GPIO_Port GPIOB
#define Z_D_Pin GPIO_PIN_4
#define Z_D_GPIO_Port GPIOB
#define Y_D_Pin GPIO_PIN_6
#define Y_D_GPIO_Port GPIOB
#define Z_A_Pin GPIO_PIN_7
#define Z_A_GPIO_Port GPIOB
#define STOP_BUTTON_Pin GPIO_PIN_9
#define STOP_BUTTON_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
