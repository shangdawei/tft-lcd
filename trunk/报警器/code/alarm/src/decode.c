/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: decode.c
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/14  || Create this file
**  ******************************************************************************************
*/

#define	_DEC_C_
//#include <ina90.h> 
#include "def.h"
#include "main.h"

#define	BUFANG_CMD		0x01	//布防
#define	CHEFANG_CMD		0x02	//撤防
#define	ITC_CMD			0x03	//紧急报警
#define	PART_CF_CMD		0x04	//部分布撤防区撤防

/*------------------------------------------*/
#define	REMOTE_ADDR		0xAAAA

uint8   RecData[24]={0};//原始数据
uint8   Data[12]={0};//解码后存放的数据，前8字节为地址码，后4字节为数据码
                        //0，1，2分别表示0码，1码，悬空码
bool      count=0;//捕获中断次数奇偶判断,0为偶数,布尔数据类型

uint8   CodeCount=0;//接收原始码计数（24个字节）
uint8   StartCdoe=0;//间隔码标志位
uint8   CNT=0;//捕获值缓冲单元（取高8位）
uint8   flag=0;//接收原始码完成标志位

void decode_init(void)
{
  __disable_interrupt();
  PORTB |= (1 << PB0); 
  DDRB &= ~(1 << DDB0);//捕获端口设置为输入
  TIMSK1|=0x20; //输入捕获使能
  TCCR1B=0x42;   //上升沿捕获，时钟8分频1us计数一次,最大计数65ms 
  __enable_interrupt();
}
//**************************************
//定时器1输入捕获中断，上升沿触发
//
//
//*************************************
#pragma vector = TIMER1_CAPT_vect
__interrupt void timer1_capt_isr(void)
{
  uint8   CNT1=0;
  CNT1=ICR1L; 
  CNT=ICR1H; 
  count=!count;//奇偶标志互换
  if(count)//奇数次测低电平
  {
    TCCR1B&=~(1<<ICES1);//奇数次中断后置为下降沿触发，用于测高电平时间
    CNT1=CNT;
    CNT1=CNT1>>4;//只判断高8位的高4位是否大于0x03 
    if(CNT1>0x03)
    {
      StartCdoe=1;//间隔码标志置1
    }
  }
  else//偶数次测高电平时间
  {
    TCCR1B|=(1<<ICES1);//偶数次中断后置为上升沿触发，用于测高电平时间
    if(StartCdoe) //遇到间隔码则读取后面的24个码，取高8位的计算值
    {
      RecData[CodeCount++]=CNT;//读取地址码和数据码
      if(CodeCount==24)
      {
        CodeCount=0;//源码个数计数清零
        StartCdoe=0;//接收完原始码后，间隔码标志清零
        flag=1;//完成标志
      }
    }
  }
  TCNT1H=0;//定时器清零 
  TCNT1L=0;
}
//*********************************
//函数名称：void DecodeNew(void)
//函数功能：解码
//传递参数：无
//返回值：  无
//*********************************
void DecodeNew(void)
{
  uint8 DecodeCom=12,i=0,j=0;
  while(DecodeCom)
  {
    if((RecData[i]<3)&&(RecData[i+1]>3))//为悬空码
    {
      Data[j++]=2;
      i+=2;
    }
    else
    {
      if((RecData[i]<3)&&(RecData[i+1]<3))//都小于则为0码
      {
        Data[j++]=0;
        i+=2;
      }
      else//否则为1码
      {
        Data[j++]=1;
        i+=2;
      }
    }
    DecodeCom--;
  }
}
/*  $Function   :   do_remote_cmd
==  ==============================================================================================
==  Description :   处理遥控器命令
==  ==============================================================================================
==  Argument    :   
==  ==============================================================================================
==  Return      :   
==              :   
==  ===============================================================================================
==  History     : Modify by  ||    ID    ||     Date      ||     Contents
==              :   xul      ||          ||   2009/09/26  || Create this function
==  ===============================================================================================
*/
void do_remote_cmd(uint8 val)
{
	switch(val)
	{
		case BUFANG_CMD://布防命令
			gWorkMode.alarm_md = ALARM_ON;
			break;
		case CHEFANG_CMD://撤防命令
			gWorkMode.alarm_md = ALARM_OFF;
			break;
		case ITC_CMD://紧急报警

			break;
		case PART_CF_CMD://部分布撤防区撤防

			break;
		default:
			break;
	}
}
/*  $Function   :   decode_process
==  ==============================================================================================
==  Description :   解码进程
==  ==============================================================================================
==  Argument    :   
==  ==============================================================================================
==  Return      :   
==              :   
==  ===============================================================================================
==  History     : Modify by  ||    ID    ||     Date      ||     Contents
==              :   xul      ||          ||   2009/09/26  || Create this function
==  ===============================================================================================
*/
void decode_process(void)
{
	uint16 data_addr = 0;
	uint8 data_buf = 0;
	uint8 i;
	if(flag == 1)
	{
		flag = 0;
		DecodeNew();//解码
		for(i = 0; i < 8; i++)
			data_addr |= Data[i] << (14 - i*2);
		for(i = 8; i < 12; i++)
			data_buf |= Data[i] << (11 - i);
		
		if(data_addr == REMOTE_ADDR)//遥控器地址，8位全部悬空
		{
			do_remote_cmd(data_buf);
		}
	}
}

/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
