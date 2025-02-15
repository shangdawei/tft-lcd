//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file	key.c
/// @brief	按键处理
///
///
///
/// @version	1.0     
/// @author		xul	<gxuliang@gmail.com>          
/// @date		2009年09月14日                 
///
///
///     修订说明：最初版本
//////////////////////////////////////////////////////////////////////////
#define _KEY_C_

#include "def.h"
#include "key.h"
#include "eeprom.h"
#include "main.h"
#include "voice.h"
#include "time.h"
#include <string.h>
#include "stdlib.h"

#define	MAX_CHAR	32
#define PARTAB_SZ		(sizeof(KeyTable) / sizeof(PAR_TAB))
static char key_char[MAX_CHAR]="";
//static KER_PAR key_par;
const uint8 keyVal[] = {1,2,3,'F',4,5,6,'G',7,8,9,'H','*',0,'#','I','A','B','C'};
//建立按键查找列表
/*
PAR_TAB KeyTable[] = {
	{  REC_LWORD,   key_rec_set,},
	{  SET_PNUM,    key_phone_num_set,},
	{  BAL_OFF,     general_par_set,},
	{  BAL_ON,     general_par_set,},
	{  SERY_OFF,     general_par_set,},
	{  SERY_ON,     general_par_set,},
	{  AL_SD_OFF,     general_par_set,},
	{  AL_SD_ON,     general_par_set,},
	{  RE_AL_SD_OFF,     general_par_set,},
	{  RE_AL_SD_ON,     general_par_set,},
	{  BEEP_TIME,     general_par_set,},
	{  AL_DELAY_TM,     general_par_set,},
	{  RE_RING_TM,     general_par_set,},
	{  L_AL_OFF,     general_par_set,},
	{  L_AL_ON,     general_par_set,},
     //   {  BAL_OFF,     general_par_set,},
    //    {  BAL_OFF,     general_par_set,},
};*/



