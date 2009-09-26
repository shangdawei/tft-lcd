/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: voice.h
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/17  || Create this file
**  ******************************************************************************************
*/

#ifndef _VIE_H_
#define _VIE_H_
#include "def.h"

#ifndef _VIE_C_
#define GLOBLE_VIE extern
#else
#define GLOBLE_VIE
#endif


#define REC_DIR   DDRD
#define REC_CTL   PORTD
#define REC_PIN   (1<<PD6)

#define PLAY_DIR   DDRD
#define PLAY_CTL   PORTD
#define PLAY_PIN   (1<<PD5)

#define FT_DIR    DDRB
#define FT_CTL    PORTB
#define FT_PIN    (1<<PB7)

GLOBLE_VIE bool Rec_Flag;


GLOBLE_VIE void voice_rec(bool i);
#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

