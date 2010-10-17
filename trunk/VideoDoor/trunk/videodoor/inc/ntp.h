//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//



#ifndef _NTP_H_
#define _NTP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "system.h"

#define NTP_SERVER    "time.buptnet.edu.cn"
#define NTP_PORT      123

#define JAN_1970      0x83aa7e80      /* 2208988800 1970 - 1900 in seconds */

/* How to multiply by 4294.967296 quickly (and not quite exactly)
* without using floating point or greater than 32-bit integers.
* If you want to fix the last 12 microseconds of error, add in
* (2911*(x))>>28)
*/
#define NTPFRAC(x) (4294 * (x) + ((1981 * (x))>>11))

/* The reverse of the above, needed if we want to set our microsecond
 * clock (via settimeofday) based on the incoming time in NTP format.
 * Basically exact.
 */
#define USEC(x) (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

struct ntptime 
{
	unsigned int coarse;
	unsigned int fine;
};


typedef struct  tagCONFIG_NTP
{
    BOOL enable;
    DWORD ntp_addr;
    WORD  ntp_port;
    DWORD cycle_min;
    
}CONFIG_NTP;

void ntp_init(void);

#endif

