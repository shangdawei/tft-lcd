/******************** (C) COPYRIGHT 2009 TEAM *********************************
* File Name          : datadeal.h
* Author             : gxuliang@gmail.com
* Version            : V0.1
* Date               : 06/06/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/
#ifndef _DATADEAL_H_
#define	_DATADEAL_H_

#ifndef	_DATADEAL_C_
#define	GLOBLE_DATADEAL		extern
#else
#define	GLOBLE_DATADEAL
#endif

enum GPT_CMD
{
	GPT_VER				= 0x0001,//查询版本
	GPT_TIME			= 0x0002,//查询/设置时间
	GPT_DBG				= 0x0003,//主控调试信息
	GPT_PIC_QUERY		= 0x0101,//请求传输图片
	GPT_CHR_QUERY		= 0x0102,//请求传输字库
	GPT_PIC_INDEX		= 0x0103,//查询图片索引
	GPT_CLS				= 0x0201,//清屏
	GPT_PIC_VIEW		= 0x0202,//显示图片
	GPT_CHR_VIEW		= 0x0203,//显示字库
	GPT_PIC_PAR			= 0x0204,//图片效果
	GPT_PIC_DEL			= 0x0205,//删除图片
	GPT_FPGA_RW			= 0x0206,//FPGA读写
};


GLOBLE_DATADEAL u8 CRC_Check(pGPTIP pGptip);
GLOBLE_DATADEAL void DealCmd(pGPTIP pGptip);
GLOBLE_DATADEAL void get_sys_info(pGPTIP pGptip);

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
