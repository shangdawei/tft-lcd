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
#include "sys_log.h"
#include "net.h"

COM_STA g_com_status[MAX_NC_PORT];

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
        g_com_status[i].issue = 0;

        cfg_info->conf_com[i].id = i;

        pipe(g_com_status[i].p_fd);
    }

    for (i = 0; i < MAX_NC_PORT; i++)
    {
        pthread_create(&trd_com[i], NULL, (void *)&com_proc, (void *)&cfg_info->conf_com[i]);
        pthread_detach(trd_com[i]);
    }

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
    //...
    while (1)
    {
        //select  com fd   p_fd
        len = read(com_sta->fd, lsbuf, sizeof(lsbuf));

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
            continue ;
        }
        else
        {
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
    for (i = 0; i < cfg_wknet->max_num; i++)
    {
        if (net_sta->conn_status[i].client_conn > 0)
            len = send(net_sta->conn_status[i].client_conn, buf, size, 0);
    }

    //unlock
    return len;
}


void com_reset(int i)
{
    COM_STA* com_sta = &g_com_status[i];
    close(com_sta->fd);
    com_open(i);
}
/*=================================com.c end============================================*/

