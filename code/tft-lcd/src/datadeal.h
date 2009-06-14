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
	GPT_VER				= 0x0001,//��ѯ�汾
	GPT_TIME			= 0x0002,//��ѯ/����ʱ��
	GPT_DBG				= 0x0003,//���ص�����Ϣ
	GPT_PIC_QUERY		= 0x0101,//������ͼƬ
	GPT_CHR_QUERY		= 0x0102,//�������ֿ�
	GPT_PIC_INDEX		= 0x0103,//��ѯͼƬ����
	GPT_CLS				= 0x0201,//����
	GPT_PIC_VIEW		= 0x0202,//��ʾͼƬ
	GPT_CHR_VIEW		= 0x0203,//��ʾ�ֿ�
	GPT_PIC_PAR			= 0x0204,//ͼƬЧ��
	GPT_PIC_DEL			= 0x0205,//ɾ��ͼƬ
	GPT_FPGA_RW			= 0x0206,//FPGA��д
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
	u8		sge;//��ʾ��Ч
}VER_INFO, *pVER_INFO;

typedef	struct _PIC_INFO
{
	u16		id;
	u32		size;
	u32		addr;//������FLASH�ϵ��׵�ַ
}PIC_INFO, *pPIC_INFO;

/*ȫ����ʾģʽ*/
typedef	struct _PIC_SHOW
{
	u8	mode;//��ʾģʽ
	u8	time;//�л�ʱ��
	u16	id_l;//��ʾͼƬ����
	u16	id_h;//��ʾͼƬ����
}PIC_SHOW;

GLOBLE_DATADEAL u8 CRC_Check(pGPTIP pGptip);
GLOBLE_DATADEAL void DealCmd(u8 COMx, pGPTIP pGptip);
GLOBLE_DATADEAL void get_sys_info(u8 COMx, pGPTIP pGptip);
GLOBLE_DATADEAL int cmd_send_ack(u8 COMx, void* data, u16 len);

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
