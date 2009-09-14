/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: phone.c
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/14  || Create this file
**  ******************************************************************************************
*/

#include "def.h"
#include "phone.h"

static void clk_h(void)
{
	PORTC |= DIAL_CLK;
}

static void clk_l(void)
{
	PORTC &= ~DIAL_CLK;
}

static void put_value(uint8 value)
{
	if(value >= 1)
		PORTC |= DIAL_DATA;
	else
		PORTC &= ~DIAL_DATA;
}

static void enable_cs(void)
{
	DDRC |= DIAL_CS;	//output 
	PORTC &= ~DIAL_CS;	//put 0,enable CS
}
static void disable_cs(void)
{
	DDRC |= DIAL_CS;	//output 
	PORTC |= DIAL_CS;	//put 0,disable CS
}

void put_9200a(uint8 value)
{
	int8 i;
	
//	PORTC &= !DIAL_CS;	//put 0,enable CS
	
	for(i = 4; i >= 0 ; i--)
	{
		clk_h();
		put_value((value >> i) & 0x01);
		__delay_cycles(20);
		clk_l();
		__delay_cycles(20);
	}
	 __delay_cycles(20);
	__delay_cycles(20);
//	PORTC |= DIAL_CS;
	
}

void init_dial(void)
{
	DDRC |= DIAL_CS | DIAL_CLK | DIAL_DATA;	//output 
}

void dial_num(uint8 *p, uint8 num)
{
	uint8 i;
	init_dial();
	enable_cs();
	for(i = 0; i < num; i++)
		put_9200a(p[i]);
	put_9200a(0xFF);	//stop code
	disable_cs();
}
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
