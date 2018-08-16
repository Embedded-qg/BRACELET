#include "power.h"
extern void delay_ms(u16 nms);
//�Լ�д�Ĳ����õ�
void LED_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//�������� ģʽ �ٶ�
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;
	//���ù���ģʽΪ�������
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
	//���ù����ٶ�
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	//���ó�ʼ������
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//�������� ģʽ �ٶ�
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	//���ù���ģʽ��������
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPD;
	//���ó�ʼ������
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//�������� ģʽ �ٶ�
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	//���ù���ģʽΪ��������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	//���ó�ʼ������
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}
//�����õİ�������
uint8_t key_scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	//�������������
	if(GPIO_ReadInputDataBit(GPIOx,  GPIO_Pin)==1)
	{
		delay_ms(1);		//������
		if(GPIO_ReadInputDataBit(GPIOx,  GPIO_Pin)==1)
		{
			while(GPIO_ReadInputDataBit(GPIOx,  GPIO_Pin))
				;			//���ּ��
			return 1;
		}
	}
	return 0;
}
//����ģʽ  ����PA0���ܻ���
void STANDBY_MODE()
{
	//ʹ��ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
		
		//��⸴λ��Դ
		if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET)
		{
				//�����ڶ���LED
				GPIO_SetBits(GPIOA,GPIO_Pin_2 );
		}
		else
		{
			//�Ǵ������Ѹ�λ �رյ�2��LED
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		}
	
		delay_ms(100);
		//���WU״̬λ
		PWR_ClearFlag (PWR_FLAG_WU);
		
		// ʹ��WKUP���ŵĻ��ѹ��� ��ʹ��PA0
		PWR_WakeUpPinCmd (ENABLE);
		
		// �������ģʽ 
		PWR_EnterSTANDBYMode();
}
