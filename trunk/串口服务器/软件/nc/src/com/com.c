/**
* @file com.c
* @brief	:
* @author 	:xul <xu_liang@dahuatech.com>
* @version 1
* @date	:	 2010-01-27
*/
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "com.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/ioctl.h>
#include "sys_log.h"
#include "net.h"
#include "thread.h"
#include <string.h>

COM_STA g_com_status[MAX_NC_PORT];

static LOCK_t net_wr_lock[MAX_NC_PORT];

static int com_open(int index);
static int write_to_net(int index, const char * buf, int size);
static void com_proc(void *arg);


/* ==============================================================================================*/
/**
* @brief	:	com_start
*/
/* ==============================================================================================*/
void com_start(void)
{
    pthread_t trd_com[MAX_NC_PORT];
    int i;
    CONF_DATA *cfg_info = g_conf_info;

    for (i = 0; i < MAX_NC_PORT; i++)
    {
        g_com_status[i].id = i;
        g_com_status[i].fd = -1;
        g_com_status[i].issue[0] = g_com_status[i].issue[1] = 0;
        trd_lock_init(&net_wr_lock[i]);
        cfg_info->conf_com[i].id = i;

//        pipe(g_com_status[i].p_fd);
    }

    for (i = 0; i < MAX_NC_PORT; i++)
    {
        pthread_create(&trd_com[i], NULL, (void *)&com_proc, (void *)&cfg_info->conf_com[i]);
        pthread_detach(trd_com[i]);
    }

}


/* ==============================================================================================*/
/** 
* @brief	:	setTTYSpeed 
* 
* @param	:	fd
* @param	:	speed
* 
* @return	:	
*/
/* ==============================================================================================*/
static int setTTYSpeed(int fd, int speed)
{
    if (fd <= 0)
        return -1;

    struct termios ntm;
    bzero(&ntm, sizeof(ntm));
    tcgetattr(fd, &ntm);

    switch (speed)
    {
    case 1200:
        ntm.c_cflag = B1200;
        break;
    case 2400:
        ntm.c_cflag = B2400;
        break;
    case 4800:
        ntm.c_cflag = B4800;
        break;
    case 9600:
        ntm.c_cflag = B9600;
        break;
    case 19200:
        ntm.c_cflag = B19200;
        break;
    case 38400:
        ntm.c_cflag = B38400;
        break;
    case 57600:
        ntm.c_cflag = B57600;
        break;
    case 115200:
        ntm.c_cflag = B115200;
        break;
    default:
        break;
    }
    tcsetattr(fd, TCSANOW, &ntm);
    return 1;
}

/* ==============================================================================================*/
/** 
* @brief	:	setTTYParity 
* 
* @param	:	fd
* @param	:	databits
* @param	:	parity
* @param	:	stopbits
* 
* @return	:	
*/
/* ==============================================================================================*/
static int setTTYParity(int fd, int databits, int parity, int stopbits)
{
    if (fd <= 0)
        return -1;

    struct termios ntm;
    bzero(&ntm, sizeof(ntm));
    tcgetattr(fd, &ntm);
	printf("databits = %d\n", databits);
    switch (databits)
    {
    case 5:
        ntm.c_iflag = 5;
        break;
    case 6:
        ntm.c_iflag = 6;
        break;
    case 7:
        ntm.c_iflag = 7;
        break;
    case 8:
        ntm.c_iflag = 8;
        break;
    default:
        printf("com_params_set: data bits value[%d] is illegal!\n", databits);
        return -1;
    }
    
    switch (parity)
    {
    case 3:
        ntm.c_oflag = 0;
        break;
    case 2://Ææ
        ntm.c_oflag = 1;
        break;
    case 1://Å¼
        ntm.c_oflag = 2;
        break;
    default:
        printf("com_params_set: parity value[%d] is illegal!\n", parity);
        return -2;
    }
    
    //set stop bits
    switch (stopbits)
    {
    case 1:
        ntm.c_lflag = 2;
        break;
    case 2:
        ntm.c_lflag = 2;
        break;
    default:
        printf("com_params_set: stop bits value[%d] is illegal!\n", stopbits);
        return -3;
    }
    
    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd, TCSANOW, &ntm) != 0)
    {
        perror("com_params_set: tcsetattr");
        return -4;
    }
    
    return 1;

}

