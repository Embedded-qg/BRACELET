/*
  ******************************************************************************
  * @file    max30102_main.c
  * @author  Li GuoYan
  * @version V1.0
  * @date    2018-8-18
  * @brief   ���ļ�Ϊmax30102ʵ�ֹ��ܵ��������ļ�������max30102���ܵ�ʵ��
  ******************************************************************************
*/

#include "SysTick.h"
#include "max30102.h" 
#include "IIC1.h"
#include "algorithm.h"
#include "max30102_main.h"
#include "stdio.h"

uint32_t aun_ir_buffer[500];    //����⴫��������
uint32_t aun_red_buffer[500];   //��⴫��������

int32_t  n_ir_buffer_length;    //���ݳ���
int32_t  n_sp02; 				//Ѫ��Ũ��ֵ
int32_t  n_heart_rate;          //����ֵ
int8_t   ch_spo2_valid;         //�ж�Ѫ��Ũ��ֵ�Ƿ���Ч
int8_t   ch_hr_valid;           //�ж�����ֵ�Ƿ���Ч
int8_t   excute_once = 1; 	    //ǰ��һ������ֻ��Ҫִ��һ�Σ���ε��øú�����ʱ�򲻱��ظ�ִ��
int8_t   count1 = 0;           //��������1��ÿִ��һ�κ����ó�ʮ��Ѫ������ֵ
int8_t   count2 = 0;           //��������2��������������ֵ��ƽ��ֵ

int32_t sp02[20];              //Ѫ�����Ͷȴ洢����
int32_t heart_rate[20];        //����ֵ�洢����

/*
 * ������: max30102_main
 * ����  : �ú���Ϊmax30102����������Ҫ�ǻ�ȡ���ʸߵͺ�Ѫ��Ũ�ȣ�ÿ��
           ��һ�θú���������aver_num�����ݣ��洢��sp02��heart_rate�У�
		   �����ú����������ݲ�������ƽ��ֵ
 * ����  : ��
 * 
 * ����  : ��
 *         
 * ����  : ���ⲿ����
 */
void max30102_main(void)
{ 	
  u8 temp[6];
  int i;                //ѭ����������
	
   if(excute_once)                //���ж������ִֻ��һ��
   {
		excute_once = 0;
		max30102_init(); 			 //MAX30102��ʼ��
		printf("max30102 is initlized!");
	
		n_ir_buffer_length=500;    //���ݳ�����Ϊ500
	
	   /*����ǰ500����������, Ȼ��ó����������ݵķ�Χ*/
       for(i=0;i<n_ir_buffer_length;i++)
        {
           while(MAX30102_INT==1);   //�ȴ�MAX30102�жϵĿ���
        
		   max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		   aun_red_buffer[i] =  (long)((long)((long)temp[0] & 0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];   //�������������ֵ 
		   aun_ir_buffer[i]  =  (long)((long)((long)temp[3] & 0x03)<<16) | (long)temp[4]<<8 | (long)temp[5];   //�������������ֵ 
        }
	
	   /*ȡ500����������֮�󣬿�ʼ��������ֵ��Ѫ��Ũ��ֵ*/
       maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
    }
   
	while(count1 != aver_num)
	{		
		/*����ǰһ�ٸ��������ݣ��Ѻ��İٸ���������*/
        for(i=100;i<500;i++)
         {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
         }
		   
        for(i=400;i<500;i++)
         {   
			/*������Ҫ��һ�ٸ�����*/
            while(MAX30102_INT==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0] & 0x03)<<16) | (long)temp[1]<<8  | (long)temp[2];    //�������������ֵ 
			aun_ir_buffer[i]  =  (long)((long)((long)temp[3] & 0x03)<<16) | (long)temp[4]<<8  | (long)temp[5];    //�������������ֵ 	
		 }   
		   /*�������ݴ����Լ���ֵ*/
			if(n_heart_rate > 100 && n_heart_rate < 200)
		       heart_rate[count1] = n_heart_rate - 50;
			else
			   heart_rate[count1] = n_heart_rate;
		       
			  sp02[count1++] = n_sp02;       
		   
//		    /*�ڴ��ڴ�ӡ���*/
//            printf("\r\nHR=%i, ", heart_rate[count1 - 1]); 
//			printf("HRvalid=%i, ", ch_hr_valid);
// 			printf("SpO2=%i, ", n_sp02);
//			printf("SPO2Valid=%i\r\n", ch_spo2_valid);
		
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
	}
	    count1 = 0;     //���¼���
	    printf("\r\n\r\n�����ʱ���ƽ������Ϊ%d\r\n\r\n",get_aver_hr(heart_rate));
	    printf("\r\n\r\n�����ʱ���ƽ��Ѫ��Ũ��Ϊ%.2f\r\n\r\n",get_aver_spO2(sp02));
 }

 /*
 * ������: get_aver_hr
 * ����  : �ú�����aver_num���������ݽ������ݴ��������ƽ��ֵ��
 *
 * ����  : aver_num����������
 * 
 * ����  : aver_num���������ݵ�ƽ��ֵ
 *         
 * ����  : ���ⲿ���ã����ú�ɵøö�ʱ�������ֵ������ǰ������ȵ���max30102_main����
 */
