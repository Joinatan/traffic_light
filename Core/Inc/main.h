/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
extern uint8_t LEDS[3];
extern SPI_HandleTypeDef hspi3;


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* #define RUN_TEST_PROGRAM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define CAR1_Pin GPIO_PIN_4
#define CAR1_GPIO_Port GPIOC
#define CAR1_EXTI_IRQn EXTI4_IRQn
#define SHIFT_REG_STORE_CLK_Pin GPIO_PIN_12
#define SHIFT_REG_STORE_CLK_GPIO_Port GPIOB
#define CAR2_Pin GPIO_PIN_13
#define CAR2_GPIO_Port GPIOB
#define CAR2_EXTI_IRQn EXTI15_10_IRQn
#define CAR3_Pin GPIO_PIN_14
#define CAR3_GPIO_Port GPIOB
#define CAR3_EXTI_IRQn EXTI15_10_IRQn
#define SHIFT_REG_ENABLE_Pin GPIO_PIN_7
#define SHIFT_REG_ENABLE_GPIO_Port GPIOC
#define SHIFT_REG_RESET_Pin GPIO_PIN_9
#define SHIFT_REG_RESET_GPIO_Port GPIOA
#define CAR4_Pin GPIO_PIN_10
#define CAR4_GPIO_Port GPIOA
#define CAR4_EXTI_IRQn EXTI15_10_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define PEDESTRIAN1_Pin GPIO_PIN_15
#define PEDESTRIAN1_GPIO_Port GPIOA
#define PEDESTRIAN1_EXTI_IRQn EXTI15_10_IRQn
#define PEDESTRIAN2_Pin GPIO_PIN_7
#define PEDESTRIAN2_GPIO_Port GPIOB
#define PEDESTRIAN2_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
