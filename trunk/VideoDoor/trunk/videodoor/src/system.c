//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "system.h"

DWORD sys_str2ip(BYTE *str)
{
	BYTE *p = str;
	BYTE cIp;
	DWORD ip = 0;
	signed char j = 0;

	cIp = (BYTE)atoi(p);
	ip += (((DWORD)(cIp)) << (j * 8)); 
	j++;

	while ((*p != 0) && (j >= 0))
	{
		if (*p != '.')
		{
			p++;
			continue;
		}

		cIp = (BYTE)atoi(++p);
		ip += (((DWORD)(cIp)) << (j * 8));  
		j++;
	}
	return ip;
}

void sys_ip2str(DWORD ip, BYTE *str)
{
	BYTE i;
	BYTE cIp[4];
	DWORD iIp = ip;
	
	for (i = 0; i < 4; i++)
	{
		cIp[i] = (BYTE)( (iIp>>(i*8)) & (0xff) );
	}
	sprintf(str, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
}

