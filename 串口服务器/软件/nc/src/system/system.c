#include "system.h"

int trd_create(TRD_t *trd_id, void *(*func)(void *))
{
	int ret = -1;

	ret = pthread_create(trd_id, NULL, func, NULL);
	if (ret != 0)
	{
		perror("trd_create: pthread_create");
		return -1;
	}

	ret = pthread_detach(*trd_id);
	if (ret != 0)
	{
		perror("trd_create: pthread_detach");
		return -2;
	}

	return 0;
}


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


