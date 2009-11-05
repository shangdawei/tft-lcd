/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: eeprom.c
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/22  || Create this file
**  ******************************************************************************************
*/

#define _EEPROM_C_
#include "def.h"
#include "eeprom.h"
#include <string.h>


void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
/* 等待上一次写操作结束 */
while(EECR & (1<<EEPE))
;
/* 设置地址和数据寄存器 */
EEAR = uiAddress;
EEDR = ucData;
/* 置位EEMWE */
EECR |= (1<<EEMPE);
/* 置位EEWE 以启动写操作E */
EECR |= (1<<EEPE);
}


unsigned char EEPROM_read(unsigned int uiAddress)
{
/* 等待上一次写操作结束 */
while(EECR & (1<<EEPE))
;
/* 设置地址寄存器 */
EEAR = uiAddress;
/* 设置EERE 以启动读操作 */
EECR |= (1<<EERE);
/* 自数据寄存器返回数据 */
return EEDR;
}

/*  $Function   :   phone_num_save
==  ==============================================================================================
==  Description :   电话号码保存到eeprom   废弃！！！
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
void phone_num_save(uint8 sn, uint8* num, uint8 len)
{
  StPhSla st_phone;
  uint8 i;
  uint8 *p=(uint8*)(&st_phone);
  memcpy(st_phone.phone_num, num, len);
  st_phone.len = len;
  
  
  for(i = 0; i < sizeof(StPhSla); i++)
  {
     EEPROM_write(PN_ADD + i + (sn - 1)*sizeof(StPhSla), p[i]);
  }
}

/*  $Function   :   phone_work_save
==  ==============================================================================================
==  Description :   工作参数保存到eeprom
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
void phone_work_save(STWORK *p_mode)
{
  uint8 i;
  uint8 *p = (uint8*)p_mode;
  for(i = 0; i < sizeof(STWORK); i++)
  {
    if(p[i] != EEPROM_read(STWORK_ADD + i))
      EEPROM_write(STWORK_ADD + i, p[i]);
  }
}
/*  $Function   :   phone_work_read
==  ==============================================================================================
==  Description :   工作参数从eeprom读取
==  ==============================================================================================
==  Argument    :   
==  ==============================================================================================
==  Return      :   
==              :   
==  ===============================================================================================
==  History     : Modify by  ||    ID    ||     Date      ||     Contents
==              :   xul      ||          ||   2009/11/5  || Create this function
==  ===============================================================================================
*/
void phone_work_read(STWORK *p_mode)
{
	uint8 i;
	uint8 *p = (uint8*)p_mode;
	for(i = 0; i < sizeof(STWORK); i++)
	{
		p[i] =  EEPROM_read(STWORK_ADD + i);
	}
}
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
