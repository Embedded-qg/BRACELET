#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "power.h"
#include "SysTick.h"
#include "usart.h"
#include "gprs.h"
#include <stdio.h>
#include <string.h>


int main(void)
{

	
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
}
