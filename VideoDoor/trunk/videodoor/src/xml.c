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
#include "iconv.h"

#include "xml.h"
#include "config.h"
#include "print.h"

xmlDocPtr xml_open(BYTE *file_name)
{
    xmlDocPtr doc;   //定义解析文档指针

    doc = xmlReadFile(CFG_FILE, "utf-8", 256);  //解析文件

    if (doc == NULL )
    {

        errorf("Document not parsed successfully.");

        return NULL;
    }

    return doc;

}

xmlXPathObjectPtr get_nodeset(xmlDocPtr doc, const xmlChar *xpath)
{

    xmlXPathContextPtr context;

    xmlXPathObjectPtr result;

    context = xmlXPathNewContext(doc);

    if (context == NULL)
    {

        printf("context is NULL\n");

        return NULL;

    }

    result = xmlXPathEvalExpression(xpath, context);

    xmlXPathFreeContext(context);

    if (result == NULL)
    {

        printf("xmlXPathEvalExpression return NULL\n");

        return NULL;

    }

    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
    {

        xmlXPathFreeObject(result);

        printf("nodeset is empty\n");

        return NULL;

    }

    return result;

}

xmlXPathObjectPtr find_node(xmlDocPtr doc, xmlChar *xpath)
{
    xmlNodePtr cur;

    xmlChar *value;


    xmlXPathObjectPtr app_result = get_nodeset(doc, xpath);

    if (app_result == NULL)
    {

        printf("app_result is NULL\n");

    }

    return app_result;
}


int aoti_hex(char *p)
{
    int len = strlen(p);
    int i, j = 0;
    int val = 0;

    for (i = len - 1; i >= 0; i--)
    {
        val += (p[i] - '0') * pow(16, j);
        j++;
    }

    return val;
}

int get_xml_val(xmlXPathObjectPtr app_result, const xmlChar * key, const xmlChar *prop, const xmlChar * pval)
{
    xmlChar *value;
    xmlNodePtr cur;
    int i, val = -1;
    xmlNodeSetPtr nodeset ;

    if (app_result)
    {
        nodeset = app_result->nodesetval;

        for (i = 0; i < nodeset->nodeNr; i++)
        {

            cur = nodeset->nodeTab[i];

            cur = cur->xmlChildrenNode;

            while (cur != NULL)
            {

                value = xmlGetProp(cur, prop);

                if (prop != NULL && (xmlStrcmp(pval, value)))
                {
                    xmlFree(value);
                    cur = cur->next;
                    continue;
                }


                xmlFree(value);

                if ((!xmlStrcmp(cur->name, key)))
                {

                    value = xmlNodeGetContent(cur);

                    if (value != NULL)
                    {

                        //printf("val: %s\n\n", (char *)value);
                        if (value[1] == 'x')
                            val = aoti_hex(&value[2]);
                        else
                            val = atoi(value);

                        xmlFree(value);

                        return val;

                    }
                }

                cur = cur->next;
            }
        }
    }

    return val;
}


int get_xml_str(xmlXPathObjectPtr app_result, const xmlChar * key, BYTE *strdst, int len)
{
    xmlChar *value;
    xmlNodePtr cur;
    int i, val = -1;
    xmlNodeSetPtr nodeset ;

    if (strdst == NULL || len <= 0)
        return FALSE;

    if (app_result)
    {
        nodeset = app_result->nodesetval;

        for (i = 0; i < nodeset->nodeNr; i++)
        {

            cur = nodeset->nodeTab[i];

            cur = cur->xmlChildrenNode;

            while (cur != NULL)
            {

                if ((!xmlStrcmp(cur->name, key)))
                {

                    value = xmlNodeGetContent(cur);

                    if (value != NULL)
                    {

                        //printf("val: %s\n\n", (char *)value);

                        memcpy(strdst, value, strlen(value) < len ? strlen(value) : len);

                        xmlFree(value);

                        return TRUE;

                    }
                }

                cur = cur->next;
            }
        }
    }

    return FALSE;

}


void close_node(xmlXPathObjectPtr app_result)
{
    xmlXPathFreeObject (app_result);
}


void xml_close(xmlDocPtr doc)
{
    xmlFreeDoc(doc);
}


int covert(char *desc, char *src, char *input, size_t ilen, char *output, size_t olen)
{
    char **pin = &input;
    char **pout = &output;

    if (input[0] == '\0')
        return -1;

    iconv_t cd = iconv_open(desc, src);

    if (cd == (iconv_t) - 1)
    {
        perror("iconv_open");
        return -1;
    }

    memset(output, 0, olen);

    if (iconv(cd, pin, &ilen, pout, &olen)) return -1;

    iconv_close(cd);
    return 0;
}





int get_xml_sys(CONFIG_SYS* config)
{
    xmlXPathObjectPtr app_result ;
    xmlDocPtr doc;
    char lsbuf[512] = "", buf[512] = "";

    doc = xml_open(CFG_FILE);
    app_result = find_node(doc, XML_SYS);

    if (app_result == NULL)
    {
        xml_close(doc);
        return FALSE;
    }

    config->id = get_xml_val(app_result, "id", NULL, NULL);
    config->type = get_xml_val(app_result, "type", NULL, NULL);
    
    get_xml_str(app_result, "hostname", lsbuf, sizeof(lsbuf));
    covert("GBK", "UTF-8", lsbuf, strlen(lsbuf), config->hostname, 64);



    close_node(app_result);
    xml_close(doc);
}

int get_xml_net(CONFIG_NET* config)
{

}

int update_xml_sys(CONFIG_SYS* config)
{

}

int update_xml_net(CONFIG_NET* config)
{

}

//////////////////////////////////////////////////////////////////////////
///
///     sys_config_add
///     @param *root_node 
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-16
//////////////////////////////////////////////////////////////////////////
void sys_config_add(xmlNodePtr root_node)
{
    xmlNodePtr sys_node = NULL, node1 = NULL;


    //creates a new node, which is "attached" as child node of root_node node.
    sys_node = xmlNewChild(root_node, NULL, BAD_CAST "System", BAD_CAST NULL);
    node1 = xmlNewChild(sys_node, NULL, BAD_CAST "id", BAD_CAST"01");

    node1 = xmlNewChild(sys_node, NULL, BAD_CAST "type", BAD_CAST"02");


    char p[64] = "室内";
    char lsbuf[64] = "";
    covert("UTF-8", "GBK", p, strlen(p), lsbuf, 64);
    node1 = xmlNewChild(sys_node, NULL, BAD_CAST "hostname", BAD_CAST lsbuf);
    strcpy(p, "南向北");
    covert("UTF-8", "GBK", p, strlen(p), lsbuf, 64);
    node1 = xmlNewChild(sys_node, NULL, BAD_CAST "pos_info", BAD_CAST lsbuf);



}

//////////////////////////////////////////////////////////////////////////
///
///     conf_makedefault
///     @param *file_name 
///     @return int 1
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-16
//////////////////////////////////////////////////////////////////////////
int	conf_makedefault(BYTE *file_name)
{
    xmlDocPtr doc = NULL;		/* document pointer */

    xmlNodePtr root_node = NULL;/* node pointers */

    // Creates a new document, a node and set it as a root node

    doc = xmlNewDoc(BAD_CAST "1.0");


    root_node = xmlNewNode(NULL, BAD_CAST "CONFIG");
    xmlDocSetRootElement(doc, root_node);

    sys_config_add(root_node);


    //Dumping document to stdio or file
    xmlSaveFormatFileEnc(file_name, doc, "UTF-8", 1);
    /*free the document */
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 1;

}


