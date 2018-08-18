#include "gprs.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"

/*************  ���ر�������	**************/
char Uart2_Buf[Buf2_Max];//����2���ջ���
uint32_t First_Int = 0;
char error_result[20];
char server_ip_port_cmd[45];

/*************  �ⲿ�������� ****************/
extern  void UART2_Send_Command(char* s);  
extern void delay_ms(u16 nms);
extern u8 UART2_Send_AT_Command_End(char *b,char *a,u8 wait_time,u32 interval_time);
extern void UART2_SendString(char* s);
extern u8 UART2_Send_AT_Command(char *b,char *a,u8 wait_time,u32 interval_time);

/*************	���غ�������	**************/
void CLR_Buf(void);     //�������2���ջ���
u8 Wait_CREG(u8 query_times);    //�ȴ�ģ��ע��ɹ�
u8 Find(char *a);

//���gprs������״̬
int check_status(void)
{
	int ret;
	char esc_char[2];
	
	esc_char[0] = 0x1B;//�˳��ַ�
	esc_char[1] = '\0';
	
	ret = UART2_Send_AT_Command("AT","OK",6,50);//����ͨ���Ƿ�ɹ�
		
	if(ret == 0)
	{
		UART2_SendString(esc_char);//��һ����>״̬����ô�÷���һ��ESC�ַ�
		return COMMUNITE_ERROR;
	}
	
	ret = UART2_Send_AT_Command("AT+CPIN?","READY",4,50);//��ѯ���Ƿ����

	if(ret == 0)
	{
		return NO_SIM_CARD_ERROR;
	}
	
	ret = Wait_CREG(3);//��ѯ���Ƿ�ע�ᵽ����
	if(ret == 0)
	{
		return SIM_CARD_NO_REG_ERROR;
	}
    
  ret = UART2_Send_AT_Command("ATE0","OK",2,50);//�رջ��Թ���
	if(ret == 0)
	{
		return EAT0_ERROR;
	}
    
	return 1;
}
//������Ϣ��������
// server_IP_and_port�Ƿ�������IP��ַ���˿�
//content��Ҫ���͸�������������
int send_data_to_server(char *server_IP_and_port,char *content)
{
	u8 ret;
	char end_char[2];
	
	
	end_char[0] = 0x1A;//�����ַ�
	end_char[1] = '\0';
	
	ret = UART2_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,50*2);//��ѯ����״̬
	if(ret == 1)//˵����������������״̬
	{
		ret = UART2_Send_AT_Command("AT+CIPSEND",">",3,70);//������������
		if(ret == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		UART2_SendString(content);
		delay_ms(10);
		ret = UART2_Send_AT_Command_End(end_char,"SEND OK",3,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
		if(ret == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;
	}
	
	ret = UART2_Send_AT_Command("AT+CGATT=1","OK",3,50*7);//��������
	if(ret == 0)
	{
		return AT_CGATT_ERROR;
	}
	
	ret = UART2_Send_AT_Command("AT+CGACT=1,1","OK",3,50*2);//��������
	if(ret == 0)
	{
		return AT_CGATT1_ERROR;
	}

	memset(server_ip_port_cmd,'\0',45);
	strcpy(server_ip_port_cmd,"AT+CIPSTART=TCP,");
	strcat(server_ip_port_cmd,server_IP_and_port);
	
	ret = UART2_Send_AT_Command(server_ip_port_cmd,"CONNECT OK",6,50*5);//���ӷ�����
	if(ret == 0)
	{
		return AT_CIPSTART_ERROR;
	}
	
	ret = UART2_Send_AT_Command("AT+CIPSEND",">",3,70);//������������
	if(ret == 0)
	{
		return AT_CIPSEND_ERROR;
	}
	
	UART2_SendString(content);
	
	ret = UART2_Send_AT_Command_End(end_char,"SEND OK",2,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
    
  if(ret == 0)
	{
		return END_CHAR_ERROR;
	}
	
	return 1;
}

/*******************************************************************************
* ������ : CLR_Buf
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //��������������
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
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
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
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
* ������ : Send_data_and_error_deal
* ����   :�������ݸ�������,��������Ӧ�ĳ�����
* ����   : 
* ���   : 
* ����   : �������
* ע��   : ������
*******************************************************************************/
void Send_data_and_error_deal(char *TCP_IP_port,char *Data)
{
	int  ret;			//�������״̬
	memset(error_result,'\0',20);
	ret = check_status();
	if(ret == 1)
	{
			ret = send_data_to_server(TCP_IP_port,Data);//�������ݵ�������					
	}
	if(ret == 1)
	{
			sprintf(error_result,"�ɹ����͵�server\r\n");
			UART1_SendString(error_result);
			delay_ms(50);
			UART1_SendString("�յ��ظ���\r\n");  //received:��������ݲ��ǻظ�����������
			UART1_SendString(Uart2_Buf);
			UART1_SendString("\r\n");
			
	}
	else //���ݷ���ʧ�ܣ���ʱ��������ʱ��������µ�ʧ�ܣ������ͶϿ����ӣ�Ȼ��Ϳ��Լ�����������
	{
		sprintf(error_result,"������� : %d\r\n",ret);
		printf("������� : %d\r\n",ret);
		UART1_SendString(error_result);
		UART2_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//�ر�����
	
	}
}

