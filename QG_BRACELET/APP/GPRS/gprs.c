#include "gprs.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"

/*************  本地变量声明	**************/
char Uart2_Buf[Buf2_Max];//串口2接收缓存
uint32_t First_Int = 0;
char error_result[20];
char server_ip_port_cmd[45];

/*************  外部函数声明 ****************/
extern  void UART2_Send_Command(char* s);  
extern void delay_ms(u16 nms);
extern u8 UART2_Send_AT_Command_End(char *b,char *a,u8 wait_time,u32 interval_time);
extern void UART2_SendString(char* s);
extern u8 UART2_Send_AT_Command(char *b,char *a,u8 wait_time,u32 interval_time);

/*************	本地函数声明	**************/
void CLR_Buf(void);     //清除串口2接收缓存
u8 Wait_CREG(u8 query_times);    //等待模块注册成功
u8 Find(char *a);

//检测gprs的连接状态
int check_status(void)
{
	int ret;
	char esc_char[2];
	
	esc_char[0] = 0x1B;//退出字符
	esc_char[1] = '\0';
	
	ret = UART2_Send_AT_Command("AT","OK",6,50);//测试通信是否成功
		
	if(ret == 0)
	{
		UART2_SendString(esc_char);//万一进入>状态，那么久发送一个ESC字符
		return COMMUNITE_ERROR;
	}
	
	ret = UART2_Send_AT_Command("AT+CPIN?","READY",4,50);//查询卡是否插上

	if(ret == 0)
	{
		return NO_SIM_CARD_ERROR;
	}
	
	ret = Wait_CREG(3);//查询卡是否注册到网络
	if(ret == 0)
	{
		return SIM_CARD_NO_REG_ERROR;
	}
    
  ret = UART2_Send_AT_Command("ATE0","OK",2,50);//关闭回显功能
	if(ret == 0)
	{
		return EAT0_ERROR;
	}
    
	return 1;
}
//发送信息给服务器
// server_IP_and_port是服务器的IP地址跟端口
//content是要发送给服务器的内容
int send_data_to_server(char *server_IP_and_port,char *content)
{
	u8 ret;
	char end_char[2];
	
	
	end_char[0] = 0x1A;//结束字符
	end_char[1] = '\0';
	
	ret = UART2_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,50*2);//查询连接状态
	if(ret == 1)//说明服务器处于连接状态
	{
		ret = UART2_Send_AT_Command("AT+CIPSEND",">",3,70);//开发发送数据
		if(ret == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		UART2_SendString(content);
		delay_ms(10);
		ret = UART2_Send_AT_Command_End(end_char,"SEND OK",3,250);//发送结束符，等待返回ok,等待5S发一次，因为发送数据时间可能较长
		if(ret == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;
	}
	
	ret = UART2_Send_AT_Command("AT+CGATT=1","OK",3,50*7);//附着网络
	if(ret == 0)
	{
		return AT_CGATT_ERROR;
	}
	
	ret = UART2_Send_AT_Command("AT+CGACT=1,1","OK",3,50*2);//激活网络
	if(ret == 0)
	{
		return AT_CGATT1_ERROR;
	}

	memset(server_ip_port_cmd,'\0',45);
	strcpy(server_ip_port_cmd,"AT+CIPSTART=TCP,");
	strcat(server_ip_port_cmd,server_IP_and_port);
	
	ret = UART2_Send_AT_Command(server_ip_port_cmd,"CONNECT OK",6,50*5);//连接服务器
	if(ret == 0)
	{
		return AT_CIPSTART_ERROR;
	}
	
	ret = UART2_Send_AT_Command("AT+CIPSEND",">",3,70);//开发发送数据
	if(ret == 0)
	{
		return AT_CIPSEND_ERROR;
	}
	
	UART2_SendString(content);
	
	ret = UART2_Send_AT_Command_End(end_char,"SEND OK",2,250);//发送结束符，等待返回ok,等待5S发一次，因为发送数据时间可能较长
    
  if(ret == 0)
	{
		return END_CHAR_ERROR;
	}
	
	return 1;
}

/*******************************************************************************
* 函数名 : CLR_Buf
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //将缓存内容清零
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}

/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
u8 Wait_CREG(u8 query_times)
{
	u8 i;
	u8 k;
	u8 j;
	i = 0;
	CLR_Buf();
	while(i == 0)        			
	{

		UART2_Send_Command("AT+CREG?");
		delay_ms(100); 
		
		for(k=0;k<Buf2_Max;k++)      			
		{
			if((Uart2_Buf[k] == '+')&&(Uart2_Buf[k+1] == 'C')&&(Uart2_Buf[k+2] == 'R')&&(Uart2_Buf[k+3] == 'E')&&(Uart2_Buf[k+4] == 'G')&&(Uart2_Buf[k+5] == ':'))
			{
					 
				if((Uart2_Buf[k+7] == '1')&&((Uart2_Buf[k+9] == '1')||(Uart2_Buf[k+9] == '5')))
				{
					i = 1;
					return 1;
				}
				
			}
		}
		j++;
		if(j > query_times)
		{
			return 0;
		}
		
	}
	return 0;
}

/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

uint8_t Find(char *a)
{ 

	if(strstr(Uart2_Buf, a)!=NULL)
	{
		return 1;
	}	
	else
	{
		return 0;
	}
		
}

/*******************************************************************************
* 函数名 : Send_data_and_error_deal
* 描述   :发送数据给服务器,并且有相应的出错处理
* 输入   : 
* 输出   : 
* 返回   : 错误符号
* 注意   : 出错处理
*******************************************************************************/
void Send_data_and_error_deal(char *TCP_IP_port,char *Data)
{
	int  ret;			//用来标记状态
	memset(error_result,'\0',20);
	ret = check_status();
	if(ret == 1)
	{
			ret = send_data_to_server(TCP_IP_port,Data);//发送数据到服务器					
	}
	if(ret == 1)
	{
			sprintf(error_result,"成功发送到server\r\n");
			UART1_SendString(error_result);
			delay_ms(50);
			UART1_SendString("收到回复：\r\n");  //received:后面的内容才是回复的真正内容
			UART1_SendString(Uart2_Buf);
			UART1_SendString("\r\n");
			
	}
	else //数据发送失败，此时可能连接时间过长导致的失败，这样就断开连接，然后就可以继续连接上了
	{
		sprintf(error_result,"错误代码 : %d\r\n",ret);
		printf("错误代码 : %d\r\n",ret);
		UART1_SendString(error_result);
		UART2_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//关闭链接
	
	}
}

