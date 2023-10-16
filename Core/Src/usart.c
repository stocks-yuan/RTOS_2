/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "cJSON.h"
uint8_t uart1_rx_buf[UART1_Rx_Buffer_Max_Num];
uint8_t uart1_tx_buf[UART1_Tx_Buffer_Max_Num];
uint8_t receive_data[UART1_Rx_Buffer_Max_Num];
uint8_t uart2_tx_buf[UART2_Tx_Buffer_Max_Num];
uint16_t usart1_rx_len;
Motor_data motor[5];
enum _motor
{
	Motor1 = 1,
	Motor2,
	Motor3,
	Motor4,
	Motor5
};

uint8_t cjson_parse_fail_flag;
uint8_t usart1_rx_flag;
uint8_t water_sw;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1,(uint8_t *)uart1_rx_buf,UART1_Rx_Buffer_Max_Num);
  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Channel7;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//����1��DMA����  DMA_HandleTypeDef *hdma  UART_HandleTypeDef *huart,
void USART_TX_DMA_Send(UART_HandleTypeDef *huart,DMA_HandleTypeDef *hdma,uint8_t *buffer, uint16_t length)
{
    //�ȴ���һ�ε����ݷ������
	while(HAL_DMA_GetState(hdma) != HAL_DMA_STATE_READY);
    //while(__HAL_DMA_GET_COUNTER(&hdma_usart1_tx));
	
    //�ر�DMA
    __HAL_DMA_DISABLE(hdma);

    //��ʼ��������
    HAL_UART_Transmit_DMA(huart, buffer, length);
}

//����1��DMA����printf
void usart1_printf(const char *format, ...)
{
	uint32_t length = 0;
	va_list args;
	
	__va_start(args, format);
	
	length = vsnprintf((char*)uart1_tx_buf, sizeof(uart1_tx_buf), (char*)format, args);
	
	USART_TX_DMA_Send(&huart1,&hdma_usart1_tx,uart1_tx_buf, length);
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1) //����Ǵ���1
	{
		// ��F7ϵ���ǿ��Բ�д�ģ�F1����д
		__HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_FLAG_TC4); //���DMA2_Steam7������ɱ�־
		HAL_UART_DMAStop(&huart1);		//��������Ժ�رմ���DMA,ȱ����һ�������
	}
	else if (huart->Instance == USART2) //����Ǵ���1
	{
		// ��F7ϵ���ǿ��Բ�д�ģ�F1����д
		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx, DMA_FLAG_TC4); //���DMA2_Steam7������ɱ�־
		HAL_UART_DMAStop(&huart2);		//��������Ժ�رմ���DMA,ȱ����һ�������
	}
}


void HAL_UART_ReceiveIdle(UART_HandleTypeDef *huart)
{
	//�������˴��ڿ����ж�
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)) 
    {
			uint32_t tmp_flag = 0;
			uint32_t temp;
			if(huart->Instance == USART1)
			{
				/* 1.�����־ */
				__HAL_UART_CLEAR_IDLEFLAG(huart); //������б�־

				/* 2.��ȡDMA */
				HAL_UART_DMAStop(huart); //��ֹͣDMA����ͣ����
				//����Ӧע�����ݽ��ղ�Ҫ���� USART_DMA_RX_BUFFER_MAXIMUM
				usart1_rx_len = UART1_Rx_Buffer_Max_Num - (__HAL_DMA_GET_COUNTER(&hdma_usart1_rx)); //���ո������ڽ��ջ������ܴ�С��ʣ�����
				
				/* 3.�������ݽ����������� */
				memcpy(receive_data, uart1_rx_buf, usart1_rx_len); 
				
				usart1_rx_flag = 1; //��־�Ѿ��ɹ����յ�һ���ȴ�����
				memset(uart1_rx_buf,0,UART1_Rx_Buffer_Max_Num);//����������
				
		
			}
    }
}

//����1�жϷ������
void USART1_IRQHandler(void)
{
	HAL_UART_ReceiveIdle(&huart1);
	HAL_UART_IRQHandler(&huart1); //����HAL���жϴ����ú���
	
	/* 4.���´򿪴���DMA���� */
	while (HAL_UART_Receive_DMA(&huart1,(uint8_t *)uart1_rx_buf, UART1_Rx_Buffer_Max_Num)!=HAL_OK); 
}



