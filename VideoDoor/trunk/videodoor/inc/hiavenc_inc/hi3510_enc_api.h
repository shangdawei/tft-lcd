
/************************************************************************************************
*
* Copyright (c) 2010,�㽭�󻪼����ɷ����޹�˾ �з�����
* All rights reserved.
* 
*************************************************************************************************
*
* �ļ����ƣ�hi3510_enc_api.h.h
* ժ           Ҫ��
* ��           �������ļ��������ز����Ŀ�Ľӿ��ļ�
* 
* ��ǰ�汾��1.0.1
* ��           �ߣ�fang_yixin@dahuatech.coms
* ��  ��   �ߣ�fang_yixin@dahuatech.com
* �������ڣ�2010��9��23��
*
*************************************************************************************************/


#ifndef _M_HI3510_AVENC_MM_INC_H_
#define _M_HI3510_AVENC_MM_INC_H_


typedef struct {
   int left;
   int top;
   int right;
   int bottom;
}Rect,*pRect;

/// ��Ƶ����ѹ����ʽ����
typedef enum VideoEncType
{
	videoEncMPEG2 = 0,	///< MPEG2
	videoEncMPEG4,		///< MPEG4
	videoEncH263,		///< H.263
	videoEncH264,		///< H.264
	videoEncMotionJPEG,	///< MJPG
	videoEncNumber,		///< ѹ��������Ŀ
} VideoEncType;


/// ����оƬ���������ṹ��128�ֽ�
typedef struct VideoEncChipCaps 
{
	unsigned long ladenBitrate;		///< ����оƬ֧�ֵ������Ƶ��������kbps(ѹ��ǰ,��16bpp����)
	int reserved[31];		///< �����ֽ�
} VideoEncChipCaps;


/// �����豸�����ṹ��128�ֽ�
typedef struct VideoEncCaps
{
	unsigned long types;	///< ����оƬ֧�ֵ���Ƶѹ������VideoEncType������
	char coverCount;		///< ֧���������򸲸Ǹ�����
	char titleCount;		///< ֧�����ı�����ӵĸ�����
	char snapByOtherSize;	///< �Ƿ��ж�����ץͼ�ֱ��ʣ�����ʹ���������ֱ��ʡ�
	float snapFps;			///< ���ץͼ֡��
	int reserved[29];		///< �����ֽ�
} VideoEncCaps;


/// ��Ƶ������������ģʽ����
typedef enum VideoEncBitrateCtrl
{
	videoEncBitrateCtrlConstant = 0,	///< �̶�������
	videoEncBitrateCtrlVariable,	///< �ɱ�������
} VideoEncBitrateCtrl;

/// ��Ƶ֡�����ʽ
typedef enum VideoEncPackType
{
	videoEncPackTypeDHAV = 0,		///< DHAV��ʽ���
	videoEncPackTypePS,			///< PS����ʽ���
}VideoEncPackType;

/// ��Ƶ�������ݸ�ʽ�ṹ��128�ֽ�
typedef struct VideoEncFormat
{
	VideoEncType		type;				///< ѹ����׼��ȡVideoEncType���͵�ֵ��
	int					width;				///< ͼ�����ؿ�ȣ�16�ı���,����64 
	int					height;				///< ͼ�����ظ߶ȣ�16�ı���,����64 
	VideoEncBitrateCtrl	bitrateControl;		///< ��������ģʽ��ȡVideoEncBitrateCtrl���͵�ֵ�� 
	int					bitrate;			///< �ο�������С��kbpsΪ��λ��
	float				fps;				///< ֡��ֵ��NTSC/PAL������
	char				gop : 8;			///< I֡�����������֡����һ��I֡������PAL����һ��ȡֵ25��50��
	char				quality : 8;		///< ͼ��������ȡֵ1-6��ֵԽ��ͼ������Խ��,ֻ���ڿɱ������²������á�
	VideoEncPackType	pack : 8;			///< �����ʽ
	int					reserved[25];		///< �����ֽ�
} VideoEncFormat;


