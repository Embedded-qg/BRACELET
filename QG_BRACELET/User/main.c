#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "power.h"
#include "SysTick.h"
#include "usart.h"
#include "gprs.h"
#include <stdio.h>
#include <string.h>


int main(void)
{

	
	// 来到这里的时候，系统的时钟已经被配置成72M。
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
}
