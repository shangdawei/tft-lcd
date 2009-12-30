#include <string.h>
#include <stdlib.h>
#include "conf.h"
#include "file.h"
#include "sys_log.h"

#define offsetof(TYPE, MEMBER) ((int)&((TYPE *)0)->MEMBER)
#define conf_offsetof(x)   offsetof(CONF_DATA, x)
CONF_DATA *g_conf_info = NULL;


DWORD conf_size_get(BYTE conf_type)
{
    DWORD conf_size = 0;

    switch (conf_type)
    {
    case CONF_VER:
        conf_size = sizeof(CONFIG_VER);
        break;
    case CONF_SYS:
        conf_size = sizeof(CONFIG_SYS);
        break;
    case CONF_NET:
        conf_size = sizeof(CONFIG_NET);
        break;
    case CONF_NC:
        conf_size = sizeof(CONFIG_NC) * MAX_NC_PORT;
        break;
    case CONF_COM:
        conf_size = sizeof(CONFIG_COM) * MAX_NC_PORT;
        break;
    case CONF_WKNET:
        conf_size = sizeof(CONFIG_WKNET) * MAX_NC_PORT;
        break;
    default:
        break;
    }

    return conf_size;
}


int conf_offset_get(BYTE conf_type)
{
    int offset = -1;

    switch (conf_type)
    {
    case CONF_VER:
        offset = conf_offsetof(conf_ver);
        break;
    case CONF_SYS:
        offset = conf_offsetof(conf_sys);
        break;
    case CONF_NET:
        offset = conf_offsetof(conf_net);
        break;
        //se CONF_NC:
        //ffset = conf_offsetof(conf_nc);
        //eak;
    case CONF_COM:
        offset = conf_offsetof(conf_com);
        break;
    case CONF_WKNET:
        offset = conf_offsetof(conf_wknet);
        break;
    default:
        break;
    }

    return offset;
}

BOOL sys_conf_get(BYTE conf_type, BYTE cfg_set_type, CONF_INFO *conf_info)
{
    int i;
    CONF_DATA *cfg_info = g_conf_info;

    int offset = -1;
    int conf_size = -1;


    conf_size = conf_size_get(conf_type);
    offset = conf_offset_get(conf_type);
    printf("%d   %d\n", conf_size, offset);
    memcpy(&(conf_info->conf_data), (BYTE *)cfg_info + offset, conf_size);
    conf_info->conf_size = conf_size;
    //printf("111%d   %d\n", conf_size, offset);
    return SUCCESS;
}


BOOL sys_conf_set(BYTE conf_type, BYTE cfg_set_type, CONF_INFO *conf_info)
{
    int i;
    CONF_DATA *cfg_info = g_conf_info;

    int offset = -1;
    int conf_size = -1;


    conf_size = conf_size_get(conf_type);
    offset = conf_offset_get(conf_type);
    printf("%d   %d\n", conf_size, offset);
//	printf("ip = %d\n", conf_info->conf_data.conf_net.dev_ip);
    memcpy((BYTE *)cfg_info + offset, &(conf_info->conf_data), conf_size);
    //conf_info->conf_size = conf_size;
    //printf("%d   %d\n", conf_size, offset);

    //sys_conf_save(CONF_USR_SET);
    return SUCCESS;
}



void sys_conf_save(BYTE save_type)
{
    FILE *cfg_fp = NULL;

    if (save_type == CONF_USR_SET)
    {
        cfg_fp = sys_file_open(CFG_FILE, MODE_WRITE);

        if (cfg_fp == NULL)
        {
            printf("sys_conf_save: open %s error!\n", CFG_FILE);
            return;
        }
    }
    else
    {
        cfg_fp = sys_file_open(CFG_FILE, MODE_WRITE);

        if (cfg_fp == NULL)
        {
            printf("sys_conf_save: open %s error!\n", CFG_FILE);
            return;
        }
    }

    sys_file_write(cfg_fp, g_conf_info, sizeof(CONF_DATA));
    sys_file_close(cfg_fp);
}



static void sys_config_load(BYTE load_type)
{

    FILE *cfg_fp = NULL;

    if (load_type == CONF_USR_SET)
    {
        cfg_fp = sys_file_open(CFG_FILE, MODE_READ);
    }
    else
    {
        cfg_fp = sys_file_open(CFG_FILE, MODE_READ);
    }

    if (cfg_fp == NULL)
    {
        sys_log(MOD_CONF, LOG_ERR, "Open configure file error");
        return;
    }

    memset(g_conf_info, 0, sizeof(CONF_DATA));
    sys_file_read(cfg_fp, g_conf_info, sizeof(CONF_DATA));
    sys_file_close(cfg_fp);

}


void conf_init(void)
{
    g_conf_info = malloc(sizeof(CONF_DATA));

    if (g_conf_info == NULL)
    {
        printf("Malloc %d bytes for Configure data error!\n", sizeof(CONF_DATA));
        return;
    }

//	memcpy(g_conf_info->conf_ver.cfg_ver, __DATE__, sizeof(__DATE__));
//	printf("[%s]\n", g_conf_info->conf_ver.cfg_ver);
    if (!file_is_existed(CFG_FILE))
    {
        sys_log(MOD_CONF, LOG_MSG, "make defalt config file!");
        make_default_file();
    }

    sys_config_load(CONF_USR_SET);
}




void conf_ver(void)
{
    CONF_DATA *cfg_info = g_conf_info;
    sprintf(cfg_info->conf_ver.cfg_buid, "%s %s", __DATE__, __TIME__);
    memcpy(cfg_info->conf_ver.cfg_ver, DEV_VER, sizeof(cfg_info->conf_ver.cfg_ver));
}


void conf_sys(void)
{
    CONF_DATA *cfg_info = g_conf_info;
    strcpy(cfg_info->conf_sys.dev_name, "NC600");
    cfg_info->conf_sys.dev_type = 0x01;
    strcpy(cfg_info->conf_sys.dev_sn, "12345678");
}

void conf_net(void)
{
    CONF_DATA *cfg_info = g_conf_info;
    cfg_info->conf_net.dev_ip = sys_str2ip("192.168.1.200");
    cfg_info->conf_net.dev_nm = sys_str2ip("255.255.0.0");
    cfg_info->conf_net.dev_gw = sys_str2ip("192.168.1.1");
}

void conf_nc(void)
{
    int i;
    CONF_DATA *cfg_info = g_conf_info;

    for (i = 0; i < MAX_NC_PORT; i++)
    {
        cfg_info->conf_com[i].bps = 19200;
        cfg_info->conf_com[i].dbit = 8;
        cfg_info->conf_com[i].chk = 0;
        cfg_info->conf_com[i].sbit = 1;
        cfg_info->conf_com[i].DTR = 0;
        cfg_info->conf_com[i].RTS = 0;

        cfg_info->conf_wknet[i].mode = 1;
        cfg_info->conf_wknet[i].lport = 0;
    }

}

void make_default_file(void)
{
    conf_ver();
    conf_sys();
    conf_net();
    conf_nc();
    sys_conf_save(CONF_DFT_SET);
}


