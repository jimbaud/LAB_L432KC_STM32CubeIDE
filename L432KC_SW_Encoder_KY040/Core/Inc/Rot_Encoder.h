/*
 * Rot_Encoder.h
 *
 *  Created on: Jan 13, 2025
 *      Author: Joel
 */

#ifndef INC_ROT_ENCODER_H_
#define INC_ROT_ENCODER_H_

#include "main.h"
#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

extern volatile uint8_t encoder_flag;

/* Déclarations des fonctions */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_ROT_ENCODER_H_ */
