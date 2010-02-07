/**
* @file net.c
* @brief	:
* @author 	:xul <xu_liang@dahuatech.com>
* @version 1
* @date	:	 2010-01-27
*/
#include "net.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sys_log.h"
#include "com.h"
#include "thread.h"
#include <errno.h>
#include <string.h>

NET_STA g_net_status[MAX_NC_PORT];


TRD_LST net_trd[MAX_NC_PORT][MAX_SESSION] = {0, };


static LOCK_t com_wr_lock[MAX_NC_PORT];

static void net_proc(void *arg);
static void mode_tcp_srv_proc(void* arg);
static int net_svr_proc(int fd, int index);
static int write_to_com(int index, const char * buf, int size);
static void mode_tcp_cli_proc(void* arg);
static void mode_udp_proc(void* arg);
static int udp_recv(int index, int i);


/* ==============================================================================================*/
/**
* @brief	:	net_conn_send
*
* @param	:	conn_info
* @param	:	net_data
* @param	:	len
*
* @return	:
*/
/* ==============================================================================================*/

int net_conn_send(NET_CONN_INFO *conn_info, void *net_data, DWORD len)
{
    BYTE *ptr = net_data;


    send(conn_info->fd, ptr, len, MSG_DONTWAIT | MSG_NOSIGNAL);
}




/* ==============================================================================================*/
/**
* @brief	:	net_start
*/
/* ==============================================================================================*/
void net_start(void)
{

    pthread_t trd_net[MAX_NC_PORT];
    int i, j;
    CONF_DATA *cfg_info = g_conf_info;

    for (i = 0; i < MAX_NC_PORT; i++)
    {
        g_net_status[i].id = i;

        for (j = 0; j < MAX_SESSION; j++)
        {
            g_net_status[i].fd[j] = -1;
            g_net_status[i].conn_status[j].idx = i;
        }

        g_net_status[i].issue = 0;
        trd_lock_init(&com_wr_lock[i]);
        cfg_info->conf_wknet[i].id = i;

//        pipe(g_net_status[i].p_fd);
    }

    for (i = 0; i < MAX_NC_PORT; i++)
    {
//        pthread_create(&trd_net[i], NULL, (void *)&net_proc, (void *)&cfg_info->conf_wknet[i]);
//        pthread_detach(trd_net[i]);
        net_proc((void *)&cfg_info->conf_wknet[i]);
    }
}


/* ==============================================================================================*/
/**
* @brief	:	net_proc
*
* @param	:	arg
*/
/* ==============================================================================================*/
static void net_proc(void *arg)
{
    CONFIG_WKNET* cfg_wknet = (CONFIG_WKNET*)arg;
    NET_STA* net_sta = &g_net_status[cfg_wknet->id];
//<<<<<<< .mine
    int i;
    int srv_flag = 0, udp_flag = 0;
    pthread_t trd_net_srv;

    printf("%d--%d\n", cfg_wknet->id, cfg_wknet->mode);

    switch (cfg_wknet->mode)
    {
    case MOD_NULL:
        //do sth
        break;
    case MOD_TCP_UPD:

        for (i = 0; i < cfg_wknet->max_num; i++)
        {
            if (cfg_wknet->session[i].protocol == P_TCP_SRV)
            {
                srv_flag ++;

                if (srv_flag == 1)
                {
                    trd_create2(&net_trd[cfg_wknet->id][i], (void*)&mode_tcp_srv_proc, (void *)cfg_wknet, "mode_tcp_srv_proc");
                }
            }
            else if (cfg_wknet->session[i].protocol == P_TCP_CLI)
            {
                net_sta->conn_status[i].conn_idx = i;
                //pthread_create(&trd_net_srv, NULL, (void *)&mode_tcp_cli_proc, (void *)&net_sta->conn_status[i]);
                //pthread_detach(trd_net_srv);
                trd_create2(&net_trd[cfg_wknet->id][i], (void*)&mode_tcp_cli_proc, (void *)&net_sta->conn_status[i], "mode_tcp_cli_proc");
            }
            else if (cfg_wknet->session[i].protocol == P_UDP)
            {
                udp_flag ++;

                if (srv_flag == 1)
                {
                    trd_create2(&net_trd[cfg_wknet->id][i], (void*)&mode_udp_proc, (void *)cfg_wknet, "mode_udp_proc");
                }
            }
        }

        if (srv_flag == 1)
        {
            //pthread_create(&trd_net_srv, NULL, (void *)&mode_tcp_srv_proc, (void *)cfg_wknet);
            //pthread_detach(trd_net_srv);
            //trd_create2(&net_trd[cfg_wknet->id][i], (void*)&mode_tcp_srv_proc, (void *)cfg_wknet, "mode_tcp_srv_proc");
        }

        if (udp_flag == 1)
        {

        }

        break;
    default:
        break;

    }

}


