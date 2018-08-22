#include "gps.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
GPS_Data_Typedef GPS_Data;
double latitude=0,longitude=0;



//把收到的GPS数据换成百度地图谷歌地图的数据格式
void gps_to_map(char * latitude_word,char *longitude_word)
{
	double temp=0;
	
	//转换纬度的坐标
	latitude=(latitude_word[0]-48)*10+(latitude_word[1]-48);
	//用一个字符串转换函数把字符换成浮点数
	temp=atof(latitude_word+2)/(double)60;
//	printf("latitude=%lf,temp=%lf\r\n",latitude,temp);
	latitude+=temp;
	printf("最终纬度=%lf\r\n",latitude);

	//按照地图格式转换公式进行转换
	longitude=(longitude_word[0]-48)*100+(longitude_word[1]-48)*10+longitude_word[2]-48;
	temp=atof(longitude_word+3)/(double)60;
//	printf("longitude=%lf,temp=%lf\r\n",longitude,temp);
	longitude+=temp;
	printf("最终经度=%lf\r\n",longitude);
	
}

void errorLog(int num)
{
	
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0,GPS_latitude[20],GPS_N_S[2],GPS_longitude[20],GPS_E_W[2];
	if (GPS_Data.isGetData)
	{
		GPS_Data.isGetData = false;
		printf("**************\r\n");
		printf(GPS_Data.GPS_Buffer);
		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				//查找划分字符段，用逗号来划分一段有用数据帧
				if ((subString = strstr(GPS_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						//获取是否为有效数据的判断字符串
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;
						case 3:memcpy(GPS_latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:	memcpy(GPS_N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(GPS_longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(GPS_E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}
					
					subString = subStringNext;
					
					//判断数据是否有效
					if(usefullBuffer[0] == 'A')
						GPS_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						GPS_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}
			
		}
		printf("GPS参数\r\n");
		printf("纬度%s,经度%s\r\n",GPS_latitude,GPS_longitude);
		//接收到准确的数据，把GPS数据转换成经纬度
		if(true==GPS_Data.isUsefull)
		{
			printf("GPS_latitude=%s,GPS_longitude=%s\r\n",GPS_latitude,GPS_longitude);
			gps_to_map(GPS_latitude,GPS_longitude);
		}
		else
				latitude=0,longitude=0;
	}
}