int cJSON_msg_parse(char* data)
{
	cJSON* cjson_test = NULL;
	cJSON* cjson_ch = NULL;
	cJSON* cjson_sta = NULL;
	cJSON* cjson_vel = NULL;
	cJSON* cjson_water_sw = NULL;
	 /* ��������JSO���� */
	cjson_test = cJSON_Parse(data);
	if(cjson_test == NULL)
	{
		return 1;
	}
	 /* ���θ���������ȡJSON���ݣ���ֵ�ԣ� */
	cjson_ch = cJSON_GetObjectItem(cjson_test,"ch");
	cjson_sta = cJSON_GetObjectItem(cjson_test,"state");
	cjson_vel = cJSON_GetObjectItem(cjson_test,"vel");	
	cjson_water_sw = cJSON_GetObjectItem(cjson_test,"water_sw");	
	/* ��ȡ���� */
	Motor_data_update(cjson_ch->valueint,cjson_sta->valueint,cjson_vel->valueint,cjson_water_sw->valueint);
	
	//�ͷŶ�
	cJSON_Delete(cjson_test);

	//����������ݷ�f4
	Transmit_msg_to_f4((uint8_t)cjson_ch->valueint,cjson_sta->valueint,(uint8_t)cjson_vel->valueint,cjson_water_sw->valueint);
	return 0;
}


void Motor_data_update(uint8_t _motor_id,uint8_t _motor_state,uint8_t _motor_vel,uint8_t _water_sw)
{
	water_sw = _water_sw;
	switch(_motor_id)
	{
		case Motor1:{motor[0].motorID = _motor_id;motor[0].state = _motor_state;motor[0].vel = _motor_vel;
		for(uint8_t i =1;i<5;i++)
			{
				motor[i].motorID = 0;
				motor[i].state = 0;
				motor[i].vel = 0;
			}			
		}break;
		
		case Motor2:{motor[1].motorID = _motor_id;motor[1].state = _motor_state;motor[1].vel = _motor_vel;
			motor[0].motorID =0;motor[0].state =0;motor[0].vel = 0;
		for(uint8_t i =2;i<5;i++)
			{
				motor[i].motorID = 0;
				motor[i].state = 0;
				motor[i].vel = 0;
			}		
		}break;
		
		case Motor3:{motor[2].motorID = _motor_id;motor[2].state = _motor_state;motor[2].vel = _motor_vel;
			motor[0].motorID =0;motor[0].state =0;motor[0].vel = 0;
			motor[1].motorID =0;motor[1].state =0;motor[1].vel = 0;
			motor[3].motorID =0;motor[3].state =0;motor[3].vel = 0;
			motor[4].motorID =0;motor[4].state =0;motor[4].vel = 0;
		}break;
	
		case Motor4:{motor[3].motorID = _motor_id;motor[3].state = _motor_state;motor[3].vel = _motor_vel;
		for(uint8_t i =0;i<3;i++)
			{
				motor[i].motorID = 0;
				motor[i].state = 0;
				motor[i].vel = 0;
			}	
			motor[4].motorID =0;motor[4].state =0;motor[4].vel = 0;
		
		}break;
			
		case Motor5:{motor[4].motorID = _motor_id;motor[4].state = _motor_state;motor[4].vel = _motor_vel;
		for(uint8_t i =0;i<4;i++)
			{
				motor[i].motorID = 0;
				motor[i].state = 0;
				motor[i].vel = 0;
			}	
		}break;			
		default:break;
	}
}

/*
	֡ͷ��0X5A
	_motor_id�����ID
	_motor_state�����״̬
	_motor_vel������ٶ�
	_water_sw����ˮ����   1����ʼ��ˮ  0��ֹͣ��ˮ
	����֡��0xff 0xff

*/
void Transmit_msg_to_f4(uint8_t _motor_id,uint8_t _motor_state,uint8_t _motor_vel,uint8_t _water_sw)
{
	uart2_tx_buf[0] = 0x5a;
	uart2_tx_buf[1] = _motor_id;
	uart2_tx_buf[2] = _motor_state;
	uart2_tx_buf[3] = _motor_vel;
	uart2_tx_buf[4] = water_sw;
	uart2_tx_buf[5] = 0xff;
	uart2_tx_buf[6] = 0xff;
	USART_TX_DMA_Send(&huart2,&hdma_usart2_tx,uart2_tx_buf, 6);
}

/* USER CODE END 1 */
