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

#define		SYS_VER		0x0001

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


#define SUCCESS         0
#define FAILURE        -1

enum INTERFACE
{
	COM1		=	0,
	COM2		=	1,
	COM3		=	2,
	USB			=	3,
};

typedef	struct	_VER_INFO
{
	u32		pcb_ver;
	u32		fpga_ver;
	u32		mcu_ver;
	u8		sge;//显示特效
}VER_INFO, *pVER_INFO;

typedef	struct _PIC_INFO
{
	u16		id;
	u32		size;
	u32		addr;//保存在FLASH上的首地址
}PIC_INFO, *pPIC_INFO;

/*全局显示模式*/
typedef	struct _PIC_SHOW
{
	u8	mode;//显示模式
	u8	time;//切换时间
	u16	id_l;//显示图片下限
	u16	id_h;//显示图片上限
}PIC_SHOW;

GLOBLE_DATADEAL u8 CRC_Check(pGPTIP pGptip);
GLOBLE_DATADEAL void DealCmd(u8 COMx, pGPTIP pGptip);
GLOBLE_DATADEAL void get_sys_info(u8 COMx, pGPTIP pGptip);
GLOBLE_DATADEAL int cmd_send_ack(u8 COMx, void* data, u16 len);

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