/// ֡����
typedef enum VideoEncFrameType
{
	videoEncFrameI = 0,			///< ��I֡
	videoEncFrameP,				///< ��P֡
	videoEncFrameB,				///< ��B֡
	videoEncFrameW,				///< ��ˮӡ֡
	videoEncFrameG, 			///< GPS֡
	videoEncFrameNumber			///< ֡������Ŀ
} VideoEncFrameType;


/// ��Ƶ�������128�ֽ�
typedef struct VideoEncPacket
{
	unsigned char				*buffer;	///< ���ݻ���ָ��
	unsigned long				length;		///< ���ݻ�����Ч����
	unsigned long				pts;		///< ʱ���(jiffies_64*(1000/HZ))������Ϊ��λ
	unsigned long				sequence;	///< ֡��ţ�����У���Ƿ�֡
	VideoEncFrameType	type;		///< ֡����
	char				head;		///< �Ƿ����֡ͷ��1-������0-������
	char				tail;		///< �Ƿ����֡β��1-������0-������
	char				newFormat;	///< �����ʽ�Ƿ����˱䶯��0��δ�䶯��1���ֱ����б䶯��2�����������б䶯
	char				remain;		///< ����������ݰ����Ƿ��б�֡������֡�ĺ������ݿ��Ի�ȡ
	unsigned long				utc;		///< ֡��ͼ���Ӧ��UTC(1970-1-1 00:00:00)����
	unsigned char				mark;		///< ָ��֡��ǣ�����VideoIn�����õ�ֵ��0��ʾû�б��
	unsigned char				specLength;	///< ������CodecSpecInfo���ݳ��ȣ����Ϊ0���ʾ��Ч
	unsigned char				*specBuffer;///< CodecSpecInfo������Ϣ������H.264��PPS,SPS����Ϣ
	unsigned long				ptsBeforeEnc;///< ����ǰ��ʱ���(jiffies_64*(1000/HZ))
	unsigned long				ptsAfterEnc;///< ������ʱ���(jiffies_64*(1000/HZ))
	unsigned long				ptsLib;		///< ����������ȡ�������ݰ���ʱ���(jiffies_64*(1000/HZ))
	int					reserved[16];///< �����ֽ�
} VideoEncPacket;






