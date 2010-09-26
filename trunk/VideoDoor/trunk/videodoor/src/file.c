//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "file.h"
//////////////////////////////////////////////////////////////////////////
///
///     file_is_existed
///     @param *file_name
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-16
//////////////////////////////////////////////////////////////////////////
int file_is_existed(BYTE *file_name)
{
    return access(file_name, F_OK) == 0;
}

