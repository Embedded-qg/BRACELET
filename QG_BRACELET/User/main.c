#include "stm32f10x.h"   
#include "max30102_main.h" 
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
extern int32_t sp02[20];              //Ѫ�����Ͷȴ洢����
extern int32_t heart_rate[20];        //����ֵ�洢����


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
	
	UART1_SendString("ϵͳ��.......................\r\n");
	//��ʱһ�£��ȴ�ϵͳ�ȶ�
	for(i=0;i<20;i++)
		delay_ms(100);
	while(1)
	{
		char data_to_serve[200];
		max30102_main();                 //max30102������������Ѫ���������ݣ����ȵ��øú�������ȡֵ��
									  //����һ�δ����Ҫ9s���������ݣ��ɼ�С����������    
	 
	 	get_aver_hr(heart_rate);         //�ú�������һ������ֵ��ƽ��ֵ���˴�û�и�ֵ������ʱ�ɸ�����Ҫȡ��
	 	get_aver_spO2(sp02);             //�ú�������һ��Ѫ��Ũ�ȵ�ƽ��ֵ���˴�û�и�ֵ������ʱ�ɸ�����Ҫȡ��
	
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

