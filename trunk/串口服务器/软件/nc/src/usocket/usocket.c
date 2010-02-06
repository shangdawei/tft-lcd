/**
* @file usocket.c
* @brief	:
* @author 	:xul <xu_liang@dahuatech.com>
* @version 1
* @date	:	 2010-01-27
*/
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "file.h"
#include "system.h"
#include "sys_log.h"
#include "usocket.h"
#include "conf.h"
#include "thread.h"
#include "com.h"

#define STALE   30  /* client's name can't be older than this (sec) */
#define offsetof1(TYPE, MEMBER) ((int)&((TYPE *)0)->MEMBER)

static int net_cmd_proc(NET_CONN_INFO *conn_info);
static int net_conn_recv(int fd, void *net_data, int len);
void net_conf_query_ack(BYTE conf_type, CONF_INFO *conf_data, NET_CONN_INFO *conn_info);
void net_conf_set_ack(BYTE conf_type, int ret, NET_CONN_INFO *conn_info);
void net_issue_get(NET_CONN_INFO *conn_info);

inline void itsip_pack(ITS_CMD cmd, int extlen, void *data, ITSIP_PACKET *its_pkt);

/*
 * Wait for a client connection to arrive, and accept it.
 * We also obtain the client's user ID from the pathname
 * that it must bind before calling us.
 * Returns new fd if all OK, <0 on error
 */
int
serv_accept(int listenfd, uid_t *uidptr)
{
    int                 clifd, len, err, rval;
    time_t              staletime;
    struct sockaddr_un  un;
    struct stat         statbuf;

    len = sizeof(un);

    if ((clifd = accept(listenfd, (struct sockaddr *) & un, (socklen_t*) & len)) < 0)
        return(-1);     /* often errno=EINTR, if signal caught */

    /* obtain the client's uid from its calling address */
    len -= offsetof1(struct sockaddr_un, sun_path); /* len of pathname */
    un.sun_path[len] = 0;           /* null terminate */

    if (stat(un.sun_path, &statbuf) < 0)
    {
        rval = -2;
        goto errout;
    }

#ifdef S_ISSOCK     /* not defined for SVR4 */

    if (S_ISSOCK(statbuf.st_mode) == 0)
    {
        rval = -3;      /* not a socket */
        goto errout;
    }

#endif

    if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) ||
            (statbuf.st_mode & S_IRWXU) != S_IRWXU)
    {
        rval = -4;     /* is not rwx------ */
        goto errout;
    }

    staletime = time(NULL) - STALE;

    if (statbuf.st_atime < staletime ||
            statbuf.st_ctime < staletime ||
            statbuf.st_mtime < staletime)
    {
        rval = -5;    /* i-node is too old */
        goto errout;
    }

    if (uidptr != NULL)
        *uidptr = statbuf.st_uid;   /* return uid of caller */

    unlink(un.sun_path);        /* we're done with pathname now */
    return(clifd);

errout:
    err = errno;
    close(clifd);
    errno = err;
    return(rval);
}


#define QLEN 10

/*
 * Create a server endpoint of a connection.
 * Returns fd if all OK, <0 on error.
 */
int
serv_listen(const char *name)
{
    int                 fd, len, err, rval;
    struct sockaddr_un  un;

    /* create a UNIX domain stream socket */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return(-1);

    unlink(name);   /* in case it already exists */

    /* fill in socket address structure */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof1(struct sockaddr_un, sun_path) + strlen(name);

    /* bind the name to the descriptor */
    if (bind(fd, (struct sockaddr *)&un, len) < 0)
    {
        rval = -2;
        goto errout;
    }

    if (listen(fd, QLEN) < 0)   /* tell kernel we're a server */
    {
        rval = -3;
        goto errout;
    }

    return(fd);

errout:
    err = errno;
    close(fd);
    errno = err;
    return(rval);
}


