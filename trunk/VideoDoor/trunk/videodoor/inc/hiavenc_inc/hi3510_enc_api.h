
/************************************************************************************************
*
* Copyright (c) 2010,浙江大华技术股份有限公司 研发中心
* All rights reserved.
* 
*************************************************************************************************
*
* 文件名称：hi3510_enc_api.h.h
* 摘           要：
* 描           述：此文件处理等相关操作的库的接口文件
* 
* 当前版本：1.0.1
* 作           者：fang_yixin@dahuatech.coms
* 修  改   者：fang_yixin@dahuatech.com
* 创建日期：2010年9月23日
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

/// 视频编码压缩格式类型
typedef enum VideoEncType
{
	videoEncMPEG2 = 0,	///< MPEG2
	videoEncMPEG4,		///< MPEG4
	videoEncH263,		///< H.263
	videoEncH264,		///< H.264
	videoEncMotionJPEG,	///< MJPG
	videoEncNumber,		///< 压缩类型数目
} VideoEncType;


/// 编码芯片编码能力结构，128字节
typedef struct VideoEncChipCaps 
{
	unsigned long ladenBitrate;		///< 编码芯片支持的最高视频负载码流kbps(压缩前,按16bpp计算)
	int reserved[31];		///< 保留字节
} VideoEncChipCaps;


/// 编码设备能力结构，128字节
typedef struct VideoEncCaps
{
	unsigned long types;	///< 编码芯片支持的视频压缩类型VideoEncType的掩码
	char coverCount;		///< 支持最多的区域覆盖个数。
	char titleCount;		///< 支持最多的标题叠加的个数。
	char snapByOtherSize;	///< 是否有独立的抓图分辨率，否则使用主码流分辨率。
	float snapFps;			///< 最大抓图帧率
	int reserved[29];		///< 保留字节
} VideoEncCaps;


/// 视频编码码流控制模式类型
typedef enum VideoEncBitrateCtrl
{
	videoEncBitrateCtrlConstant = 0,	///< 固定码流。
	videoEncBitrateCtrlVariable,	///< 可变码流。
} VideoEncBitrateCtrl;

/// 视频帧打包格式
typedef enum VideoEncPackType
{
	videoEncPackTypeDHAV = 0,		///< DHAV格式打包
	videoEncPackTypePS,			///< PS流格式打包
}VideoEncPackType;

/// 视频编码数据格式结构，128字节
typedef struct VideoEncFormat
{
	VideoEncType		type;				///< 压缩标准，取VideoEncType类型的值。
	int					width;				///< 图像像素宽度，16的倍数,大于64 
	int					height;				///< 图像像素高度，16的倍数,大于64 
	VideoEncBitrateCtrl	bitrateControl;		///< 码流控制模式，取VideoEncBitrateCtrl类型的值。 
	int					bitrate;			///< 参考码流大小，kbps为单位。
	float				fps;				///< 帧率值，NTSC/PAL不区分
	char				gop : 8;			///< I帧间隔，即多少帧出现一个I帧，比如PAL制下一般取值25，50等
	char				quality : 8;		///< 图像质量，取值1-6，值越大，图像质量越好,只有在可变码流下才起作用。
	VideoEncPackType	pack : 8;			///< 打包格式
	int					reserved[25];		///< 保留字节
} VideoEncFormat;


/// 帧类型
typedef enum VideoEncFrameType
{
	videoEncFrameI = 0,			///< 是I帧
	videoEncFrameP,				///< 是P帧
	videoEncFrameB,				///< 是B帧
	videoEncFrameW,				///< 是水印帧
	videoEncFrameG, 			///< GPS帧
	videoEncFrameNumber			///< 帧类型数目
} VideoEncFrameType;


/// 视频编码包，128字节
typedef struct VideoEncPacket
{
	unsigned char				*buffer;	///< 数据缓冲指针
	unsigned long				length;		///< 数据缓冲有效长度
	unsigned long				pts;		///< 时间戳(jiffies_64*(1000/HZ))，毫秒为单位
	unsigned long				sequence;	///< 帧序号，用于校验是否丢帧
	VideoEncFrameType	type;		///< 帧类型
	char				head;		///< 是否包含帧头，1-包含，0-不包含
	char				tail;		///< 是否包含帧尾，1-包含，0-不包含
	char				newFormat;	///< 编码格式是否发生了变动，0：未变动，1：分辨率有变动，2：其他参数有变动
	char				remain;		///< 除了这个数据包，是否还有本帧或其他帧的后续数据可以获取
	unsigned long				utc;		///< 帧内图像对应的UTC(1970-1-1 00:00:00)秒数
	unsigned char				mark;		///< 指定帧标记，即从VideoIn处设置的值，0表示没有标记
	unsigned char				specLength;	///< 包含的CodecSpecInfo数据长度，如果为0则表示无效
	unsigned char				*specBuffer;///< CodecSpecInfo具体信息，比如H.264的PPS,SPS等信息
	unsigned long				ptsBeforeEnc;///< 编码前的时间戳(jiffies_64*(1000/HZ))
	unsigned long				ptsAfterEnc;///< 编码后的时间戳(jiffies_64*(1000/HZ))
	unsigned long				ptsLib;		///< 编码库从驱动取到本数据包的时间戳(jiffies_64*(1000/HZ))
	int					reserved[16];///< 保留字节
} VideoEncPacket;






// 视频编码接口，494字节
typedef struct VideoEncoder
{
	/// 底层私有数据
	void *priv;


	/// 增加接口引用
	int (*addRef)(struct VideoEncoder *thiz);


	/// 释放接口
	int (*release)(struct VideoEncoder *thiz);


	/// 得到编码器所在的芯片ID
	///
	/// \param [in] thiz 视频编码接口指针
	/// \retval >=0 芯片ID，由库里面统一分配，不同的芯片有唯一的编号。
	///					芯片号在创建编码通道的时候得到
	/// \retval < 获取失败
	int (*getChipId)(struct VideoEncoder *thiz);


	/// 获得芯片的编码能力
	///
	/// \param [in] thiz 视频编码接口指针
	/// \param [out] caps 芯片能力结构VideoEncChipCaps指针
	///
	int (*getChipCaps)(struct VideoEncoder *thiz, VideoEncChipCaps *pcaps);


	/// 得到视频编码设备能力
	/// 
	/// \param [in] thiz 视频编码接口指针
	/// \retval 0 销毁失败
	/// \retval <0 销毁成功
	int (*getCaps)(struct VideoEncoder *thiz, VideoEncCaps *pcaps);


	/// 强制编码器产生I帧。新的I帧应该在下一个数据包VideoEncPacket第一帧。
	/// 
	/// \param [in] thiz 视频编码接口指针
	/// \retval 0  设置成功。
	/// \retval <0  设置失败。
	int (*forceIFrame)(struct VideoEncoder *thiz);


	/// 设置视频编码数据压缩的格式。
	/// 
	/// \param [in] thiz 视频编码接口指针
	/// \param [in] format 指向视频格式结构VideoEncFormat的指针，NULL表示不编码视频。
	/// \retval 0  设置成功
	/// \retval <0  设置失败
	int (*setFormat)(struct VideoEncoder *thiz, VideoEncFormat* pformat);


	/// 开始编码。在视频制式、视频编码选项、图像质量、帧率、码流控制方式、分辨率、
	/// 编码模式等参数被改变时，应用程序会停止视频编码，然后重新开始视频编码。
	/// 
	/// \param [in] thiz 视频编码接口指针
	/// \retval 0  开始成功
	/// \retval <0  开始失败
	int (*start)(struct VideoEncoder *thiz);


	/// 停止编码，当前操作立即返回。
	/// 
	/// \param [in]  thiz 视频编码接口指针
	/// \retval 0  停止成功
	/// \retval <0  停止失败
	int (*stop)(struct VideoEncoder *thiz);




	/// 读取编码数据，读为非阻塞模式；每次最多读取一帧数据，如果帧数据已经分块，
	/// 则分多次传出。如果没有数据，则返回错误。
	/// 
	/// \param [in] thiz 视频编码接口指针
	/// \param [out] packet 编码缓冲结构VideoEncPacket指针。
	/// \return>=0 通道号（在输入通道号为-1时）
	/// \retval 0  读成功
	/// \retval <0  读失败
	int (*getPacket)(struct VideoEncoder *thiz, VideoEncPacket* ppacket);


	/// 释放视频编码数据，需要和VideoEncGetPacket成对调用。
	///
	/// \param [in]  thiz 视频编码接口指针
	int (*releasePacket)(struct VideoEncoder *thiz);


	/// 设置视频编码中视频输入的区域。
	/// 
	/// \param [in] thiz 视频编码接口指针
	/// \param [in] src 视频输入通道号
	/// \param rect 输入在输出中的视频区域，该区域会自动加一个边框，最后设置的区域
	///                 放在视频输出的最顶层。
	/// \retval 0  设置成功
	/// \retval <0  设置失败
	int (*setInputRect)(struct VideoEncoder *thiz, int src, Rect *prect);

	
	/// 应用程序定时调用， 通知编码设备检测自身状态， 如果有错误， 重启设备，同时返回错误。 
	/// 
	/// \param [in] thiz 视频编码接口指针
	/// \retval 0  同步成功
	/// \retval <0  同步失败
	void (*synchronize)(struct VideoEncoder *thiz);




	///< 保留字节
	int	*				reserved[110];
} VideoEncoder;


/// 码流类型
typedef enum VideoEncStream
{
	videoEncStreamMain = 0,	///< 主码流
	videoEncStreamExtra1,	///< 辅码流1
	videoEncStreamExtra2,	///< 辅码流2
	videoEncStreamExtra3,	///< 辅码流3
	videoEncStreamSnapshot,	///< 抓图
	videoEncStreamNumber	///< 种类数
} VideoEncStream;


/// 视频编码接口描述，128字节
typedef struct VideoEncDesc
{
	VideoEncStream	stream;		///< 指定编码码流的类型
	ulong srcs;					///< 视屏输入通道号掩码，每一位对应一个视频输入物理通道
	int reserved[30];			///< 保留
} VideoEncDesc;


/// 创建视频编码设备，编码的对象可能由多个视频输入源组合而成。创建成功后，如果芯片资源
/// 限制，不一定可以正常编码，应用程序需要根据编码能力来计算。
///
/// \param [in] desc  视频编码接口描述结构VideoEncDesc指针
/// \param [out] videoEncoder 视频编码接口指针的指针
/// \retval <0 创建失败
/// \retval 0 创建成功
int createVideoEncoder(VideoEncDesc *desc, VideoEncoder **videoEncoder);


//此接口为暂时的测试接口，从键盘输入相关参数，程序做相应功能的测试处理
int HiSlaveDataProc(void);
/*

int sysinit(void);
*/
#endif

