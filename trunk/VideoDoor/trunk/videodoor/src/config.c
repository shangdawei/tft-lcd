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

void config_init(void)
{
    conf_makedefault(CFG_FILE);
}


int getConfig(CONF_TYPE type, CONF_UNION *config)
{
    switch(type)
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
    switch(type)
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




