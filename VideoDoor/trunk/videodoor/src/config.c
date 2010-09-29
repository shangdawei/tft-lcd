//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "config.h"
#include "xml.h"
#include "file.h"


/* global config var */
CONF_DATA	g_conf_info;


void config_init(void)
{

    if (!file_is_existed(CFG_FILE))
    {
        conf_makedefault(CFG_FILE);
    }


    sys_config_load(TYPE_ALL);
}


int sys_config_load(CONF_TYPE load_type)
{
    CONF_DATA *config = &g_conf_info;

    switch (load_type)
    {
    case TYPE_ALL:
        get_xml_sys(&config->con_sys);
        get_xml_net(&config->con_net);
        break;
    case TYPE_SYS:
        get_xml_sys(&config->con_sys);
        break;
    case TYPE_NET:
        get_xml_net(&config->con_net);
        break;
    default:
        break;
    }

    return 1;
}


int getConfig(CONF_TYPE type, CONF_UNION *config)
{
    switch (type)
    {
    case TYPE_SYS:
        return get_xml_sys(&config->con_sys);
        break;
    case TYPE_NET:
        return get_xml_net(&config->con_net);
        break;
    default:
        return FALSE;
    }
}


int setConfig(CONF_TYPE type, CONF_UNION *config)
{
    switch (type)
    {
    case TYPE_SYS:
        return update_xml_sys(&config->con_sys);
        break;
    case TYPE_NET:
        return update_xml_net(&config->con_net);
        break;
    default:
        return FALSE;
    }
}




