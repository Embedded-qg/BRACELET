#include "stm32f10x.h"   
#include "max30102_main.h" 
#include "power.h"
#include "SysTick.h"
#include "usart.h"
#include "gprs.h"
#include <stdio.h>
#include <string.h>

extern int32_t sp02[20];              //Ѫ�����Ͷȴ洢����
extern int32_t heart_rate[20];        //����ֵ�洢����

int main(void)
{
	SysTick_Init(72);        //ϵͳʱ������Ϊ72M
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //�жϷ��飬������  
	
	while(1)                          //���ڽ��Ǽ�ʾ�����Ժ����ϵ�ʱ������޸�
	{
	 /*Ѫ������ģ��*/
	 max30102_main();                 //max30102������������Ѫ���������ݣ����ȵ��øú�������ȡֵ��
									  //����һ�δ����Ҫ9s���������ݣ��ɼ�С����������    
	 
	 get_aver_hr(heart_rate);         //�ú�������һ������ֵ��ƽ��ֵ���˴�û�и�ֵ������ʱ�ɸ�����Ҫȡ��
	 get_aver_spO2(sp02);             //�ú�������һ��Ѫ��Ũ�ȵ�ƽ��ֵ���˴�û�и�ֵ������ʱ�ɸ�����Ҫȡ��
	
	 
	}
}


