/*
  ******************************************************************************
  * @file    max30102_main.c
  * @author  Li GuoYan
  * @version V1.0
  * @date    2018-8-18
  * @brief   该文件为max30102实现功能的主函数文件，用作max30102功能的实现
  ******************************************************************************
*/

#include "SysTick.h"
#include "max30102.h" 
#include "IIC1.h"
#include "algorithm.h"
#include "max30102_main.h"
#include "stdio.h"

uint32_t aun_ir_buffer[500];    //红外光传感器数据
uint32_t aun_red_buffer[500];   //红光传感器数据

int32_t  n_ir_buffer_length;    //数据长度
int32_t  n_sp02; 				//血氧浓度值
int32_t  n_heart_rate;          //心率值
int8_t   ch_spo2_valid;         //判断血氧浓度值是否有效
int8_t   ch_hr_valid;           //判断心率值是否有效
int8_t   excute_once = 1; 	    //前面一段内容只需要执行一次，多次调用该函数的时候不必重复执行
int8_t   count1 = 0;           //计数变量1，每执行一次函数得出十个血氧心率值
int8_t   count2 = 0;           //计数变量2，用来处理心率值的平均值

int32_t sp02[20];              //血氧饱和度存储数组
int32_t heart_rate[20];        //心率值存储数组

/*
 * 函数名: max30102_main
 * 描述  : 该函数为max30102主函数，主要是获取心率高低和血氧浓度，每调
           用一次该函数，产生aver_num个数据，存储在sp02、heart_rate中，
		   并调用函数处理数据并计算其平均值
 * 输入  : 无
 * 
 * 返回  : 无
 *         
 * 调用  : 被外部调用
 */
void max30102_main(void)
{ 	
  u8 temp[6];
  int i;                //循环计数变量
	
   if(excute_once)                //该判断语句下只执行一次
   {
		excute_once = 0;
		max30102_init(); 			 //MAX30102初始化
		printf("max30102 is initlized!");
	
		n_ir_buffer_length=500;    //数据长度设为500
	
	   /*读出前500个样本数据, 然后得出传感器数据的范围*/
       for(i=0;i<n_ir_buffer_length;i++)
        {
           while(MAX30102_INT==1);   //等待MAX30102中断的开启
        
		   max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		   aun_red_buffer[i] =  (long)((long)((long)temp[0] & 0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];   //利用相关数据求值 
		   aun_ir_buffer[i]  =  (long)((long)((long)temp[3] & 0x03)<<16) | (long)temp[4]<<8 | (long)temp[5];   //利用相关数据求值 
        }
	
	   /*取500个样本数据之后，开始计算心率值和血氧浓度值*/
       maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
    }
   
	while(count1 != aver_num)
	{		
		/*丢弃前一百个样本数据，把后四百个数据左移*/
        for(i=100;i<500;i++)
         {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
         }
		   
        for(i=400;i<500;i++)
         {   
			/*再求需要的一百个数据*/
            while(MAX30102_INT==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0] & 0x03)<<16) | (long)temp[1]<<8  | (long)temp[2];    //利用相关数据求值 
			aun_ir_buffer[i]  =  (long)((long)((long)temp[3] & 0x03)<<16) | (long)temp[4]<<8  | (long)temp[5];    //利用相关数据求值 	
		 }   
		   /*进行数据处理以及赋值*/
			if(n_heart_rate > 100 && n_heart_rate < 200)
		       heart_rate[count1] = n_heart_rate - 50;
			else
			   heart_rate[count1] = n_heart_rate;
		       
			  sp02[count1++] = n_sp02;       
		   
//		    /*在串口打印输出*/
//            printf("\r\nHR=%i, ", heart_rate[count1 - 1]); 
//			printf("HRvalid=%i, ", ch_hr_valid);
// 			printf("SpO2=%i, ", n_sp02);
//			printf("SPO2Valid=%i\r\n", ch_spo2_valid);
		
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
	}
	    count1 = 0;     //重新计数
	    printf("\r\n\r\n您这段时间的平均心率为%d\r\n\r\n",get_aver_hr(heart_rate));
	    printf("\r\n\r\n您这段时间的平均血氧浓度为%.2f\r\n\r\n",get_aver_spO2(sp02));
 }

 /*
 * 函数名: get_aver_hr
 * 描述  : 该函数对aver_num个心率数据进行数据处理和求其平均值，
 *
 * 输入  : aver_num个心率数据
 * 
 * 返回  : aver_num个心率数据的平均值
 *         
 * 调用  : 被外部调用，调用后可得该段时间的心率值，调用前请务必先调用max30102_main函数
 */
