#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "power.h"
#include "SysTick.h"
#include "usart.h"
#include "gprs.h"
#include "gps.h"
#include <stdio.h>
#include <string.h>

extern GPS_Data_Typedef GPS_Data;
extern double latitude;   //纬度
extern double longitude;	//经度

int main(void)
{
	uint8_t i;
	// 来到这里的时候，系统的时钟已经被配置成72M。
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	//打印调试GPRS时用115200的波特率，接收GPS时，用9600
	USART1_Init_Config(115200);
	//初始化串口2
	USART2_Init_Config(115200);
	
	UART1_SendString("系统启动.......................\r\n");
	//延时一下，等待系统稳定
	for(i=0;i<20;i++)
		delay_ms(100);
	while(1)
	{
		char data_to_serve[200];
		//parseGpsBuffer();
		//printf("%lf,%lf\r\n",longitude,latitude);
		//转换数据
		sprintf(data_to_serve,"lo:%lf,la:%lf",longitude,latitude);
		//发送数据给后台
		Send_data_and_error_deal(Serve_IP_TCP_Port,data_to_serve);
		for(i=0;i<20;i++)
			delay_ms(100);
	}
}
