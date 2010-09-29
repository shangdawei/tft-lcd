//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "net.h"
#include "print.h"
#include "itsip.h"
#include "config.h"
#include "log.h"
#include "system.h"
#include "indoor.h"
#include "outdoor.h"

static void net_svr_start(NET_SERV_INFO *serv);

//////////////////////////////////////////////////////////////////////////
///
///     net_init
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
void net_init(NET_CONN_INFO *conn, NET_SERV_INFO *serv)
{

    memset(conn, 0, sizeof(NET_CONN_INFO));
    conn->conn_idx = -1;
    conn->client_conn = -1;
    conn->idle = 0;


    net_svr_start(serv);
}

//////////////////////////////////////////////////////////////////////////
///
///     net_svr_start
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
static void net_svr_start(NET_SERV_INFO *serv)
{
    int flag = 0;
    int ret = -1;
    struct sockaddr_in server_addr;
    int sndlen;

    serv->fd = socket(AF_INET, SOCK_STREAM, 0);

    if (serv->fd == -1)
    {
        perror("net_server_start: socket");
        return;
    }

    // Enable address reuse and keeplive
    flag = ON;
    ret = setsockopt(serv->fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    if (ret == -1)
    {
        perror("net_server_start: setsockopt()");
        return;
    }

    //	Enable keeplive
    ret = setsockopt(serv->fd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag));

    if (ret == -1)
    {
        perror("net_server_start: setsockopt()");
        return;
    }

    sndlen = 16 * 1024;
    ret = setsockopt(serv->fd, SOL_SOCKET, SO_SNDBUF, &sndlen, sizeof(int));

    if (ret == -1)
    {
        perror("net_server_start: setsockopt()");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(serv->port);
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    while (bind(serv->fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("net_server_start: bind");
        //close(g_listen_conn);

        //return;
        sleep(3);
    }

    if (listen(serv->fd, MAX_TCP_CONN) == -1)
    {
        perror("net_server_start: listen");
        close(serv->fd);

        return;
    }

    sys_print(FUNC, INFO, "等待网络连接......\n");
}


//////////////////////////////////////////////////////////////////////////
///
///     net_conn_close
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
void net_conn_close(NET_CONN_INFO *conn_info)
{

    sys_print(FUNC, INFO, "网络连接 <%d> 关闭.\n", conn_info->conn_idx);

    if (conn_info->client_conn > 0)
    {
        close(conn_info->client_conn);

        conn_info->client_conn = -1;
        conn_info->file_type = 0xff;

    }

}


//////////////////////////////////////////////////////////////////////////
///
///     net_conn_recv
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
int net_conn_recv(NET_CONN_INFO *conn_info, void *net_data, DWORD len)
{
    BYTE *p_net_data = NULL;
    int recv_cnt = 0;
    int total_length = 0;
    fd_set readfds;
    int res = -1;
    struct timeval tv;
    UQWORD ms_cnt1 = 0LLU;
    UQWORD ms_cnt2 = 0LLU;
    UQWORD ms_cnt3 = 0LLU;

    if (conn_info->client_conn <= 0)
    {
        return FAILURE;
    }

    p_net_data = net_data;
    total_length = len;

    ms_cnt1 = system_mscount_get();

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(conn_info->client_conn, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 1000;

        res = select(conn_info->client_conn + 1, &readfds, NULL, NULL, &tv);

        if (res < 0)
        {
            /*
             * note by xuw 2007/03/02: for signal interrupt, do not close the socket, only continue.
             */
            if (errno == EINTR)
            {
                continue;
            }

            perror("net_conn_recv:select");
            net_conn_close(conn_info);

            return FAILURE;
        }
        else if (res > 0)
        {
            if (FD_ISSET(conn_info->client_conn, &readfds))
            {
                recv_cnt  = recv(conn_info->client_conn, p_net_data, total_length, MSG_DONTWAIT | MSG_NOSIGNAL);
                ms_cnt1 = system_mscount_get();
            }
        }
        else
        {
            ms_cnt2 = system_mscount_get();

            //If the return value of select() is still Zero after 500ms, Close the socket
            if (ms_cnt2 > ms_cnt1 && ms_cnt2 - ms_cnt1 >= TIMEOUT_CNT)
            {
                net_conn_close(conn_info);
                sys_print(FUNC, ERROR, "客户端超过 %llu毫秒未响应, 必须关闭此次连接\n", ms_cnt2 - ms_cnt1);
                writeLog(LOG_ERROR, "客户端超过 %llu毫秒未响应, 必须关闭此次连接\n", ms_cnt2 - ms_cnt1);
                return FAILURE;
            }

            continue;
        }

        if (recv_cnt == -1)
        {
            perror("net_conn_recv:recv");

            if (errno == ECONNRESET)
            {
                net_conn_close(conn_info);
            }

            return FAILURE;
        }

        if (recv_cnt == 0)
        {
            net_conn_close(conn_info);

            return FAILURE;
        }

        if (total_length - recv_cnt == 0)
        {
            return SUCCESS;
        }
        else
        {
            ms_cnt3 = system_mscount_get();

            //If the return value of select() is still Zero after 500ms, Close the socket
            if (ms_cnt3 > ms_cnt1 && ms_cnt3 - ms_cnt1 >= TIMEOUT_CNT)
            {
                //We must close this socket and return FAILURE when the client is in problem.
                //Otherwise system will stop to capture picture and video
                net_conn_close(conn_info);
                sys_print(FUNC, ERROR, "客户端发送错误，必须关闭此次连接\n");
                return FAILURE;
            }

            p_net_data += recv_cnt;
            total_length -= recv_cnt;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
///
///     net_cmd_proc
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
static void net_cmd_proc(ITSIP *p_net_head, NET_CONN_INFO *conn_info)
{
    if (conn_info->client_conn < 1)
    {
        return;
    }
    
    if (p_net_head->itsip_head == ITS_HEAD)
    {
        switch(p_net_head->itsip_cmd)
        {
        case ITS_DOOR_ASK:
            sys_print(FUNC, INFO, "==================CMD is door ask===================\n");
            doorAsk(conn_info);
            //doorAskAck(conn_info);
            break;
        case ITS_DOOR_OPEN:
            doorOpen();
            sys_print(FUNC, INFO, "==================CMD is door open===================\n");
            break;
        default:
            sys_print(FUNC, WARN, "==================CMD is unknown[0x%.2x]===================\n", p_net_head->itsip_cmd);
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////////
///
///     net_svr_proc
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
static void net_svr_proc(NET_CONN_INFO *conn_info)
{
    int net_recv_ret = -1;
    ITSIP net_data;

    if (conn_info->client_conn < 1)
    {
        return;
    }

    ///本机为indoor，处理outdoor的消息
    ///若本机为outdoor，只要处理音频数据
   // if(conn_info->client_type == OUT_TP)
    {
        memset(&net_data, 0, sizeof(ITSIP));

        if ((net_recv_ret = net_conn_recv(conn_info, &net_data, sizeof(ITSIP))) == FAILURE)
        {
            return;
        }

        net_cmd_proc(&net_data, conn_info);
    }
  //  else    ///outdoor，处理indoor的音频数据
    {

    }

}


//////////////////////////////////////////////////////////////////////////
///
///     net_process
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
void net_process(void* arg)
{
    int i;
    int addr_size = 0;
    int res = 0;
    int client_sock = -1;
    struct timeval tv;
    struct sockaddr_in client_addr;
    static int m_max_i = 0;			//! max client connection number
    static fd_set m_readfds;		//! set of read socket handles
    int m_max_sock = -1;

    NET_INFO* door = (NET_INFO*)arg;
    NET_CONN_INFO *conn = door->conn;
    NET_SERV_INFO *serv = door->serv;

//    while (1)
    {
        FD_ZERO(&m_readfds);

        if (serv->fd > 0)
        {
            FD_SET(serv->fd, &m_readfds);
            m_max_sock = serv->fd;
        }

        for ( i = 0; i < MAX_TCP_CONN; i++)
        {
            if (conn[i].client_conn < 0)
            {
                continue;
            }

            FD_SET(conn[i].client_conn, &m_readfds);
            m_max_sock = m_max_sock > conn[i].client_conn ? m_max_sock : conn[i].client_conn;
        }

        tv.tv_sec = 0;
        tv.tv_usec = 10000;

        //res = select(m_max_sock + 1, &m_readfds, NULL, NULL, &tv);
        res = select(m_max_sock + 1, &m_readfds, NULL, NULL, NULL);

        if (res == 0)
        {
            return;
        }
        else if (res < 0)
        {
            if (errno == EINTR)
            {
                return;
            }

            perror("server_process: select");

            //Close all client connections
            for (i = 0; i < MAX_TCP_CONN; i++)
            {
                if (conn[i].client_conn > 0)
                {
                    net_conn_close(&conn[i]);
                }
            }

            return;
        }
        else if (res > 0)
        {
            if (FD_ISSET(serv->fd, &m_readfds))
            {
                addr_size = sizeof(client_addr);
                client_sock = accept(serv->fd, (struct sockaddr *) & client_addr, &addr_size);
                sys_print(FUNC, INFO, "++++++++++++++++++++新连接进入 %s.++++++++++++++++++++\n", inet_ntoa(client_addr.sin_addr));

                if (client_sock > 0)
                {
                    for (i = 0; i < MAX_TCP_CONN; i++)
                    {
                        if (conn[i].client_conn < 0)
                        {
                            conn[i].conn_idx = i;
                            conn[i].client_conn = client_sock;
                            conn[i].idle = 0;
                            strcpy(conn[i].clientip, inet_ntoa(client_addr.sin_addr));


                            if (i > m_max_i)
                            {
                                m_max_i = i;
                            }

                            break;
                        }
                    }

                    if (i == MAX_TCP_CONN)
                    {
                        close(client_sock);
                        client_sock = -1;
                    }
                }

                if (client_sock > m_max_sock)
                {
                    m_max_sock = client_sock;
                }
            }

            for (i = 0; i <= m_max_i; i++)
            {
                if (conn[i].client_conn < 0)
                {
                    continue;
                }

                if (FD_ISSET(conn[i].client_conn, &m_readfds))
                {
                    conn[i].idle = 0;
                    net_svr_proc(&conn[i]);
                }
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
///
///     socket_cli_init
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-27
//////////////////////////////////////////////////////////////////////////
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

    sys_print(FUNC, INFO, "it will connect to serv!\n");

    if ( connect( fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0 )
    {
        perror("connect timeout");
        writeLog(LOG_ERROR, "连接服务器失败[%s]\n", sys_ip2str_static(ip));
        close(fd);
        return -1;
    }

    sys_print(FUNC, INFO, " connect to serv ok!\n");
    
    writeLog(LOG_INFO, "连接服务器成功[%s]\n", sys_ip2str_static(ip));
    return fd;

}