// ��Ƶ����ӿڣ�494�ֽ�
typedef struct VideoEncoder
{
	/// �ײ�˽������
	void *priv;


	/// ���ӽӿ�����
	int (*addRef)(struct VideoEncoder *thiz);


	/// �ͷŽӿ�
	int (*release)(struct VideoEncoder *thiz);


	/// �õ����������ڵ�оƬID
	///
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \retval >=0 оƬID���ɿ�����ͳһ���䣬��ͬ��оƬ��Ψһ�ı�š�
	///					оƬ���ڴ�������ͨ����ʱ��õ�
	/// \retval < ��ȡʧ��
	int (*getChipId)(struct VideoEncoder *thiz);


	/// ���оƬ�ı�������
	///
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \param [out] caps оƬ�����ṹVideoEncChipCapsָ��
	///
	int (*getChipCaps)(struct VideoEncoder *thiz, VideoEncChipCaps *pcaps);


	/// �õ���Ƶ�����豸����
	/// 
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \retval 0 ����ʧ��
	/// \retval <0 ���ٳɹ�
	int (*getCaps)(struct VideoEncoder *thiz, VideoEncCaps *pcaps);


	/// ǿ�Ʊ���������I֡���µ�I֡Ӧ������һ�����ݰ�VideoEncPacket��һ֡��
	/// 
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \retval 0  ���óɹ���
	/// \retval <0  ����ʧ�ܡ�
	int (*forceIFrame)(struct VideoEncoder *thiz);


	/// ������Ƶ��������ѹ���ĸ�ʽ��
	/// 
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \param [in] format ָ����Ƶ��ʽ�ṹVideoEncFormat��ָ�룬NULL��ʾ��������Ƶ��
	/// \retval 0  ���óɹ�
	/// \retval <0  ����ʧ��
	int (*setFormat)(struct VideoEncoder *thiz, VideoEncFormat* pformat);


	/// ��ʼ���롣����Ƶ��ʽ����Ƶ����ѡ�ͼ��������֡�ʡ��������Ʒ�ʽ���ֱ��ʡ�
	/// ����ģʽ�Ȳ������ı�ʱ��Ӧ�ó����ֹͣ��Ƶ���룬Ȼ�����¿�ʼ��Ƶ���롣
	/// 
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \retval 0  ��ʼ�ɹ�
	/// \retval <0  ��ʼʧ��
	int (*start)(struct VideoEncoder *thiz);


	/// ֹͣ���룬��ǰ�����������ء�
	/// 
	/// \param [in]  thiz ��Ƶ����ӿ�ָ��
	/// \retval 0  ֹͣ�ɹ�
	/// \retval <0  ֹͣʧ��
	int (*stop)(struct VideoEncoder *thiz);




	/// ��ȡ�������ݣ���Ϊ������ģʽ��ÿ������ȡһ֡���ݣ����֡�����Ѿ��ֿ飬
	/// ��ֶ�δ��������û�����ݣ��򷵻ش���
	/// 
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \param [out] packet ���뻺��ṹVideoEncPacketָ�롣
	/// \return>=0 ͨ���ţ�������ͨ����Ϊ-1ʱ��
	/// \retval 0  ���ɹ�
	/// \retval <0  ��ʧ��
	int (*getPacket)(struct VideoEncoder *thiz, VideoEncPacket* ppacket);


	/// �ͷ���Ƶ�������ݣ���Ҫ��VideoEncGetPacket�ɶԵ��á�
	///
	/// \param [in]  thiz ��Ƶ����ӿ�ָ��
	int (*releasePacket)(struct VideoEncoder *thiz);


	/// ������Ƶ��������Ƶ���������
	/// 
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \param [in] src ��Ƶ����ͨ����
	/// \param rect ����������е���Ƶ���򣬸�������Զ���һ���߿�������õ�����
	///                 ������Ƶ�������㡣
	/// \retval 0  ���óɹ�
	/// \retval <0  ����ʧ��
	int (*setInputRect)(struct VideoEncoder *thiz, int src, Rect *prect);

	
	/// Ӧ�ó���ʱ���ã� ֪ͨ�����豸�������״̬�� ����д��� �����豸��ͬʱ���ش��� 
	/// 
	/// \param [in] thiz ��Ƶ����ӿ�ָ��
	/// \retval 0  ͬ���ɹ�
	/// \retval <0  ͬ��ʧ��
	void (*synchronize)(struct VideoEncoder *thiz);




	///< �����ֽ�
	int	*				reserved[110];
} VideoEncoder;


/// ��������
typedef enum VideoEncStream
{
	videoEncStreamMain = 0,	///< ������
	videoEncStreamExtra1,	///< ������1
	videoEncStreamExtra2,	///< ������2
	videoEncStreamExtra3,	///< ������3
	videoEncStreamSnapshot,	///< ץͼ
	videoEncStreamNumber	///< ������
} VideoEncStream;


/// ��Ƶ����ӿ�������128�ֽ�
typedef struct VideoEncDesc
{
	VideoEncStream	stream;		///< ָ����������������
	ulong srcs;					///< ��������ͨ�������룬ÿһλ��Ӧһ����Ƶ��������ͨ��
	int reserved[30];			///< ����
} VideoEncDesc;


/// ������Ƶ�����豸������Ķ�������ɶ����Ƶ����Դ��϶��ɡ������ɹ������оƬ��Դ
/// ���ƣ���һ�������������룬Ӧ�ó�����Ҫ���ݱ������������㡣
///
/// \param [in] desc  ��Ƶ����ӿ������ṹVideoEncDescָ��
/// \param [out] videoEncoder ��Ƶ����ӿ�ָ���ָ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int createVideoEncoder(VideoEncDesc *desc, VideoEncoder **videoEncoder);


//�˽ӿ�Ϊ��ʱ�Ĳ��Խӿڣ��Ӽ���������ز�������������Ӧ���ܵĲ��Դ���
int HiSlaveDataProc(void);
/*

int sysinit(void);
*/
#endif