/*  $Function   :   key_process
==  ==============================================================================================
==  Description :   按键进程
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
void key_process(void)
{
	uint8 val = 0xFF;
	static bool flag = 0;
	static uint8 cnt = 0;
	val = key_read();
	
	if(val != 0xFF && Rec_Flag == 1)
	{
		voice_rec(STOP);
		return ;
	}

	if(flag == 0)
	{
		switch(val)
		{
			case 'F':
			case 'G':
			case 'H':
			case 'I':
		TAG1:
				flag = 1;
				cnt = 0;
				key_char[cnt++] = val;
				break;
			default:
				return ;
		}
	}
	else
	{	
		switch(val)
		{
			case 'F':
			case 'G':
			case 'H':
			case 'I':
				if(key_char[0] != val)
					goto TAG1;

				flag = 0;
				key_char[cnt++] = val;
				key_analyse(key_char, cnt);
				cnt = 0;
				break;	
			default:
				if(cnt >= MAX_CHAR)
				{
					//led show error
					flag = 0;
					cnt = 0;
					return ;
				}
				key_char[cnt++] = val;
				break;
		}
	}	
}
/*  $Function   :   key_analyse
==  ==============================================================================================
==  Description :   分析按键字符串   这里的按键设置和说明书的略有不同，应该给份说明文档
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
void key_analyse(char *p, uint8 num)
{
	uint16 passwd = 0;
	uint32 id = 0, val = 0;
	uint8 i;
	STWORK* pInfo = &gSysinfo;
	switch(p[0])
	{
		case '*'://用于开启键盘
			for(i = 1; i < (num -1); i++)
				passwd |= (p[i] << (16 - i*4));
			if(passwd == pInfo->password)
			{
				pInfo->is_key_open = KEY_OPEN;
			}
		case 'H'://录音
			if(pInfo->is_key_open == KEY_CLOSE)
				return ;
			for(i = 1; i < (num -1); i++)
				passwd |= (p[i] << (16 - i*4));
		//	key_par.old_passwd = passwd;
			key_rec_set(passwd);
		//	strncpy(p, REC_LWORD, sizeof(REC_LWORD));
			break;
		case 'G'://设置
			if(pInfo->is_key_open == KEY_CLOSE)
				return ;
			switch (p[1])
			{
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					key_phone_num_set(&p[1]);//保存电话号码
					phone_work_save(pInfo);
				//	strncpy(p, SET_PNUM, sizeof(SET_PNUM));				
					break;
				case 0:
					pInfo->l_al = (p[2] == 0x30)?0:1;//有线防区开启/关闭
					break;
				case 7:
					if(memcmp(&p[2], &p[6], 4) == 0)//2次输入新密码一样
					{
						for(i = 1; i <= 4; i++)
							passwd |= (p[i + 1] << (16 - i*4));
						pInfo->password = passwd;
						//保存
						phone_work_save(pInfo);
					}
					break;
				case 9://设置主机ID码
					for(i = 1; i <= 6; i++)
						id |= (p[i + 1] << (24 - i*4));
					
					pInfo->localID = id;
					//保存
					phone_work_save(pInfo);
					break;
				default:
					break;
			}
			break;
		case 'F'://编程
			if(pInfo->is_key_open == KEY_CLOSE)
				return ;
			switch (p[1])
			{
				case 1:
					pInfo->bal_md = (p[2] == 0x30)?0:1;//断线报警功能 1-开启，0-关闭
					phone_work_save(pInfo);
					break;
				case 2:
					pInfo->sery_md = (p[2] == 0x30)?0:1;//密码保护功能 1-开启，0-关闭
					phone_work_save(pInfo);
					break;
				case 3:
					pInfo->al_sd_md = (p[2] == 0x30)?0:1;//0-无声报警，1-有声报警
					phone_work_save(pInfo);
					break;
				case 4:
					pInfo->re_al_sd_md = (p[2] == 0x30)?0:1;//遥控警号伴音 1-开启，0-关闭
					phone_work_save(pInfo);
					break;
				case 5:
					for(i = 1; i <= 2; i++)
					{
						val |= (p[i + 1] << (8 - i*4));
					}
					pInfo->beep_tm = val > 30?30:val;//警笛鸣响时间，最大30分钟
					phone_work_save(pInfo);
					break;
				case 6:
					for(i = 1; i <= 2; i++)
					{
						val |= (p[i + 1] << (8 - i*4));
					}
					pInfo->al_del_tm = val > 99?99:val;//布防延时时间,最大99秒
					phone_work_save(pInfo);
					break;
				case 7:
					pInfo->re_ring_tm = val > 9?9:val;//远程操作振铃次数设置
					phone_work_save(pInfo);
					break;
				case 8:
					pInfo->al_sd_md = (p[2] == 0x30)?0:1;//0-无声报警，1-有声报警
					phone_work_save(pInfo);
					break;
				default:
					break;
			}
			break;
		case 'I'://清除
			if(pInfo->is_key_open == KEY_CLOSE)
				return ;
			//do sth
			break;
		default:
			return ;

	}

/*
	for(i = 0; i < PARTAB_SZ; i++)
	{
		if(strncmp(KeyTable[i].ParseStr, p, strlen(KeyTable[i].ParseStr)) == 0)
		{
			 KeyTable[i].ParseProc(&key_par);
			 return ;
		}
	}*/

}

//////////////////////////////////////////////////////////////////////////
///
///     按键初始化，主要是对相关IO进行配置
///		@author xuliang<gxuliang@gmail.com>
///		@date	2010年04月14日
//////////////////////////////////////////////////////////////////////////
void init_key(void)
{
	PORTC |= LINE_PIN;
	DDRC &= ~LINE_PIN; //line is intput，上拉
  
	/*row is output，输出高电平*/
	PORTC |= (1<<PC6);
	PORTB |= (1<< PB3 | 1<<PB4 | 1<<PB5);
	DDRC |= (1<<PC6);
	DDRB |= (1<< PB3 | 1<<PB4 | 1<<PB5);
}

//////////////////////////////////////////////////////////////////////////
///
///     对按键相应列作输出处理
///     @param	i 扫描的列号
///		@author xuliang<gxuliang@gmail.com>
///		@date	2010年04月14日
//////////////////////////////////////////////////////////////////////////
void set_port(uint8 i)
{
	PORTC |= (1<<PC6);
	PORTB |= (1<< PB3 | 1<<PB4 | 1<<PB5);

	switch(i) 
	{
	case 3:
		PORTC &= ~(1<<PC6);//输出低电平
		break;
	case 2:
		PORTB &= ~(1<<PB5);
		break;
	case 1:
		PORTB &= ~(1<<PB4);
		break;
	case 0:
		PORTB &= ~(1<<PB3);
		break;
	default:
		break;
	}
}




