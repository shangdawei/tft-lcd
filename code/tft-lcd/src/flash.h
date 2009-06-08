/******************** (C) COPYRIGHT 2009 TEAM *********************************
* File Name          : flash.h
* Author             : gxuliang@gmail.com
* Version            : V0.1
* Date               : 06/06/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/
#ifndef _FLASH_H_
#define	_FLASH_H_

#ifndef	_FLASH_C_
#define	GLOBLE_FLASH		extern
#else
#define	GLOBLE_FLASH
#endif
GLOBLE_FLASH void Init_nandflash(void);
GLOBLE_FLASH void flash_erase(u32 sect_addr);
GLOBLE_FLASH void flash_read(u8 *buf, u32 addr, u32 cnt);
GLOBLE_FLASH int flash_write(u8 *buf, u32 addr, u32 cnt);

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
