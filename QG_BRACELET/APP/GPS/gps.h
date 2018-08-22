#ifndef _GPS_H
#define _GPS_H

#include "stm32f10x.h"
//定义数组长度
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

#define false 0
#define true 1


typedef struct GPSData_Typedef
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isUsefull;		//定位信息是否有效
} GPS_Data_Typedef ;


void GPS_Init(void);
void gps_to_map(char * latitude_word,char *longitude_word);
void parseGpsBuffer(void);
void printGpsBuffer(void);


#endif
