#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
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
	
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	USART2_Init_Config(115200);  //����2��ʼ��
	USART1_Init_Config(115200);
	//�ȴ�ϵͳ�ȶ�
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
