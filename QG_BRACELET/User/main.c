#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "power.h"
#include "SysTick.h"
#include "usart.h"
#include "gprs.h"
#include <stdio.h>
#include <string.h>

extern char error_result[20];
extern char Uart2_Buf[Buf2_Max];
extern uint32_t First_Int;


int main(void)
{
	uint8_t i;
	
	// 来到这里的时候，系统的时钟已经被配置成72M。
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	USART2_Init_Config(115200);  //串口2初始化
	USART1_Init_Config(115200);
	//等待系统稳定
	for(i = 0;i < STABLE_TIMES;i++)
	{
		delay_ms(50);
	}
	
	while(1)
	{
		Send_data_and_error_deal(Server_IP_port ,"12346789798879848464684646874684356484645646845645456468468464684");
		for(i=0;i<30;i++)
		{
			delay_ms(200);
		}
	}
	
}