/* ==============================================================================================*/
/**
* @brief	:	usocket_process
*/
/* ==============================================================================================*/
static void usocket_process(void)
{
    int fd;
    int cli_fd[16];
    char buf[16] = "";
    int len, i;
    fd_set fds;
    int max_fd = 0;
    NET_CONN_INFO conn_info;
    memset(cli_fd, 0, sizeof(cli_fd));
    fd = serv_listen(USOCKET_FILE);

    sys_log(MOD_USOCKET, LOG_MSG, "%s start!", __FUNCTION__);

    while (1)
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        max_fd = max_fd > fd ? max_fd : fd;

        for (i = 0; i < 16; i++)
        {
            if (cli_fd[i] > 0)
            {
                FD_SET(cli_fd[i], &fds);
                max_fd = max_fd > cli_fd[i] ? max_fd : cli_fd[i];
            }
        }

        max_fd ++;
        int ret = select(max_fd, &fds, NULL, NULL, NULL);

        if (ret < 0)
        {
            perror("susocket_proces select");
            return;
        }

        if (FD_ISSET(fd, &fds))
        {
            for (i = 0; i < 16; i++)
            {
                if (cli_fd[i] == 0)
                    break;
            }

            if (i == 16) continue;

            cli_fd[i] = serv_accept(fd, NULL);
        }
        else
        {
            for (i = 0; i < 16; i++)
            {
                if (cli_fd[i] == 0) continue;

                if (FD_ISSET(cli_fd[i], &fds))
                {
                    /*
                    len = read(cli_fd[i], buf, 16);
                    if(len <= 0)
                    {
                    	close(cli_fd[i]);
                       	cli_fd[i] = 0;
                    	continue;
                    }
                    printf("%s\n", buf);
                    */
                    printf("read select!\n");
                    conn_info.fd = cli_fd[i];

                    if (net_cmd_proc(&conn_info) == 2)
                    {
                        printf("socket [%d] is closed!\n", cli_fd[i]);
                        close(cli_fd[i]);
                        cli_fd[i] = 0;
                    }
                }
            }

        }

        /*
        for(i = 0; i < 16; i++)
        {
        	printf("cli_fd[%d] = %d\n", i, cli_fd[i]);
        }
        */
    }

    exit(1);
}


/* ==============================================================================================*/
/**
* @brief	:	usocket_start
*/
/* ==============================================================================================*/
void usocket_start(void)
{
    sys_log(MOD_USOCKET, LOG_MSG, "%s start!", __FUNCTION__);
    TRD_t trd_usocket;
    trd_create(&trd_usocket, (void *)&usocket_process);
}

/* ==============================================================================================*/
/**
* @brief	:	net_cmd_proc
*
* @param	:	conn_info
*
* @return	:
*/
/* ==============================================================================================*/
static int net_cmd_proc(NET_CONN_INFO *conn_info)
{
    int len;
    ITSIP net_data;
    CONF_INFO conf_info;
    int ret , i;

    if ((ret = net_conn_recv(conn_info->fd, &net_data, sizeof(ITSIP))) == FAILURE)
    {
        sys_log(MOD_USOCKET, LOG_ERR, "%s: recv ITSIP failed!", __FUNCTION__);
        return 0;
    }
    else if (ret == CLOSED)
        return 2;

    switch (net_data.itsip_cmd)
    {
    case ITS_CONF_QUERY://查询
        printf("---[%d]\n", net_data.itsip_data[0]);
        sys_conf_get(net_data.itsip_data[0], CONF_USR_SET, &conf_info);
        net_conf_query_ack(net_data.itsip_data[0], &conf_info, conn_info);
        printf("[%s]\n", &conf_info.conf_data);
        break;
    case ITS_CONF_SET://设置
        printf("net_data.itsip_extlen = %d\n", net_data.itsip_extlen);

        if (net_conn_recv(conn_info->fd, (char*)&(conf_info.conf_data), net_data.itsip_extlen))
            ret = 1;
        else
            ret = sys_conf_set(net_data.itsip_data[0], CONF_USR_SET, &conf_info);

        net_conf_set_ack(net_data.itsip_data[0], ret, conn_info);
        sys_conf_save(CONF_USR_SET);
        printf("ret = %d\n", ret);
        break;
    case ITS_PORT_RESET://重启端口
        printf("ITS_PORT_RESET\n");

        for (i = 0; i < MAX_NC_PORT; i++)
        {
            printf("port[%d] = %d\n", i, net_data.itsip_data[i]);

            if (net_data.itsip_data[i])
            {
                net_reset(i);
                com_reset(i);
            }
        }

        break;
    case ITS_ISSUE_QUERY://流量查询 
        printf("ITS_ISSUE_QUERY\n");
        net_issue_get(conn_info);
        break;
    case ITS_REBOOT_SET://重启设备
        printf("system will reboot!\n");
        system("reboot");
        break;
    default:
        break;
    }

    return 1;
}

