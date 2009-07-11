/******************** (C) COPYRIGHT 2009 TEAM *********************************
* File Name          : picture.h
* Author             : gxuliang@gmail.com
* Version            : V0.1
* Date               : 06/30/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/
#ifndef _PICTURE_H_
#define	_PICTURE_H_
#include "gptip.h"
#ifndef	_PICTURE_C_
#define	GLOBLE_PICTURE		extern
#else
#define	GLOBLE_PICTURE
#endif


GLOBLE_PICTURE void pic_upload(u8 COMx, pGPTIP pGptip);
GLOBLE_PICTURE void pic_download(u8 COMx, pGPTIP pGptip);
GLOBLE_PICTURE void show_pic(u8 COMx, pGPTIP pGptip);


#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

