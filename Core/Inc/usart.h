/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define UART1_Rx_Buffer_Max_Num  64
#define UART1_Tx_Buffer_Max_Num  64
#define UART2_Tx_Buffer_Max_Num  64
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
typedef struct
{
	int motorID;
	int state;
	int vel;
	
}Motor_data;


extern uint8_t usart1_rx_flag;
extern uint8_t cjson_parse_fail_flag;
extern uint8_t receive_data[UART1_Rx_Buffer_Max_Num];
extern uint8_t cjson_parse_fail_flag;
extern uint8_t usart1_rx_flag;
void USART_TX_DMA_Send(UART_HandleTypeDef *huart,DMA_HandleTypeDef *hdma,uint8_t *buffer, uint16_t length);
void usart1_printf(const char *format, ...);
void HAL_UART_ReceiveIdle(UART_HandleTypeDef *huart);
int cJSON_msg_parse(char* data);
void Motor_data_update(uint8_t _motor_id,uint8_t _motor_state,uint8_t _motor_vel,uint8_t _water_sw);
void Transmit_msg_to_f4(uint8_t _motor_id,uint8_t _motor_state,uint8_t _motor_vel,uint8_t _water_sw);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