//<<<<<<< .mine
/* ==============================================================================================*/
/** 
* @brief	:	CUdp 
* 
* @param	:	destIP
* @param	:	localPort
* @param	:	destPort
* 
* @return	:	
*/
/* ==============================================================================================*/
int CUdp(unsigned int destIP, unsigned short localPort, unsigned short destPort)
{
    struct sockaddr_in local_addr;
    struct sockaddr_in dest_addr;
    int sock_fd;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(localPort);

    memset(&dest_addr, 0, sizeof(dest_addr));
    /* 协议地址组包 */
    dest_addr.sin_family = AF_INET;                    /* 协议名 */
    dest_addr.sin_addr.s_addr = htonl(destIP);  /* 自动分配地址 */
    dest_addr.sin_port = htons(destPort);                   /* 端口号 */
    /* 组装系统调用socket和bind */

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(strerror(errno));
        return -1;
    }

    /* 创建UDP套接字描述符 */
    if (bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0 )  /* 命名套接字 */
    {
        perror(strerror(errno));
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}





/* ==============================================================================================*/
/**
* @brief	:	socket_cli_init
*
* @param	:	ip
* @param	:	port
*
* @return	:
*/
/* ==============================================================================================*/
int socket_cli_init(DWORD ip, DWORD port)
{
    int fd;
    struct sockaddr_in	addr;
    fd = socket( AF_INET, SOCK_STREAM, 0 );

    if (fd < 0)
    {
        perror("socket");
        return fd;
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family		= AF_INET;
    addr.sin_addr.s_addr	= (ip);
    addr.sin_port		= htons(port);

    //连接目标相机

    printf("it will connect to serv!\n");

    if ( connect( fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0 )
    {
        perror("connect timeout");
        close(fd);
        return -1;
    }

    printf(" connect to serv ok!\n");
    return fd;

}


/* ==============================================================================================*/
/**
* @brief	:	creat_srv_socket
*
* @param	:	addr
*
* @return	:
*/
/* ==============================================================================================*/
int creat_srv_socket(struct sockaddr_in addr)
{
    int fd;
	int flag = 1;
    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0)
    {
        perror("socket");
        return fd;
    }
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    while (bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        sleep(3);
    }
	sys_log(MOD_NET, LOG_MSG, "bind ok!\n");

    if (listen(fd, MAX_SESSION) < 0)
    {
        perror("listen");
        close(fd);
        return -1;
    }

    return fd;
}


/* ==============================================================================================*/
/** 
* @brief	:	close_fd 
* 
* @param	:	arg
*/
/* ==============================================================================================*/
void close_fd(void *arg)
{
    int *fd = (int*)arg;

    if (*fd > 0)
        close(*fd);

    *fd = -1;
}

/* ==============================================================================================*/
/** 
* @brief	:	thread_off 
* 
* @param	:	arg
*/
/* ==============================================================================================*/
void thread_off(void* arg)
{
    TRD_LST *p = (TRD_LST*)arg;
    trdinfo_off(p, "thead is canceled");
}


/* ==============================================================================================*/
/** 
* @brief	:	net_reset 
* 
* @param	:	i
*/
/* ==============================================================================================*/
void net_reset(int i)
{
    int j;
    CONF_DATA *cfg_info = g_conf_info;
    printf("port[%d] reseting...!\n", i);

    for (j = 0; j < MAX_SESSION; j++)
    {
        close_fd(&g_net_status[i].fd[j]);

        if (net_trd[i][j].status == ON)
        {
            trd_cancel(&net_trd[i][j]);
            net_trd[i][j].status = OFF;
        }
    }

    close_fd(&g_net_status[i].fd);
    sleep(2);
    net_proc((void *)&cfg_info->conf_wknet[i]);
}


/* ==============================================================================================*/
/**
* @brief	:	mode_tcp_cli_proc
*
* @param	:	arg
*/
/* ==============================================================================================*/
static void mode_tcp_cli_proc(void* arg)
{
    NET_CONN_INF* net_sta = (NET_CONN_INF*)arg;
    int id = net_sta->idx;
    CONFIG_WKNET* cfg_wknet = &g_conf_info->conf_wknet[id];
    int fd = -1;
    int len;
    char lsbuf[1024];

    trd_cleanup_push(thread_off, &net_trd[id][net_sta->conn_idx]);
    trdinfo_on(&net_trd[id][net_sta->conn_idx]);

    while (1)
    {
        if (fd < 0)
            fd = socket_cli_init(cfg_wknet->session[net_sta->conn_idx].ip, cfg_wknet->session[net_sta->conn_idx].port);

        if (fd < 0)
        {
            sleep(5);
            continue;
        }

        net_sta->client_conn = fd;

        len = recv(fd, lsbuf, sizeof(lsbuf), 0);

        if (len <= 0)
        {
            close(fd);
            net_sta->client_conn = fd = -1;
            continue;
        }
        else
        {
            write_to_com(id, lsbuf, len);
        }
    }

    pthread_exit(NULL);
    trd_cleanup_pop();
    pthread_exit(NULL);

}


/* ==============================================================================================*/
/** 
* @brief	:	mode_udp_proc 
* 
* @param	:	arg
*/
/* ==============================================================================================*/
static void mode_udp_proc(void* arg)
{
    CONFIG_WKNET* cfg_wknet = (CONFIG_WKNET*)arg;
    NET_STA* net_sta = &g_net_status[cfg_wknet->id];
    int i;
    fd_set m_readfds;		//! set of read socket handles
    int max_fds = 0;
    int ret;

    for (i = 0; i < cfg_wknet->max_num; i++)
    {
        if (cfg_wknet->session[i].protocol == P_UDP)
        {
            net_sta->fd[i] = CUdp(cfg_wknet->session[i].ip, cfg_wknet->session[i].port, cfg_wknet->session[i].port);
        }
    }

    while (1)
    {
        FD_ZERO(&m_readfds);

        for (i = 0; i < cfg_wknet->max_num; i++)
        {
            if (cfg_wknet->session[i].protocol == P_UDP)
            {
                FD_SET(net_sta->fd[i], &m_readfds);
                max_fds = net_sta->fd[i];
            }
        }

        ret = select(max_fds + 1, &m_readfds, NULL, NULL, NULL);

        if (ret <= 0)
        {
            for (i = 0; i < cfg_wknet->max_num; i++)
            {
                if (cfg_wknet->session[i].protocol == P_UDP)
                {
                    close_fd(&net_sta->fd[i]);
                }
            }

            return ;
        }
        else
        {
            for (i = 0; i < cfg_wknet->max_num; i++)
            {
                if (cfg_wknet->session[i].protocol == P_UDP && FD_ISSET(net_sta->fd[i], &m_readfds))
                {
                    udp_recv(cfg_wknet->id, i);
                }
            }
        }
    }
}


/* ==============================================================================================*/
/** 
* @brief	:	udp_recv 
* 
* @param	:	index
* @param	:	i
* 
* @return	:	
*/
/* ==============================================================================================*/
static int udp_recv(int index, int i)
{
    NET_STA* net_sta = &g_net_status[index];
    struct sockaddr_in src_addr;
    socklen_t src_len;
    char data[1024];
    int recv_len = recvfrom(net_sta->fd[i], data, 1024, 0, (struct sockaddr *) & src_addr, &src_len);
    return write_to_com(index, data, recv_len);
}

/* ==============================================================================================*/
/**
* @brief	:	mode_tcp_srv_proc
*
* @param	:	arg
*/
/* ==============================================================================================*/
static void mode_tcp_srv_proc(void* arg)
{
    int i;
    CONFIG_WKNET* cfg_wknet = (CONFIG_WKNET*)arg;
    int fd;
    NET_STA* net_sta = &g_net_status[cfg_wknet->id];
    fd_set m_readfds;		//! set of read socket handles
    int max_fds = 0;
    int ret;
    struct sockaddr_in client_addr;
    int addr_size = 0;
    int client_sock = -1;

    trd_cleanup_push(close_fd, &fd);
    /*****do server connection*****/
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(cfg_wknet->lport);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    fd = creat_srv_socket(addr);

    if (fd <= 0)
    {
        perror("creat_srv_socket");
        return ;
    }





    if (listen(fd, MAX_SESSION) < 0)
    {
        perror("mode_tcp_srv_proc:listen");
        close(fd);
        return ;
    }

    max_fds = fd;

    while (1)
    {
        FD_ZERO(&m_readfds);
        FD_SET(fd, &m_readfds);

        for (i = 0; i < cfg_wknet->max_num; i++)
        {
            if (cfg_wknet->session[i].protocol == P_TCP_SRV && net_sta->fd[i] > 0)
            {
                FD_SET(net_sta->fd[i], &m_readfds);
            }
        }

        ret = select(max_fds + 1, &m_readfds, NULL, NULL, NULL);

        if (ret <= 0)
        {
            for (i = 0; i < cfg_wknet->max_num; i++)
            {
                if (cfg_wknet->session[i].protocol == P_TCP_SRV && net_sta->fd[i] > 0)
                {
                    close(net_sta->fd[i]);
                    net_sta->fd[i] = -1;
                }
            }

            close(fd);
            return ;
        }
        else
        {
            if (FD_ISSET(fd, &m_readfds))
            {
                addr_size = sizeof(client_addr);
                client_sock = accept(fd, (struct sockaddr *) & client_addr, (socklen_t*) & addr_size);
                sys_log(MOD_NET, LOG_MSG, "++++++++++++++++++++ New Connection Coming from %s.++++++++++++++++++++\n", inet_ntoa(client_addr.sin_addr));

                for (i = 0; i < cfg_wknet->max_num; i++)
                {
                    if (cfg_wknet->session[i].protocol == P_TCP_SRV && net_sta->fd[i] < 0)
                    {
                        net_sta->fd[i] = client_sock;
                        net_sta->conn_status[i].conn_idx = i;
                        net_sta->conn_status[i].client_conn = client_sock;
                        net_sta->conn_status[i].idle = 0;
                        strcpy(net_sta->conn_status[i].clientip, inet_ntoa(client_addr.sin_addr));
                        break;
                    }
                }

                if (i == cfg_wknet->max_num)
                {
                    sys_log(MOD_NET, LOG_MSG, "reach max connection");
                    close(client_sock);
                }
                else
                {
                    if (client_sock > max_fds)
                    {
                        max_fds = client_sock;
                    }
                }
            }
            else
            {
                for (i = 0; i < cfg_wknet->max_num; i++)
                {
                    if (cfg_wknet->session[i].protocol == P_TCP_SRV && net_sta->fd[i] > 0)
                    {
                        if (FD_ISSET(net_sta->fd[i], &m_readfds))
                        {
                            if (net_svr_proc(net_sta->fd[i], cfg_wknet->id) < 0)
                            {
                                close(net_sta->fd[i]);
                                net_sta->fd[i] = -1;
                            }
                        }
                    }
                }
            }
        }
    }

    pthread_exit(NULL);
    trd_cleanup_pop();
    pthread_exit(NULL);



}


/* ==============================================================================================*/
/**
* @brief	:	net_svr_proc
*
* @param	:	fd
* @param	:	index
*
* @return	:
*/
/* ==============================================================================================*/
static int net_svr_proc(int fd, int index)
{
    int len;
    char lsbuf[1024];

    len = recv(fd, lsbuf, sizeof(lsbuf), 0);

    if (len <= 0)
    {
        return -1;
    }

    return write_to_com(index, lsbuf, len);
}


/* ==============================================================================================*/
/**
* @brief	:	write_to_com
*
* @param	:	index
* @param	:	buf
* @param	:	size
*
* @return	:
*/
/* ==============================================================================================*/
static int write_to_com(int index, const char * buf, int size)
{
    COM_STA* com_sta = &g_com_status[index];
    int len;

    //lock
    pthread_mutex_lock(&com_wr_lock[index]);
    com_sta->issue[1] += size;

    if (com_sta->fd > 0)
        len = write(com_sta->fd, buf, size);

    //unlock
    pthread_mutex_unlock(&com_wr_lock[index]);
    return len;
}


