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

    int i = 0;
	int ret = 0;
	int iChannel = 0;

	//创建视频编码设备(通道)，
	memset(&encdesc, 0, sizeof(VideoEncDesc));
	encdesc.stream = videoEncStreamMain;
	encdesc.srcs = ((i==0)?(0):(0x01<<(i-1)));
	ret = createVideoEncoder(&encdesc, &videoEncoder);
	if(ret == 0)
	{
		sys_print(FUNC, INFO, "hi3520_dec_init is suceful ########\n");
	}
	else
	{
		sys_print(FUNC, ERROR, "hi3520_dec_init is failt@@@@@@@@@@\n");
		return;
	}

	ret = videoEncoder[iChannel].addRef(&videoEncoder[iChannel]);
	if(ret < 0)
	{
		sys_print(FUNC, ERROR, "增加接口失败\n");
	}
	
	
	
    ///开启编码
    ret = videoEncoder[iChannel].start(&videoEncoder[iChannel]);
	if(ret < 0)
	{
		sys_print(FUNC, ERROR, "编码失败!\n");
	}
	else
	{
    	trd_create(&trd_dsp, (void *)&dsp_vid_process);
	}

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
	
	tracepoint();
    while(1)
    {
        int ret = videoEncoder[0].getPacket(&videoEncoder[0], &packet);
        if(ret == 0)///读取成功
        {
			tracepoint();
        }
    }
}
