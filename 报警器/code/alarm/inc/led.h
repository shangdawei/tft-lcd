/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: led.h
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/15  || Create this file
**  ******************************************************************************************
*/

#ifndef _LED_H_
#define _LED_H_
#include "def.h"

#define LED_CLR (1<<PB2)
#define LED_DAT (1<<PC1)
#define LED_CLK (1<<PB1)

#define LED_DAT_DIR  DDRC
#define LED_CLR_DIR  DDRB
#define LED_CLK_DIR  DDRB
#define LED_DAT_CTL  PORTC
#define LED_CLR_CTL  PORTB
#define LED_CLK_CTL  PORTB

#define D2  0
#define D3  1
#define D4  2

#define LED_D4_DIR  DDRB
#define LED_D4_CTL  PORTB
#define LED_D4      (1<<PB6)

//                   '0'  '1'    '2'  '3'   '4'    '5'  '6'   '7'   '8'   '9'   'F'   'S'   'b'
const uint8 dig1[]={0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x51, 0x00, 0x10, 0x07, 0x12, 0x03};
const uint8 dig2[]={0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x51, 0x00, 0x10, 0x07, 0x12, 0x03};

#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

