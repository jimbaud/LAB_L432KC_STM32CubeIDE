/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TFT_PIN_CS_A3_Pin GPIO_PIN_4
#define TFT_PIN_CS_A3_GPIO_Port GPIOA
#define TFT_SPI_CLK_A4_Pin GPIO_PIN_5
#define TFT_SPI_CLK_A4_GPIO_Port GPIOA
#define RGB_Pixel_Led_Pin GPIO_PIN_6
#define RGB_Pixel_Led_GPIO_Port GPIOA
#define TFT_SPI_MOSI_A6_Pin GPIO_PIN_7
#define TFT_SPI_MOSI_A6_GPIO_Port GPIOA
#define TFT_PIN_DC_D3_Pin GPIO_PIN_0
#define TFT_PIN_DC_D3_GPIO_Port GPIOB
#define TFT_PIN_RES_D6_Pin GPIO_PIN_1
#define TFT_PIN_RES_D6_GPIO_Port GPIOB
#define ENOCDER_IN2_D9_Pin GPIO_PIN_8
#define ENOCDER_IN2_D9_GPIO_Port GPIOA
#define ENCODER_IN1_D1_Pin GPIO_PIN_9
#define ENCODER_IN1_D1_GPIO_Port GPIOA
#define ENCODEUR_SWB_D0_Pin GPIO_PIN_10
#define ENCODEUR_SWB_D0_GPIO_Port GPIOA
#define ENCODEUR_SWB_D0_EXTI_IRQn EXTI15_10_IRQn
#define Stepper_In4_D10_Pin_Pin GPIO_PIN_11
#define Stepper_In4_D10_Pin_GPIO_Port GPIOA
#define Stepper_In3_D2_Pin_Pin GPIO_PIN_12
#define Stepper_In3_D2_Pin_GPIO_Port GPIOA
#define LED_Green_Brd_D13_Pin GPIO_PIN_3
#define LED_Green_Brd_D13_GPIO_Port GPIOB
#define Stepper_In2_D12_Pin_Pin GPIO_PIN_4
#define Stepper_In2_D12_Pin_GPIO_Port GPIOB
#define Stepper_In1_D11_Pin_Pin GPIO_PIN_5
#define Stepper_In1_D11_Pin_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
