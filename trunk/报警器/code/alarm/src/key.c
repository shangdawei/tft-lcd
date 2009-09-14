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

#include "def.h"
#include "key.h"
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

uint8 key_scan(void)
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

/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
