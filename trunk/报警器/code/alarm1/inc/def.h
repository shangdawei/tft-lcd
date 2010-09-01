/*
**  ******************************************************************************************
**                                      ALARM
**         (c) Copyright 2009-2010, XXXX CO.LTD.
**                                    All Rights Reserved
**  ==========================================================================================
**	File		: def.h
**  ==========================================================================================
**	Description	: Application enter pointer
**  ==========================================================================================
**  History     : Modify by  ||    ID    ||     Date      ||     Contents
**              :   xul      ||          ||   2009/09/14  || Create this file
**  ******************************************************************************************
*/
/// "$Id$"
#ifndef   _DEF_H_
#define   _DEF_H_
// Include I/O definition file
#define  ENABLE_BIT_DEFINITIONS //don't del this line
#include "stm8s.h"
#include <intrinsics.h>
//! type redefinition 
typedef unsigned char           BYTE;
typedef unsigned short          WORD;
typedef signed long              int32;
typedef unsigned long           DWORD;

typedef signed char             int8;

typedef unsigned char           BOOL;
typedef unsigned long long      UQWORD;
typedef long long               SQWORD;


#define START   1
#define STOP    0

#ifndef NULL
#define NULL 0
#endif

#endif
/*=============================================================================
==============================END OF THE FILE==================================
==============================================================================*/
