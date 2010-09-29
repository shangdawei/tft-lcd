//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//


#include "dsp.h"
#include "trd.h"
#include "print.h"
#include "hiavenc_inc//hi3510_enc_api.h"


VideoEncDesc encdesc;
VideoEncoder *videoEncoder;


static void dsp_vid_process(void *arg);

//////////////////////////////////////////////////////////////////////////
///
///     dspEncInit
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-26
//////////////////////////////////////////////////////////////////////////
void dspEncInit(void)
{
    pthread_t trd_dsp;
    trd_create(&trd_dsp, (void *)&dsp_vid_process);

    int i = 0;
	int ret = 0;
	VideoEncFormat Vencformat;
	//ret = sysinit();
	
	//创建视频编码设备(通道)，
	memset(&encdesc, 0, sizeof(VideoEncDesc));
	encdesc.stream = videoEncStreamMain;
	encdesc.srcs = ((i==0)?(0):(0x01<<(i-1)));
	ret = createVideoEncoder(&encdesc, &videoEncoder);
	
	//设置视频编码数据压缩的格式。
	Vencformat.type = videoEncH264;
	Vencformat.width = 720;
	Vencformat.height = 576;
	Vencformat.bitrateControl = videoEncBitrateCtrlConstant;
	Vencformat.bitrate = 3500;
	Vencformat.fps = 25;
	Vencformat.gop = 25;
	Vencformat.quality = 4;
	Vencformat.pack = videoEncPackTypePS;
	ret = videoEncoder[0].setFormat(&videoEncoder[0], &Vencformat);
	if(ret < 0)
	{
		sys_print(FUNC, ERROR, "设置视频编码数据压缩格式失败\n");
	}
	

	if(ret == 0)
	{
		sys_print(FUNC, INFO, "hi3520_dec_init is suceful ########\n");
	}
	else
	{
		sys_print(FUNC, ERROR, "hi3520_dec_init is failt@@@@@@@@@@\n");
	}

    ///开启编码
    videoEncoder[0].start(&videoEncoder[0]);

}

//////////////////////////////////////////////////////////////////////////
///
///     dspEncInit
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-26
//////////////////////////////////////////////////////////////////////////
static void dsp_vid_process(void *arg)
{
    VideoEncPacket packet;

    while(1)
    {
        int ret = videoEncoder[0].getPacket(&videoEncoder[0], &packet);
        if(ret == 0)///读取成功
        {

        }
    }
}
