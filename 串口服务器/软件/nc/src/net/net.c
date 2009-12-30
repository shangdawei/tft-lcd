#include "net.h"
#include <sys/types.h>
#include <sys/socket.h>

NET_STA g_net_status[MAX_NC_PORT];

static void com_proc(void *arg);


int net_conn_send(NET_CONN_INFO *conn_info, void *net_data, DWORD len)
{
    BYTE *ptr = net_data;


    send(conn_info->fd, ptr, len, MSG_DONTWAIT | MSG_NOSIGNAL);
}




void net_start(void)
{

    pthread_t trd_com[MAX_NC_PORT];
    int i;
    CONF_DATA *cfg_info = g_conf_info;

    for (i = 0; i < MAX_NC_PORT; i++)
    {
        g_net_status[i].id = i;
        g_net_status[i].fd = -1;
        g_net_status[i].issue = 0;

        cfg_info->conf_wknet[i].id = i;

        pipe(g_net_status[i].p_fd);
    }

    for (i = 0; i < MAX_NC_PORT; i++)
    {
        pthread_create(&trd_com[i], NULL, (void *)&com_proc, (void *)&cfg_info->conf_wknet[i]);
        pthread_detach(trd_com[i]);
    }
}


static void com_proc(void *arg)
{
    CONFIG_WKNET* cfg_wknet = (CONFIG_WKNET*)arg;
    NET_STA* net_sta = &g_net_status[cfg_wknet->id];
}


