//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _XML_H
#define _XML_H

#include "config.h"


/*cfg file path*/
#define PATH	//"/mnt/cfg/"
#define	CFG_FILE	PATH"itscfg.xml"


#define	XML_ROOT	"/CONFIG"
#define	XML_SYS		XML_ROOT"/System"
#define XML_NET     XML_ROOT"/Net"


int get_xml_sys(CONFIG_SYS* config);

int get_xml_net(CONFIG_NET* config);

int update_xml_sys(CONFIG_SYS* config);

int update_xml_net(CONFIG_NET* config);
//////////////////////////////////////////////////////////////////////////
///
///     conf_makedefault
///     @param *file_name
///     @return int 1
///     @author     xuliang<gxuliang@gmail.com>
///     @date       2010-09-16
//////////////////////////////////////////////////////////////////////////
int	conf_makedefault(BYTE *file_name);

#endif

