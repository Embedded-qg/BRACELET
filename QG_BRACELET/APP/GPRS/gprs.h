#ifndef _GPRS_H
#define _GPRS_H

#include "stm32f10x.h"

#define Buf2_Max 	  					100 //串口2缓存长度
#define STABLE_TIMES  				10  //等待系统上电后的稳定
#define Server_IP_port 		"119.29.223.91,6666"

//错误返回的情况
#define COMMUNITE_ERROR       -1 
#define NO_SIM_CARD_ERROR     -2
#define SIM_CARD_NO_REG_ERROR -3
#define CALL_ERROR			  -4
#define AT_CPMS_ERROR		  -5
#define AT_CMGF_ERROR		  -6
#define AT_CSCS_ERROR		  -7
#define AT_CMGS_ERROR         -8
#define END_CHAR_ERROR		  -9
#define AT_CGATT_ERROR   	  -11
#define AT_CGACT_ERROR	      -12
#define AT_CIPSTART_ERROR	  -13
#define AT_CIPSEND_ERROR	  -14
#define AT_CIPMUX_ERROR       -15
#define AT_CIPQSEND_ERROR     -16
#define AT_CSTT_ERROR		  -17
#define AT_CIICR_ERROR		  -18
#define AT_CGATT1_ERROR       -50
#define EAT0_ERROR            -51

int check_status(void);
int send_data_to_server(char *server_IP_and_port,char *content);
uint8_t Find(char *a);
u8 Wait_CREG(u8 query_times);
void Send_data_and_error_deal(char *TCP_IP_port,char *Data);

#endif