int32_t get_aver_hr(int32_t Heart_Rate[])
{
	 char i;                                      //ѭ����������
	 int  num = aver_num;                         //ͳ���ж��ٸ������ϵ�����
	 static int32_t processed_aver[2] = {70,70};  //����������ƽ��ֵ
	 int32_t result = 0;                          //�����ƽ��ֵ

	 for(i = 0;i < aver_num;i++)                    //��aver_num�������������
	 {
		 if((Heart_Rate[i]>0) && (Heart_Rate[i] < 200))    //�Ѳ��ڷ�Χ�������޳�
		 {			 
		     if((i!= 0) && ((Heart_Rate[i] - Heart_Rate[i-1]) > 30) && Heart_Rate[i-1] > 0)          //��������������̫��ʱ����Ӧ�Ĵ���
				 Heart_Rate[i] = ((Heart_Rate[i] - Heart_Rate[i-1])/2) + Heart_Rate[i-1];
			 if((i!= 0) && ((Heart_Rate[i-1] - Heart_Rate[i]) > 30) && Heart_Rate[i-1] < 200)
				 Heart_Rate[i] = ((Heart_Rate[i-1] - Heart_Rate[i])/2) + Heart_Rate[i];
			 
			 result += Heart_Rate[i];              //����Ҫ������ݽ��е���
		 }
		 else
		 {
			 num --;
			 if(num <= 6)  {num = aver_num;return -999;}	   //��4�����������ݲ���Ҫ��ķ�Χ�ڣ����ж�Ϊ������Ч������-999
		 }
	 }
	 
	 result = (int32_t)result/num;                            //��ƽ��ֵ
	 processed_aver[count2] = result;                         //���������ε�ƽ��ֵ�洢����
	 if(count2 < 1)  {count2++;processed_aver[1] = processed_aver[0];}
	 
	 if(count2 == 1 && processed_aver[0] != -999)               //��������ƽ��ֵ���̫��������Ӧ����
	 {
		 if((processed_aver[1] - processed_aver[0]) > 30) 
			 result = (int32_t)((processed_aver[1] - processed_aver[0])/2) + processed_aver[0];
		 if((processed_aver[0] - processed_aver[1]) > 30)
		     result = (int32_t)((processed_aver[0] - processed_aver[1])/2) + processed_aver[1];
	 }
	 if(result > 100) result = result - 20;
	 
	 return (result - 15);            //Ϊ��ʹ�������׼ȷ����ƽ��ֵ����Ӧ����
}

 /*
 * ������: get_aver_spO2
 * ����  : �ú�����aver_num��Ѫ��Ũ�����ݽ������ݴ��������ƽ��ֵ
 *
 * ����  : aver_num��Ѫ��Ũ������
 * 
 * ����  : aver_num��Ѫ��Ũ�����ݵ�ƽ��ֵ
 *         
 * ����  : ���ⲿ���ã����ú�ɵøö�ʱ��Ѫ��Ũ�ȣ�����ǰ������ȵ���max30102_main����
 */
float get_aver_spO2(int32_t SP02[])
{
	 char i;                              //ѭ����������
	 int  num = aver_num;                 //ͳ���ж��ٸ������ϵ�����
	 float result = 0;                    //�����ƽ��ֵ

	 for(i = 0;i < aver_num;i++)          //��aver_num�������������
	 { 
		 if((SP02[i] >= 90) && (SP02[i] <= 100))                //�Ѳ��ڷ�Χ�������޳�
		 {			 
		     if((i!= 0) && (SP02[i-1] >= 90) && ((SP02[i] - SP02[i-1]) > 5))       //��������������̫��ʱ����Ӧ�Ĵ���
				 SP02[i] = ((SP02[i] - SP02[i-1])/2) + SP02[i-1];
			 if((i!= 0) && (SP02[i-1] >= 90) && ((SP02[i-1] - SP02[i]) > 5))
			     SP02[i] = ((SP02[i-1] - SP02[i])/2) + SP02[i];
			 result += (float)SP02[i]; 
		 }
		 else
		 {
			 num --;
			 if(num < 6)  {num = aver_num;return -999;}	           //��4�����������ݲ���Ҫ��ķ�Χ�ڣ����ж�Ϊ������Ч������-999
		 }
	 }
	 
	 result = result/num;            //��ƽ��ֵ
	
	 return result;                 //����ƽ��ֵ
}
