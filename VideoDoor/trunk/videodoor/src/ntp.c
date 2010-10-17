//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//



#include "ntp.h"
#include "print.h"
#include "config.h"
#include "trd.h"

void send_packet(int fd)
{
	unsigned int data[12];
	struct timeval now;
	int ret;
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4 
#define PREC -6

	if (sizeof(data) != 48) 
	{
		fprintf(stderr,"size error\n");
		return;
	}

	memset((char*)data, 0, sizeof(data));
	data[0] = htonl((LI << 30) | (VN << 27) | (MODE << 24) 
	              | (STRATUM << 16) | (POLL << 8) | (PREC & 0xff));
	data[1] = htonl(1<<16);  /* Root Delay (seconds) */
	data[2] = htonl(1<<16);  /* Root Dispersion (seconds) */
	gettimeofday(&now, NULL);
	data[10] = htonl(now.tv_sec + JAN_1970 );//- g_conf_info->conf_ntp.ntp_timezone*3600); /* Transmit Timestamp coarse */
	data[11] = htonl(NTPFRAC(now.tv_usec));  /* Transmit Timestamp fine   */
	{
	struct tm *ltm;
	ltm = localtime(&now.tv_sec);
	//printf("\nsendtime : %.4d_%.2d_%.2d %.2d:%.2d:%.2d %ld\n", 
			//ltm->tm_year + 1900,
			//ltm->tm_mon + 1,
			//ltm->tm_mday,
			//ltm->tm_hour,
			//ltm->tm_min,
			//ltm->tm_sec,
			//now.tv_usec);
	}
	ret = send(fd, data, 48, 0);
	//printf("Send packet to ntp server, ret: %d\n", ret);
}

void get_packet_timestamp(int usd, struct ntptime *udp_arrival_ntp)
{
	struct timeval udp_arrival;

	gettimeofday(&udp_arrival, NULL);
	udp_arrival_ntp->coarse = udp_arrival.tv_sec + JAN_1970;
	udp_arrival_ntp->fine   = NTPFRAC(udp_arrival.tv_usec);
}

void rfc1305print(unsigned int *data, struct ntptime *arrival, struct timeval* tv)
{
/* straight out of RFC-1305 Appendix A */
	int li, vn, mode, stratum, poll, prec;
	int delay, disp, refid;
	struct ntptime reftime, orgtime, rectime, xmttime;
	struct tm *ltm;

#define Data(i) ntohl(((unsigned int *)data)[i])
	li      = Data(0) >> 30 & 0x03;
	vn      = Data(0) >> 27 & 0x07;
	mode    = Data(0) >> 24 & 0x07;
	stratum = Data(0) >> 16 & 0xff;
	poll    = Data(0) >>  8 & 0xff;
	prec    = Data(0)       & 0xff;
	if (prec & 0x80) prec|=0xffffff00;
	delay   = Data(1);
	disp    = Data(2);
	refid   = Data(3);
	reftime.coarse = Data(4);
	reftime.fine   = Data(5);
	orgtime.coarse = Data(6);
	orgtime.fine   = Data(7);
	rectime.coarse = Data(8);
	rectime.fine   = Data(9);
	xmttime.coarse = Data(10);
	xmttime.fine   = Data(11);
#undef Data

	gettimeofday(tv, NULL);
	ltm = localtime(&tv->tv_sec);
    /*
	printf("nowtime : %.4d_%.2d_%.2d %.2d:%.2d:%.2d %ld\n", 
			ltm->tm_year + 1900, 
			ltm->tm_mon + 1, 
			ltm->tm_mday, 
			ltm->tm_hour, 
			ltm->tm_min, 
			ltm->tm_sec,
			tv->tv_usec);
    */
/*
	tv->tv_sec = reftime.coarse - JAN_1970 +g_conf_info->conf_ntp.ntp_timezone*3600;
	tv->tv_usec = USEC(reftime.fine);
	ltm = localtime(&tv->tv_sec);
	printf("reftime : %.4d_%.2d_%.2d %.2d:%.2d:%.2d %ld\n", 
			ltm->tm_year + 1900, 
			ltm->tm_mon + 1, 
			ltm->tm_mday, 
			ltm->tm_hour, 
			ltm->tm_min, 
			ltm->tm_sec,
			tv->tv_usec);
*/
	tv->tv_sec = orgtime.coarse - JAN_1970 ;//+g_conf_info->conf_ntp.ntp_timezone*3600;
	tv->tv_usec = USEC(orgtime.fine);
	ltm = localtime(&tv->tv_sec);
    /*
	printf("orgtime : %.4d_%.2d_%.2d %.2d:%.2d:%.2d %ld\n", 
			ltm->tm_year + 1900, 
			ltm->tm_mon + 1, 
			ltm->tm_mday, 
			ltm->tm_hour, 
			ltm->tm_min, 
			ltm->tm_sec,
			tv->tv_usec);
    */
	tv->tv_sec = rectime.coarse - JAN_1970 + 8*3600;//+g_conf_info->conf_ntp.ntp_timezone*3600;
	tv->tv_usec = USEC(rectime.fine);
	ltm = localtime(&tv->tv_sec);
    /*
	printf("rectime : %.4d_%.2d_%.2d %.2d:%.2d:%.2d %ld\n", 
			ltm->tm_year + 1900, 
			ltm->tm_mon + 1, 
			ltm->tm_mday, 
			ltm->tm_hour, 
			ltm->tm_min, 
			ltm->tm_sec,
			tv->tv_usec);
    */
	tv->tv_sec = xmttime.coarse - JAN_1970+8*3600;//+g_conf_info->conf_ntp.ntp_timezone*3600;
	tv->tv_usec = USEC(xmttime.fine);
	ltm = localtime(&tv->tv_sec);
    /*
	printf("Get ntp server time: %.4d_%.2d_%.2d %.2d:%.2d:%.2d %ld\n", 
			ltm->tm_year + 1900, 
			ltm->tm_mon + 1, 
			ltm->tm_mday, 
			ltm->tm_hour, 
			ltm->tm_min, 
			ltm->tm_sec,
			tv->tv_usec);
    */
}

