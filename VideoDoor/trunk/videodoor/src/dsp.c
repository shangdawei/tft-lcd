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

}

//////////////////////////////////////////////////////////////////////////
///
///     dspEncInit
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-26
//////////////////////////////////////////////////////////////////////////
static void dsp_vid_process(void *arg)
{

}