//////////////////////////////////////////////////////////////////////////
///
///     扫描键盘，读取key值
////	@return	key_val 成功返回键码，失败返回0xFF
///		@author xuliang<gxuliang@gmail.com>
///		@date	2010年04月14日
//////////////////////////////////////////////////////////////////////////
uint8 key_read(void)
{
	uint8 i;
	uint8 val;
	static uint8 old_bak = 0xFF;
	init_key();
	for(i = 0; i < 4; i++)
	{
		set_port(i);
		if((PINC & LINE_PIN) != LINE_PIN)
			udelay(50);
		if((PINC & LINE_PIN) != LINE_PIN)
		{
			val = ~((PINC & LINE_PIN) >> 0x01) & 0x1F;//得到行，i的值就是列值
			val = val == 4 ? 3 : val;
			val = val == 8 ? 4 : val;
			val = val == 16 ? 5 : val;

			val = (val - 1) * 4 + i;
			if(val >= 20)
				return 0xFF;
			if(old_bak == val)
				return 0xFF;
			
			old_bak = val;
			return keyVal[val];
		}
	}
	old_bak = 0xFF;
	return 0xFF;
}



/*  $Function   :   key_rec_set
==  ==============================================================================================
==  Description :   录音留言
==  ==============================================================================================
==  Argument    :   
==  ==============================================================================================
==  Return      :   
==              :   
==  ===============================================================================================
==  History     : Modify by  ||    ID    ||     Date      ||     Contents
==              :   xul      ||          ||   2009/09/24  || Create this function
==  ===============================================================================================
*/
void key_rec_set(uint16 pwd)
{
	if(pwd != gSysinfo.password)
	{
		//led show error
		return ;
	}
	voice_rec(START);//开始录音
}

/*  $Function   :   key_phone_num_set
==  ==============================================================================================
==  Description :   设置电话号码
==  ==============================================================================================
==  Argument    :   
==  ==============================================================================================
==  Return      :   
==              :   
==  ===============================================================================================
==  History     : Modify by  ||    ID    ||     Date      ||     Contents
==              :   xul      ||          ||   2009/09/24  || Create this function
==  ===============================================================================================
*/
void key_phone_num_set(char* src)
{
	char *p = src;
	STWORK* pInfo = &gSysinfo;
	uint8 num = 0, i;
	num = p[0] - 0x30 - 1;
	if(num >= MAX_PHONE_NUM)
		return ;
	for(i = 0; ; i++)
	{
		pInfo->phone_num[num][i] = p[i+1];
		if(pInfo->phone_num[num][i] == 'G')
		{
			pInfo->phone_num[num][i] = '\0';
			break;
		}
	}

	phone_num_save(num, pInfo->phone_num[num], i);
}
#if 0
/*  $Function   :   key_bal_set
==  ==============================================================================================
==  Description :   设置断线报警开启/关闭
==  ==============================================================================================
==  Argument    :   
==  ==============================================================================================
==  Return      :   
==              :   
==  ===============================================================================================
==  History     : Modify by  ||    ID    ||     Date      ||     Contents
==              :   xul      ||          ||   2009/09/24  || Create this function
==  ===============================================================================================
*/
void key_bal_set(KER_PAR* p)
{
  STWORK *p_mode = &gSysinfo;
  
  p_mode->bal_md = p->val;
  phone_work_save(p_mode);
}
/*  $Function   :   sery_md_set
==  ==============================================================================================
==  Description :   密码保护功能开启/关闭
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
void sery_md_set(KER_PAR* p)
{
  STWORK *p_mode = &gSysinfo;
  
  p_mode->sery_md = p->val;
  phone_work_save(p_mode);
}
#endif
/*  $Function   :   general_par_set
==  ==============================================================================================
==  Description :   通用参数设置 开启/关闭
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
void general_par_set(KER_PAR* p)
{
  STWORK *p_mode = &gSysinfo;
  uint8 *pt = (uint8*)p_mode;
  
  pt[p->mode] =  p->val;
  phone_work_save(p_mode);
}

/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
