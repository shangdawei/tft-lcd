/******************** (C) COPYRIGHT 2009 TEAM *********************************
* File Name          : fpag.h
* Author             : gxuliang@gmail.com
* Version            : V0.1
* Date               : 06/08/2009
* Description        : 
********************************************************************************
*
*
*******************************************************************************/

#ifndef		_FPGA_H_
#define		_FPGA_H_

#ifndef		_FPGA_C_
#define		GLOBLE_FPGA		extern
#else
#define		GLOBLE_FPGA
#endif

enum FPGA_CMD
{
	PCB_VER		=	0x0000,
	FPGA_VER	=	0x0001,
	PIC_ADDR0	,//ͼƬ�׵�ַ
	PIC_ADDR1	,
	PIC_ADDR2	,
	PIC_ADDR3	,
	PIC_LEN0	,//ͼƬ����
	PIC_LEN1	,
	PIC_PAR		,//��ʾ����
	PIC_CMD		,//�л�

	NFH_STA		,//nandflash ״̬(ֻ��)
	NFH_REQ		,//nandflash ��������
};

GLOBLE_FPGA void Init_fpga(void);
GLOBLE_FPGA u16	read_fpga(u16 addr);
GLOBLE_FPGA u8 write_fpga(u16 addr, u16 data);
#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

