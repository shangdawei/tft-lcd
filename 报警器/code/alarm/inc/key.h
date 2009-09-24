/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: key.h
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/14  || Create this file
**  ******************************************************************************************
*/

#ifndef _KEY_H_
#define _KEY_H_
#include "def.h"

#ifndef _KEY_C_
#define GOLBLE_KEY  extern
#else
#define GOLBLE_KEY
#endif

#define MAX_PHONE_NUM 6

#define ROW_PIN (1<<PC1 | 1<<PC2 | 1<<PC3 | 1<<PC4 | 1<<PC5)
#define KEY_PIN PORTC

const uint8 keyVal[] = {1,2,3,'F',4,5,6,'G',7,8,9,'H','*',0,'#','I','A','B','C'};

typedef struct {
  uint8 mode;//工作模式，对付不定参数
  uint8 serial;
  uint8 val;
  uint8 phone_num[16];
  uint8 old_passwd;
  uint8 new_passwd;
  uint8 id;
} KER_PAR;

//定义指针函数
typedef struct {
	char	*ParseStr;
	void	(*ParseProc)(KER_PAR* );
} PAR_TAB;


//F1=F=编程
//F2=G=设置
//F3=H=录音
//F4=I=清除
#define REC_LWORD   "H^H"     //录音留言
#define SET_PNUM    "G^*^G"   //设置电话号码
#define BAL_OFF     "F1*0F"   //断线报警功能关闭
#define BAL_ON      "F1*1F"   //断线报警功能开启
#define SERY_OFF    "F2*0F"   //关闭密码保护功能
#define SERY_ON     "F2*1F"   //开启密码保护功能
#define AL_SD_OFF   "F3*0F"   //无声报警
#define AL_SD_ON    "F3*1F"   //有声报警
#define RE_AL_SD_OFF "F4*0F"  //遥控警号伴音的关闭
#define RE_AL_SD_ON  "F4*1F"  //遥控警号伴音的打开
#define BEEP_TIME    "F5*??F" //警笛鸣响时间
#define AL_DELAY_TM  "F6*??F" //布防延时时间
#define RE_RING_TM   "F7*?F"  //远程操作振铃次数设置
#define WL_STATE     "F8*^?F" //修改无线防区的状态
#define L_AL_OFF     "G0*0G"  //有线防区关闭
#define L_AL_ON      "G0*1G"  //有线防区开启
#define PASD_SET     "G7*^^G" //修改操作密码的设置
#define BD_ID_SET    "G9*^G"  //主机ID码设置
#define BD_RESET     "I^^I"   //主机总复位


GOLBLE_KEY  void key_rec_set(KER_PAR*);
GOLBLE_KEY  void key_phone_num_set(KER_PAR*);
GOLBLE_KEY void key_bal_set(KER_PAR* p);
#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

