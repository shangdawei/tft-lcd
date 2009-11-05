/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: key.c
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/14  || Create this file
**  ******************************************************************************************
*/

#define _KEY_C_

#include "def.h"
#include "key.h"
#include "eeprom.h"
#include "main.h"
#include "voice.h"
#include <string.h>
#include "stdlib.h"

#define	MAX_CHAR	32
#define PARTAB_SZ		(sizeof(KeyTable) / sizeof(PAR_TAB))
static char key_char[MAX_CHAR]="";
//static KER_PAR key_par;
const uint8 keyVal[] = {1,2,3,'F',4,5,6,'G',7,8,9,'H','*',0,'#','I','A','B','C'};
//�������������б�
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
==  Description :   ��������
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
==  Description :   ���������ַ���   ����İ������ú�˵��������в�ͬ��Ӧ�ø���˵���ĵ�
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
		case '*'://���ڿ�������
			for(i = 1; i < (num -1); i++)
				passwd |= (p[i] << (16 - i*4));
			if(passwd == pInfo->password)
			{
				pInfo->is_key_open = KEY_OPEN;
			}
		case 'H'://¼��
			if(pInfo->is_key_open == KEY_CLOSE)
				return ;
			for(i = 1; i < (num -1); i++)
				passwd |= (p[i] << (16 - i*4));
		//	key_par.old_passwd = passwd;
			key_rec_set(passwd);
		//	strncpy(p, REC_LWORD, sizeof(REC_LWORD));
			break;
		case 'G'://����
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
					key_phone_num_set(&p[1]);//����绰����
					phone_work_save(pInfo);
				//	strncpy(p, SET_PNUM, sizeof(SET_PNUM));				
					break;
				case 0:
					pInfo->l_al = (p[2] == 0x30)?0:1;//���߷�������/�ر�
					break;
				case 7:
					if(memcmp(&p[2], &p[6], 4) == 0)//2������������һ��
					{
						for(i = 1; i <= 4; i++)
							passwd |= (p[i + 1] << (16 - i*4));
						pInfo->password = passwd;
						//����
						phone_work_save(pInfo);
					}
					break;
				case 9://��������ID��
					for(i = 1; i <= 6; i++)
						id |= (p[i + 1] << (24 - i*4));
					
					pInfo->localID = id;
					//����
					phone_work_save(pInfo);
					break;
				default:
					break;
			}
			break;
		case 'F'://���
			if(pInfo->is_key_open == KEY_CLOSE)
				return ;
			switch (p[1])
			{
				case 1:
					pInfo->bal_md = (p[2] == 0x30)?0:1;//���߱������� 1-������0-�ر�
					phone_work_save(pInfo);
					break;
				case 2:
					pInfo->sery_md = (p[2] == 0x30)?0:1;//���뱣������ 1-������0-�ر�
					phone_work_save(pInfo);
					break;
				case 3:
					pInfo->al_sd_md = (p[2] == 0x30)?0:1;//0-����������1-��������
					phone_work_save(pInfo);
					break;
				case 4:
					pInfo->re_al_sd_md = (p[2] == 0x30)?0:1;//ң�ؾ��Ű��� 1-������0-�ر�
					phone_work_save(pInfo);
					break;
				case 5:
					for(i = 1; i <= 2; i++)
					{
						val |= (p[i + 1] << (8 - i*4));
					}
					pInfo->beep_tm = val > 30?30:val;//��������ʱ�䣬���30����
					phone_work_save(pInfo);
					break;
				case 6:
					for(i = 1; i <= 2; i++)
					{
						val |= (p[i + 1] << (8 - i*4));
					}
					pInfo->al_del_tm = val > 99?99:val;//������ʱʱ��,���99��
					phone_work_save(pInfo);
					break;
				case 7:
					pInfo->re_ring_tm = val > 9?9:val;//Զ�̲��������������
					phone_work_save(pInfo);
					break;
				case 8:
					pInfo->al_sd_md = (p[2] == 0x30)?0:1;//0-����������1-��������
					phone_work_save(pInfo);
					break;
				default:
					break;
			}
			break;
		case 'I'://���
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

void init_key(void)
{
  DDRC |= ROW_PIN; //line is output
  
  /*row is input*/
  DDRC &= ~(1<<PC6);
  DDRB &= ~(1<< PB3 | 1<<PB4 | 1<<PB5);
}

void set_port(uint8 i)
{
  PORTC |= ROW_PIN;//��������״̬
  
  if(i == 0)
  {
    PORTC &= ~(1<<PC6);//����͵�ƽ
    PORTB |= (1<< PB3 | 1<<PB4 | 1<<PB5);//�������Ϊ��
  }
  else
  {
    PORTC |= (1<<PC6);//����ߵ�ƽ
    
    PORTB |= (1<< PB3 | 1<<PB4 | 1<<PB5);//�������Ϊ��
    PORTB &= ~((1<<PB5) >> (i - 1));//��Ӧλ����͵�ƽ
  }
}

uint8 convert(uint8 val)
{
  const uint8 tab[]={1<<PC5, 1<<PC4, 1<<PC3, 1<<PC2, 1<<PC1};
  uint8 i;
  for(i = 0; i < sizeof(tab); i++)
  {
    if(tab[i] == val)
      return i;
  }
  return 0xFF;
}

void delay50us(uint32 n)
{
  uint32 i,j;
  for(i = 0; i < n; i++)
  {
    for(j = 0; j < 100; j++)
      ;
  }
}


uint8 key_read(void)
{
  uint8 i;
  uint8 val;
  init_key();
  for(i = 0; i < 4; i++)
  {
    set_port(i);
    delay50us(1);
    if((KEY_PIN & ROW_PIN) != ROW_PIN)
    {
      val = KEY_PIN & ROW_PIN;//�õ��У�i��ֵ������ֵ
      val = convert(val);//����ת��
      return (val == 0xFF)?val:(val*4 + i);
    }
  }
  return 0xFF;
}


/*  $Function   :   key_rec_set
==  ==============================================================================================
==  Description :   ¼������
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
	voice_rec(START);//��ʼ¼��
}

/*  $Function   :   key_phone_num_set
==  ==============================================================================================
==  Description :   ���õ绰����
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
==  Description :   ���ö��߱�������/�ر�
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
==  Description :   ���뱣�����ܿ���/�ر�
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
==  Description :   ͨ�ò������� ����/�ر�
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