/* ==============================================================================================*/
/**
* @brief	:	net_conn_recv
*
* @param	:	fd
* @param	:	net_data
* @param	:	len
*
* @return	:
*/
/* ==============================================================================================*/
static int net_conn_recv(int fd, void *net_data, int len)
{
    int recv_cnt = 1;

    while (len > 0)
    {
        recv_cnt = recv(fd, net_data, len, MSG_DONTWAIT | MSG_NOSIGNAL);

        if (recv_cnt < 0)
        {
            sys_log(MOD_USOCKET, LOG_ERR, "%s: recv ITSIP failed!", __FUNCTION__);
            return CLOSED;
        }
        else if (recv_cnt == 0)
        {
            printf("socket closed!\n");
            return CLOSED;
        }

        len -= recv_cnt;
    }

    return SUCCESS;
}

/* ==============================================================================================*/
/**
* @brief	:	net_conf_query_ack
*
* @param	:	conf_type
* @param	:	conf_data
* @param	:	conn_info
*/
/* ==============================================================================================*/
void net_conf_query_ack(BYTE conf_type, CONF_INFO *conf_data, NET_CONN_INFO *conn_info)
{
    ITSIP_PACKET conf_ack_pkt;
    //printf("[%d]--[%s]\n", conf_data->conf_size, &conf_data->conf_data);
    itsip_pack(ITS_ACK_CONF_QUERY, conf_data->conf_size, &conf_data->conf_data, &conf_ack_pkt);

    if (net_conn_send(conn_info, &conf_ack_pkt, sizeof(ITSIP_PACKET)) == FAILURE)
    {
        sys_log(MOD_USOCKET, LOG_ERR, "sys_conf_get_ack: send ITSIP head error.");
        return;
    }

    printf("[%d]--[%s]\n", conf_data->conf_size, &conf_data->conf_data);
}


/* ==============================================================================================*/
/** 
* @brief	:	net_conf_set_ack 
* 
* @param	:	conf_type
* @param	:	ret
* @param	:	conn_info
*/
/* ==============================================================================================*/
void net_conf_set_ack(BYTE conf_type, int ret, NET_CONN_INFO *conn_info)
{
    ITSIP_PACKET conf_ack_pkt;
    conf_ack_pkt.head.itsip_data[1] = ret;
    itsip_pack(ITS_ACK_CONF_SET, 0, NULL, &conf_ack_pkt);

    if (net_conn_send(conn_info, &conf_ack_pkt, sizeof(ITSIP)) == FAILURE)
    {
        sys_log(MOD_USOCKET, LOG_ERR, "sys_conf_get_ack: send ITSIP head error.");
        return;
    }
}

/* ==============================================================================================*/
/** 
* @brief	:	net_issue_get 
* 
* @param	:	conn_info
*/
/* ==============================================================================================*/
void net_issue_get(NET_CONN_INFO *conn_info)
{
    ITSIP_PACKET conf_ack_pkt;
    UQWORD issue[MAX_NC_PORT][2];
    int i,j;
    for(i = 0; i < MAX_NC_PORT; i++)
    {
        for(j = 0; j < 2; j++)
        {
            issue[i][j] = g_com_status[i].issue[j];
        }
    }
    itsip_pack(ITS_ACK_ISSUE_QUERY, sizeof(issue), issue, &conf_ack_pkt);
    if (net_conn_send(conn_info, &conf_ack_pkt, sizeof(ITSIP_PACKET)) == FAILURE)
    {
        sys_log(MOD_USOCKET, LOG_ERR, "net_issue_get: send ITSIP_PACKET error.");
        return;
    }
}


/* ==============================================================================================*/
/**
* @brief	:	itsip_pack
*
* @param	:	cmd
* @param	:	extlen
* @param	:	data
* @param	:	its_pkt
*/
/* ==============================================================================================*/
inline void itsip_pack(ITS_CMD cmd, int extlen, void *data, ITSIP_PACKET *its_pkt)
{
    memset(its_pkt, 0, sizeof(ITSIP_PACKET));
    its_pkt->head.itsip_head = ITS_HEAD;
    its_pkt->head.itsip_cmd = cmd;
    its_pkt->head.itsip_ver = ITSIP_VERSION;
    its_pkt->head.itsip_thl = ITSIP_THLEN;
    its_pkt->head.itsip_extlen = extlen;

    if (extlen > 0 || data != NULL)
    {
        memcpy(its_pkt->data, data, extlen);
    }
}