/* ==============================================================================================*/
/**
* @brief	:	com_proc
*
* @param	:	arg
*/
/* ==============================================================================================*/
static void com_proc(void *arg)
{
    CONFIG_COM *cfg_com = (CONFIG_COM*)arg;
    COM_STA* com_sta = &g_com_status[cfg_com->id];
    NET_STA* net_sta = &g_net_status[cfg_com->id];
    int len;
    char lsbuf[1024];

    //open com
    com_sta->fd = com_open(com_sta->id);

    if (com_sta->fd <= 0)
    {
        perror("com_open");
        return ;
    }

    //set attr
    setTTYSpeed(com_sta->fd, cfg_com->bps);
    setTTYParity(com_sta->fd, cfg_com->dbit, cfg_com->chk, cfg_com->sbit);

    while (1)
    {
        //select  com fd   p_fd
		memset(lsbuf, 0, sizeof(lsbuf));
        len = read(com_sta->fd, lsbuf, sizeof(lsbuf));
		printf("len = %d\n", len);
        if (len == 0)
        {
            sys_log(MOD_COM, LOG_MSG, "read return 0");
            continue;
        }
        else if (len < 0)
        {
            perror("read");
            close(com_sta->fd);
            com_sta->fd = -1;
           	return ; 
        }
        else
        {
            com_sta->issue[0] += len;
			printf("lsbuf is [%s]\n", lsbuf);
            write_to_net(cfg_com->id, lsbuf, len);
        }
    }
}

/* ==============================================================================================*/
/**
* @brief	:	com_open
*
* @param	:	index
*
* @return	:
*/
/* ==============================================================================================*/
static int com_open(int index)
{
    int fd;
    char name[32] = "";

    sprintf(name, "%s%d", TTY_DEV, index);
    fd = open(name, O_RDWR);
    return fd;
}


/* ==============================================================================================*/
/**
* @brief	:	write_to_net
*
* @param	:	index
* @param	:	buf
* @param	:	size
*
* @return	:
*/
/* ==============================================================================================*/
static int write_to_net(int index, const char * buf, int size)
{
    NET_STA* net_sta = &g_net_status[index];
    CONFIG_WKNET* cfg_wknet = &g_conf_info->conf_wknet[index];
    int len;
    int i;

    //lock
    pthread_mutex_lock(&net_wr_lock[index]);

    for (i = 0; i < cfg_wknet->max_num; i++)
    {
        if (net_sta->fd[i] > 0)
        {
            if (cfg_wknet->session[i].protocol == P_UDP)
            {
                struct sockaddr_in dst_addr;
                bzero(&dst_addr, sizeof(dst_addr));
                dst_addr.sin_family = AF_INET;
                dst_addr.sin_port = htons(cfg_wknet->session[index].port);
                dst_addr.sin_addr.s_addr = cfg_wknet->session[index].ip;
                len = sendto(net_sta->fd[i], buf, size, 0, (struct sockaddr *) & dst_addr, sizeof(dst_addr));
            }
            else
            {
                len = send(net_sta->fd[i], buf, size, 0);
            }
        }
    }

    //unlock
    pthread_mutex_unlock(&net_wr_lock[index]);
    return len;
}


/* ==============================================================================================*/
/** 
* @brief	:	com_reset 
* 
* @param	:	i
*/
/* ==============================================================================================*/
void com_reset(int i)
{
    COM_STA* com_sta = &g_com_status[i];
    int ret = close(com_sta->fd);
	printf("ret = %d\n", ret);
    com_sta->fd = com_open(i);
    
    CONFIG_COM *cfg_com = &g_conf_info->conf_com[i];
	printf("[%d] bps = %d, dbit = %d, chk = %d, sbit = %d\n",
		i, cfg_com->bps, cfg_com->dbit, cfg_com->chk, cfg_com->sbit);
    setTTYSpeed(com_sta->fd, cfg_com->bps);
    setTTYParity(com_sta->fd, cfg_com->dbit, cfg_com->chk, cfg_com->sbit);
}
/*=================================com.c end============================================*/

