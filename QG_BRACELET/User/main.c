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
extern double latitude;   //纬度
extern double longitude;	//经度
extern int32_t sp02[20];              //血氧浓度
extern int32_t heart_rate[20];        //心率


int main(void)
{
	uint8_t i;
	// 配置成72M
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断分2组
	//调试GPRS时用115200的波特率，调试GPS时用9600的波特率
	USART1_Init_Config(115200);
	//初始化串口2
	USART2_Init_Config(115200);
	
	
	//等待系统初始化
	for(i=0;i<20;i++)
		delay_ms(100);
	while(1)
	{
		char data_to_serve[200];
		int heartRate;
		float sp;
		max30102_main();                 //max3010初始化
									 
	 	heartRate=get_aver_hr(heart_rate);         //获取心率
	 	sp=get_aver_spO2(sp02); 		//获取血氧浓度
		
			//获取GPS数据
		//parseGpsBuffer();
		//printf("%lf,%lf\r\n",longitude,latitude);
		//如果没有报警就发这些数据给后台
		//转换字符串
		sprintf(data_to_serve,"lo:%lf,la:%lf,hr:%d,sp:%lf",longitude,latitude,heartRate,sp);
		//发送给后台
		Send_data_and_error_deal(Serve_IP_TCP_Port,data_to_serve);
		//如果报警就发help给后台
		
	}
}

