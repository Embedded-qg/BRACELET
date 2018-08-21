#include "stm32f10x.h"   
#include "max30102_main.h" 
#include "power.h"
#include "SysTick.h"
#include "usart.h"
#include "gprs.h"
#include <stdio.h>
#include <string.h>

extern int32_t sp02[20];              //血氧饱和度存储数组
extern int32_t heart_rate[20];        //心率值存储数组

int main(void)
{
	SysTick_Init(72);        //系统时钟配置为72M
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //中断分组，分两组  
	
	while(1)                          //现在仅是简单示例，以后整合的时候可作修改
	{
	 /*血氧心率模块*/
	 max30102_main();                 //max30102主函数，生成血氧心率数据，请先调用该函数，再取值，
									  //调用一次大概需要9s来计算数据，可减小计算数据量    
	 
	 get_aver_hr(heart_rate);         //该函数返回一个心率值的平均值，此处没有赋值，整合时可根据需要取用
	 get_aver_spO2(sp02);             //该函数返回一个血氧浓度的平均值，此处没有赋值，整合时可根据需要取用
	
	 
	}
}


