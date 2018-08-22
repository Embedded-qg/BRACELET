/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart3.h"
#include "gprs.h"
#include "gps.h"
#include <stdio.h>
#include <string.h>

#define USART1_REC_LEN 200

	
extern char Uart2_Buf[100];
extern uint32_t First_Int;
extern GPS_Data_Typedef GPS_Data;

char USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART_RX_STA=0;       //����״̬���	
u16 point1 = 0;


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
/*******************************************************************************
* ������  : USART1_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void USART1_IRQHandler(void)     
{
	uint8_t Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if(Res == '$')
		{
			point1 = 0;	
		}
		USART1_RX_BUF[point1++] = Res;
		//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
		if(USART1_RX_BUF[0] == '$' && USART1_RX_BUF[4] == 'M' && USART1_RX_BUF[5] == 'C')		
		{
			if(Res == '\n')		
			{
				memset(GPS_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
				memcpy(GPS_Data.GPS_Buffer, USART1_RX_BUF, point1); 	//��������
				GPS_Data.isGetData = true;
				point1 = 0;
				memset(USART1_RX_BUF, 0, USART1_REC_LEN);      //���		
			}
			if(point1 >= USART1_REC_LEN)
			{
				point1 = USART1_REC_LEN;
			}	
		}
	}
}	
/*******************************************************************************
* ������  : USART2_IRQHandler
* ����    : ����2�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
	u8 Res=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{		
		
		Res = USART_ReceiveData(USART2);
		//printf("���յ��ظ�:%c\r\n",Res);
		printf("%c",Res);
		Uart2_Buf[First_Int] = Res;  	  //�����յ����ַ����浽������
		First_Int++;                	  //����ָ������ƶ�
		if(First_Int >= Buf2_Max)       	  //���������,������ָ��ָ�򻺴���׵�ַ
		{
			First_Int = 0;
		}  
	}
	
	/*Res = USART_ReceiveData(USART2);
	printf("%c",Res);
	Uart2_Buf[First_Int] = Res;  	  //�����յ����ַ����浽������
	First_Int++;                	  //����ָ������ƶ�
	if(First_Int > Buf2_Max)       	  //���������,������ָ��ָ�򻺴���׵�ַ
	{
		First_Int = 0;
	}*/
} 	


// ����3(MCU6050����ģ��)�жϷ�����
void DEBUG_USART_IRQHandler(void) 
{
  uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
    USART_SendData(DEBUG_USARTx,ucTemp);    
	}	 
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
