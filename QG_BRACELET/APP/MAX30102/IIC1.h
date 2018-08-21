#ifndef __MAX30102_H
#define __MAX30102_H

#include "stm32f10x.h"

/*位带操作,实现比较方便的GPIO控制功能*/
/**********************************************************************/
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
   
#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

/********************************************************************/
   	   		   
			   
/*IO方向设置*/
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=4;}	
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=7;}

/*IO操作函数*/	 
#define IIC_SCL    PBout(7) //SCL
#define IIC_SDA    PBout(8) //SDA	 
#define READ_SDA   PBin(8)  //输入SDA 

/*IIC所有操作函数的声明*/
u8   IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8   IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
void IIC_Read_One_Byte(u8 daddr,u8 addr,u8* data);
void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength);
void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength);

#endif
