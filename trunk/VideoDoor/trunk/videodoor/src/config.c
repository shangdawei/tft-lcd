//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "config.h"

void config_init(void)
{

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



int	conf_makedefault(BYTE *file_name)
{
    xmlDocPtr doc = NULL;		/* document pointer */

    xmlNodePtr root_node = NULL;/* node pointers */

    // Creates a new document, a node and set it as a root node

    doc = xmlNewDoc(BAD_CAST "1.0");


    root_node = xmlNewNode(NULL, BAD_CAST "CONFIG");
    xmlDocSetRootElement(doc, root_node);




    //Dumping document to stdio or file
    xmlSaveFormatFileEnc(file_name, doc, "UTF-8", 1);
    /*free the document */
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 1;

}

