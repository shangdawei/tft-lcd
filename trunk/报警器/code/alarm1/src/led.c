//////////////////////////////////////////////////////////////////////////
///    COPYRIGHT NOTICE
///    Copyright (c) 2010, 浙江共创技术有限公司
///    All rights reserved.
///
/// @file   led.c
/// @brief  参数配置
///
///
///
/// @version    2.0
/// @author     xuliang<gxuliang@gmail.com>
/// @date       2010－04－24
///
///
///     修订说明：最初版本
//////////////////////////////////////////////////////////////////////////

#include "def.h"
#include "led.h"


static WORD LedVal=0;
//////////////////////////////////////////////////////////////////////////
///
///     LedInit
///     @param 无
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
///     @param 16盏led的值，每位1表示亮，0表示灭
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
        //PC3产生一个上升沿，可能要加延迟
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