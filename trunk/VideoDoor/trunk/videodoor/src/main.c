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

    while(1)
    {
        sleep(10);
    }

}

