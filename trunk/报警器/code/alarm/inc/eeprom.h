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

#ifndef _EEPROM_C_
#define GLOGLE_EEPROM   extern
#else
#define GLOGLE_EEPROM
#endif

GLOGLE_EEPROM void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
GLOGLE_EEPROM unsigned char EEPROM_read(unsigned int uiAddress);


#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

