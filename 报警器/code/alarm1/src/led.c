//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, �㽭�����������޹�˾
///    All rights reserved.
///
/// @file   led.c
/// @brief  ��������
///
///
///
/// @version    2.0
/// @author     xuliang<gxuliang@gmail.com>
/// @date       2010��04��24
///
///
///     �޶�˵��������汾
//////////////////////////////////////////////////////////////////////////

#include "def.h"
#include "led.h"


static WORD LedVal=0;
//////////////////////////////////////////////////////////////////////////
///
///     LedInit
///     @param ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
void LedInit(void)
{
    GPIOD->DDR |= GPIO_PIN_2; //AB
    GPIOC->DDR |= GPIO_PIN_3; //CLK
}
//////////////////////////////////////////////////////////////////////////
///
///     LedPut
///     @param 16յled��ֵ��ÿλ1��ʾ����0��ʾ��
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
static void LedPut(WORD val)
{
    BYTE i;
    for(i = 0; i < 16; i++)
    {
        if( (0x8000 >> i) & val )
            GPIOD->ODR |= GPIO_PIN_2;//PD2 as AB
        else
            GPIOD->ODR &= ~GPIO_PIN_2;//PD2 as AB
        //PC3����һ�������أ�����Ҫ���ӳ�
        GPIOC->ODR &= ~GPIO_PIN_3;//PC3 as CLK
        GPIOC->ODR |= GPIO_PIN_3;//PC3 as CLK
    }
}
//////////////////////////////////////////////////////////////////////////
///
///     LedOn
///     @param mask
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
void LedOn(WORD mask)
{
    LedVal |= mask;//
    LedPut(LedVal);
}
//////////////////////////////////////////////////////////////////////////
///
///     LedOff
///     @param mask
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-08-24
//////////////////////////////////////////////////////////////////////////
void LedOff(WORD mask)
{
    LedVal &= ~mask;//
    LedPut(LedVal);
}