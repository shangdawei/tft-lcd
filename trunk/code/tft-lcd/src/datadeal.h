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


GLOBLE_DATADEAL u8 CRC_Check(pGPTIP pGptip);
GLOBLE_DATADEAL void DealCmd(pGPTIP pGptip);
GLOBLE_DATADEAL void get_sys_info(pGPTIP pGptip);

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
