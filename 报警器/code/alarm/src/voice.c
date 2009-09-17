/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: voice.c
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/17  || Create this file
**  ******************************************************************************************
*/

#include "voice.h"


void voice_rec(bool i)
{
  REC_DIR |= REC_PIN;
  if(i == START)
    REC_CTL |= REC_PIN;
  else
    REC_CTL &= ~REC_PIN;
}
/*
void voice_rec_stop(void)
{
  REC_DIR |= REC_PIN;
  REC_CTL &= ~REC_PIN;
}
*/
/*====================================================================*/
void voice_play(bool i)
{
    PLAY_DIR |= PLAY_PIN;
    if(i == START)
      PLAY_CTL |= PLAY_PIN;
    else
      PLAY_CTL &= ~PLAY_PIN;
}
/*
void voice_play_stop(void)
{
    PLAY_DIR |= PLAY_PIN;
    PLAY_CTL &= ~PLAY_PIN;
}
*/
/*====================================================================*/
void FT_mode(bool i)
{
  FT_DIR |= FT_PIN;
  if(i == START)
  {
     FT_CTL |= FT_PIN;
     voice_rec(STOP);
     voice_play(STOP);
     //set PLAYE low
  }
  else
  {
     FT_CTL &= ~FT_PIN;
  }
}
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/

