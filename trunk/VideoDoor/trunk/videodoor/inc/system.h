//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "globleinc.h"
#include "support.h"

/*! \enum
*   \brief compare result
*/
enum COMPARE_RET
{
    EQUAL = 0,  //!< a is equal with b
    LESS  = 1,  //!< a is less than b
    MORE  = 2,  //!< a is more than b
};


DWORD sys_str2ip(BYTE *str);
void sys_ip2str(DWORD ip, BYTE *str);
BYTE* sys_ip2str_static(DWORD ip);
UQWORD system_mscount_get(void);


#endif

