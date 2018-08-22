#include "gps.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
GPS_Data_Typedef GPS_Data;
double latitude=0,longitude=0;



//���յ���GPS���ݻ��ɰٶȵ�ͼ�ȸ��ͼ�����ݸ�ʽ
void gps_to_map(char * latitude_word,char *longitude_word)
{
	double temp=0;
	
	//ת��γ�ȵ�����
	latitude=(latitude_word[0]-48)*10+(latitude_word[1]-48);
	//��һ���ַ���ת���������ַ����ɸ�����
	temp=atof(latitude_word+2)/(double)60;
//	printf("latitude=%lf,temp=%lf\r\n",latitude,temp);
	latitude+=temp;
	printf("����γ��=%lf\r\n",latitude);

	//���յ�ͼ��ʽת����ʽ����ת��
	longitude=(longitude_word[0]-48)*100+(longitude_word[1]-48)*10+longitude_word[2]-48;
	temp=atof(longitude_word+3)/(double)60;
//	printf("longitude=%lf,temp=%lf\r\n",longitude,temp);
	longitude+=temp;
	printf("���վ���=%lf\r\n",longitude);
	
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
				//���һ����ַ��Σ��ö���������һ����������֡
				if ((subString = strstr(GPS_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						//��ȡ�Ƿ�Ϊ��Ч���ݵ��ж��ַ���
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;
						case 3:memcpy(GPS_latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:	memcpy(GPS_N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(GPS_longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(GPS_E_W, subString, subStringNext - subString);break;	//��ȡE/W

						default:break;
					}
					
					subString = subStringNext;
					
					//�ж������Ƿ���Ч
					if(usefullBuffer[0] == 'A')
						GPS_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						GPS_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//��������
				}
			}
			
		}
		printf("GPS����\r\n");
		printf("γ��%s,����%s\r\n",GPS_latitude,GPS_longitude);
		//���յ�׼ȷ�����ݣ���GPS����ת���ɾ�γ��
		if(true==GPS_Data.isUsefull)
		{
			printf("GPS_latitude=%s,GPS_longitude=%s\r\n",GPS_latitude,GPS_longitude);
			gps_to_map(GPS_latitude,GPS_longitude);
		}
		else
				latitude=0,longitude=0;
	}
}










