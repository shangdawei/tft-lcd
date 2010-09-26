//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang XXX Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef _SUPPORT_H
#define _SUPPORT_H

//! type redefinition
typedef unsigned char           BYTE;
typedef unsigned short          WORD;
typedef signed int              LONG;
typedef unsigned int            DWORD;
typedef unsigned int            PARAM;
typedef void *                  HANDLE;
typedef BYTE *                  STRING;
typedef signed int              BOOL;
typedef unsigned long long      UQWORD;
typedef long long               SQWORD;

typedef DWORD                   COLORREF;
typedef DWORD                   COLORDEV;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define TRUE    1
#define FALSE   0

#define FUNC    (char*)__FUNCTION__

#define FOR(i, max)         for (i = 0; i < (max); i++)





#endif


