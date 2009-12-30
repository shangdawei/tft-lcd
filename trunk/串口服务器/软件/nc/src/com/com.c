/*
**com.c
*/
#include "com.h"

COM_STA g_com_status[MAX_NC_PORT];


static void com_proc(void *arg);


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

static void com_proc(void *arg)
{
    CONFIG_COM *cfg_com = (CONFIG_COM*)arg;
    COM_STA* com_sta = &g_com_status[cfg_com->id];


    //open com

    while (1)
    {
        //select  com fd   p_fd
    }
}



/*=================================com.c end============================================*/

