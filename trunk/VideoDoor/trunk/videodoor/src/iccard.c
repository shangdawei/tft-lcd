//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "iccard.h"
#include "trd.h"


static void icCardProcess(void *arg);

//////////////////////////////////////////////////////////////////////////
///
///     icCardInit
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-26
//////////////////////////////////////////////////////////////////////////
void icCardInit(void)
{
    pthread_t trd_iccard;

    
    trd_create(&trd_iccard, (void *)&icCardProcess);

}


//////////////////////////////////////////////////////////////////////////
///
///     icCardProcess
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
static void icCardProcess(void *arg)
{

}
//////////////////////////////////////////////////////////////////////////
///
///     doorNum2Ip
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
DWORD doorNum2Ip(DWORD num)
{
    DWORD ip = 0;


    return ip;
}

