/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: eeprom.h
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/22  || Create this file
**  ******************************************************************************************
*/

#ifndef _EEPROM_H_
#define _EEPROM_H_
#include "def.h"
#include "main.h"

#ifndef _EEPROM_C_
#define GLOGLE_EEPROM   extern
#else
#define GLOGLE_EEPROM
#endif

typedef struct {
  uint8 phone_num[16];
  uint8 len;
} StPhSla;

#define   PN_ADD    0x00
#define   STWORK_ADD     (PN_ADD + sizeof(StPhSla)*6)


GLOGLE_EEPROM void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
GLOGLE_EEPROM unsigned char EEPROM_read(unsigned int uiAddress);
GLOGLE_EEPROM void phone_num_save(uint8 sn, uint8* num, uint8 len);
GLOGLE_EEPROM void phone_work_save(STWORK *p_mode);
GLOGLE_EEPROM void phone_work_read(STWORK *p_mode);

#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

