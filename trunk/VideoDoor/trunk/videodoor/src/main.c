//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "globleinc.h"
#include "print.h"
#include "config.h"
#include "iccard.h"
#include "dsp.h"
#include "console.h"
#include "ntp.h"


//////////////////////////////////////////////////////////////////////////
///
///     main
///     @param int argc, char* argv[]
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-17
//////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    tracepoint();
    config_init();
    console_init();
    ntp_init();

    ///门口机
    if (g_conf_info.con_sys.type == OUT_TP)
    {
		sys_print(FUNC, INFO, "out door type!");
        icCardInit();
        dspEncInit();
    }
    ///室内机
    else
    {
		sys_print(FUNC, INFO, "in door type!");
    }

    while (1)
    {
        sleep(10);
    }

}

