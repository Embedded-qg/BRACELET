#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "power.h"
#include "SysTick.h"
#include "usart.h"
#include "gprs.h"
#include "gps.h"
#include <stdio.h>
#include <string.h>

extern GPS_Data_Typedef GPS_Data;
extern double latitude;   //γ��
extern double longitude;	//����

int main(void)
{
	uint8_t i;
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	//��ӡ����GPRSʱ��115200�Ĳ����ʣ�����GPSʱ����9600
	USART1_Init_Config(115200);
	//��ʼ������2
	USART2_Init_Config(115200);
	
	UART1_SendString("ϵͳ����.......................\r\n");
	//��ʱһ�£��ȴ�ϵͳ�ȶ�
	for(i=0;i<20;i++)
		delay_ms(100);
	while(1)
	{
		char data_to_serve[200];
		//parseGpsBuffer();
		//printf("%lf,%lf\r\n",longitude,latitude);
		//ת������
		sprintf(data_to_serve,"lo:%lf,la:%lf",longitude,latitude);
		//�������ݸ���̨
		Send_data_and_error_deal(Serve_IP_TCP_Port,data_to_serve);
		for(i=0;i<20;i++)
			delay_ms(100);
	}
}
