#include "power.h"
extern void delay_ms(u16 nms);
//自己写的测试用的
void LED_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置引脚 模式 速度
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;
	//设置工作模式为推挽输出
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
	//设置工作速度
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	//调用初始化函数
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//配置引脚 模式 速度
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	//设置工作模式下拉输入
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPD;
	//调用初始化函数
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//配置引脚 模式 速度
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	//设置工作模式为下拉输入
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	//调用初始化函数
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}
//测试用的按键函数
uint8_t key_scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	//如果按键被按下
	if(GPIO_ReadInputDataBit(GPIOx,  GPIO_Pin)==1)
	{
		delay_ms(1);		//防抖动
		if(GPIO_ReadInputDataBit(GPIOx,  GPIO_Pin)==1)
		{
			while(GPIO_ReadInputDataBit(GPIOx,  GPIO_Pin))
				;			//松手检测
			return 1;
		}
	}
	return 0;
}
//待机模式  拉高PA0才能唤醒
void STANDBY_MODE()
{
	//使能时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
		
		//检测复位来源
		if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET)
		{
				//电亮第二个LED
				GPIO_SetBits(GPIOA,GPIO_Pin_2 );
		}
		else
		{
			//非待机唤醒复位 关闭第2个LED
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		}
	
		delay_ms(100);
		//清除WU状态位
		PWR_ClearFlag (PWR_FLAG_WU);
		
		// 使能WKUP引脚的唤醒功能 ，使能PA0
		PWR_WakeUpPinCmd (ENABLE);
		
		// 进入待机模式 
		PWR_EnterSTANDBYMode();
}