int32_t get_aver_hr(int32_t Heart_Rate[])
{
	 char i;                                      //循环计数变量
	 int  num = aver_num;                         //统计有多少个不符合的数据
	 static int32_t processed_aver[2] = {70,70};  //经过处理后的平均值
	 int32_t result = 0;                          //所求的平均值

	 for(i = 0;i < aver_num;i++)                    //对aver_num个数据逐个处理
	 {
		 if((Heart_Rate[i]>0) && (Heart_Rate[i] < 200))    //把不在范围的数据剔除
		 {			 
		     if((i!= 0) && ((Heart_Rate[i] - Heart_Rate[i-1]) > 30) && Heart_Rate[i-1] > 0)          //测出来的数据相差太大时作相应的处理
				 Heart_Rate[i] = ((Heart_Rate[i] - Heart_Rate[i-1])/2) + Heart_Rate[i-1];
			 if((i!= 0) && ((Heart_Rate[i-1] - Heart_Rate[i]) > 30) && Heart_Rate[i-1] < 200)
				 Heart_Rate[i] = ((Heart_Rate[i-1] - Heart_Rate[i])/2) + Heart_Rate[i];
			 
			 result += Heart_Rate[i];              //符合要求的数据进行叠加
		 }
		 else
		 {
			 num --;
			 if(num <= 6)  {num = aver_num;return -999;}	   //若4个或以上数据不在要求的范围内，则判断为数据无效，返回-999
		 }
	 }
	 
	 result = (int32_t)result/num;                            //求平均值
	 processed_aver[count2] = result;                         //把相邻两次的平均值存储起来
	 if(count2 < 1)  {count2++;processed_aver[1] = processed_aver[0];}
	 
	 if(count2 == 1 && processed_aver[0] != -999)               //相邻两次平均值相差太大则作相应处理
	 {
		 if((processed_aver[1] - processed_aver[0]) > 30) 
			 result = (int32_t)((processed_aver[1] - processed_aver[0])/2) + processed_aver[0];
		 if((processed_aver[0] - processed_aver[1]) > 30)
		     result = (int32_t)((processed_aver[0] - processed_aver[1])/2) + processed_aver[1];
	 }
	 if(result > 100) result = result - 20;
	 
	 return (result - 15);            //为了使结果更加准确，对平均值作相应处理
}

 /*
 * 函数名: get_aver_spO2
 * 描述  : 该函数对aver_num个血氧浓度数据进行数据处理和求其平均值
 *
 * 输入  : aver_num个血氧浓度数据
 * 
 * 返回  : aver_num个血氧浓度数据的平均值
 *         
 * 调用  : 被外部调用，调用后可得该段时间血氧浓度，调用前请务必先调用max30102_main函数
 */
float get_aver_spO2(int32_t SP02[])
{
	 char i;                              //循环计数变量
	 int  num = aver_num;                 //统计有多少个不符合的数据
	 float result = 0;                    //所求的平均值

	 for(i = 0;i < aver_num;i++)          //对aver_num个数据逐个处理
	 { 
		 if((SP02[i] >= 90) && (SP02[i] <= 100))                //把不在范围的数据剔除
		 {			 
		     if((i!= 0) && (SP02[i-1] >= 90) && ((SP02[i] - SP02[i-1]) > 5))       //测出来的数据相差太大时作相应的处理
				 SP02[i] = ((SP02[i] - SP02[i-1])/2) + SP02[i-1];
			 if((i!= 0) && (SP02[i-1] >= 90) && ((SP02[i-1] - SP02[i]) > 5))
			     SP02[i] = ((SP02[i-1] - SP02[i])/2) + SP02[i];
			 result += (float)SP02[i]; 
		 }
		 else
		 {
			 num --;
			 if(num < 6)  {num = aver_num;return -999;}	           //若4个或以上数据不在要求的范围内，则判断为数据无效，返回-999
		 }
	 }
	 
	 result = result/num;            //求平均值
	
	 return result;                 //返回平均值
}
