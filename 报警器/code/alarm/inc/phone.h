/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: phone.h
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/14  || Create this file
**  ******************************************************************************************
*/

#ifndef _PHONE_H_
#define _PHONE_H_
#include "def.h"
#include "main.h"

#define DIAL_CLK    (1 << PC2)
#define DIAL_DATA   (1 << PC3)
#define DIAL_CS     (1 << PC0)

#define CSC9270_DV_PIN  (1 << PC5)
#define CSC9270_DV_DIR  DDRC
#define CSC9270_DV_STA  (PORTC & CSC9270_DV_PIN)

#define CSC9270_D0  (1 << PB3)
#define CSC9270_D1  (1 << PB4)
#define CSC9270_D2  (1 << PB5)
#define CSC9270_D3  (1 << PC6)
#define CSC9270_D2_D0_DIR DDRB
#define CSC9270_D3_DIR DDRC
#define CSC9270_D2_D0_STA   ((PORTB & (CSC9270_D2 | CSC9270_D1 | CSC9270_D0)) >> 3)
#define CSC9270_D3_STA      (((PORTC & CSC9270_D3) >> 3))
#define CSC9270_OE  (1 << PD7)
#define CSC9270_OE_DIR  DDRD
#define CSC9270_OE_CTL  PORTD

#ifdef _PHONE_C_
const char csc9270_dtmf_tab[] = {'D',0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,\
                                 0x09,0x00,'*','#','A','B','C'};
#endif
extern int call_the_phone(STWORK* pInfo);
extern void phone_init(void);

#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