void set_local_time(struct timeval tv)
{
	/* need root user. */
//	if (0 != getuid() && 0 != geteuid())
//		return;
//
	if (-1 == settimeofday(&tv, NULL))
	{
		perror("settimeofday.");
		return;
	}
/*	if (time_sys2rtc_set() < 0)
    {
		fprintf (stderr, "time_sys2rtc_set: failed\n");
		return;
    }
*/
//	printf("set local time to ntp server's time.\n");
}

static int ntp_stat = -1;

int ntp_sync(char *svr_name, unsigned int svr_ip, unsigned short  ntp_port)
{		
	//int ip = 0;
	int sock;
	struct sockaddr_in addr_dst;
	int addr_len = sizeof(struct sockaddr_in);

	/* create socket. */
//	sock = socket(AF_INET, SOCK_STREAM, 0);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sock)
	{
		perror("create socket");
		return -1;
	}

	/* connect to ntp server. */
	memset(&addr_dst, 0, addr_len);
	addr_dst.sin_family = AF_INET;
	addr_dst.sin_port = htons(ntp_port);

	if(svr_ip == 0 && svr_name != NULL)
	{
		printf("NTP Sever Name: %s\n", svr_name);
		struct hostent* host = gethostbyname(svr_name);
		if (NULL == host)
		{
			perror("gethostbyname");
			close(sock);
			return -1;
		}
		if (4 != host->h_length)
		{
			fprintf(stderr, "host->h_length is not 4!\n");
			close(sock);
			return -1;
		}
		memcpy(&(addr_dst.sin_addr.s_addr), host->h_addr_list[0], 4);
	}
	else		
	{
		addr_dst.sin_addr.s_addr = htonl(svr_ip);
	}

	if (-1 == connect(sock, (struct sockaddr*)&addr_dst, addr_len))
	{
		perror("connect ntp server");
		close(sock);
		ntp_stat = -1;
		return -1;
	}
	sys_print(FUNC, INFO, "Connect with NTP Server at IP<%s>, PORT<%d>\n", svr_ip, ntp_port);
	ntp_stat = 0;
	
	while (1)
	{
		fd_set fds_read;
		struct timeval timeout;
		int ret;

		unsigned int buf[12];
		int len;
		
		struct ntptime arrival_ntp;
		struct timeval newtime;

		FD_ZERO(&fds_read);
		FD_SET(sock, &fds_read);

		timeout.tv_sec = 6;
		timeout.tv_usec = 0;
		ret = select(sock + 1, &fds_read, NULL, NULL, &timeout);
		printf("return of select = %d\n", ret);
		if (-1 == ret)
		{
			if(errno == EINTR)
			{
				continue;
			}
			perror("select");
			close(sock);
			ntp_stat = -1;

			return -1;
		}
		if (0 == ret || !FD_ISSET(sock, &fds_read))
		{
			/* send ntp protocol packet. */
			send_packet(sock);
			usleep(100000);
			continue;
		}

		/* recv ntp server's response. */
		len = recv(sock, buf, sizeof(buf), MSG_WAITALL);
		if (-1 == len)
		{
			perror("recvfrom");
			close(sock);
			ntp_stat = -1;
			return -1;
		}
		if (0 == len)
		{
//			usleep(100000);
//			continue;
			close(sock);
			ntp_stat = -1;
			return -1;
		}

		/* get local timestamp. */
		get_packet_timestamp(sock, &arrival_ntp);
		/* get server's time and print it. */
		rfc1305print(buf, &arrival_ntp, &newtime);
		/* set local time to the server's time, if you're a root user. */
		set_local_time(newtime);
		break;
	}

	close(sock);
	ntp_stat = -1;
	return 0;
}

int ntp_stat_get(void)
{
	return ntp_stat;
}



void ntp_proc_trd_start(void)
{
	char ipstr[32];
    CONFIG_NTP * const pNtp = &g_conf_info.con_ntp;
    
	while(1)
	{
        
        if(pNtp->cycle_min< 1)
        {
            pNtp->cycle_min = 1;
        }
		if(pNtp->enable == 1)
		{
			if(pNtp->ntp_addr!= 0)
			{
				memset(ipstr, 0, sizeof(ipstr));
				sys_ip2str(pNtp->ntp_addr, ipstr);
				printf("NTP Server: %s\n", ipstr);
				ntp_sync(NULL, pNtp->ntp_addr, pNtp->ntp_port);
			}
			else
			{
				//ntp_sync(g_conf_info->conf_ntp.ntp_name, 0);
				//printf("NTP Server: %s\n", ipstr);
			}
		}
		sleep(pNtp->cycle_min*60);
	}
}

//////////////////////////////////////////////////////////////////////////
///
///     ntp_init
///		@author xuliang<xu_liang@dahuatech.com>
///		@date	2010Äê10ÔÂ08ÈÕ
//////////////////////////////////////////////////////////////////////////
void ntp_init(void)
{
    pthread_t trd_ntp;
    trd_create(&trd_ntp, (void *)&ntp_proc_trd_start);
}

