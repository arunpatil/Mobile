/**************************************************************************************************
 * Copyright (c) 1999-2007 ADS Alliance Data Systems, Inc.  All rights reserved.                  *
 *------------------------------------------------------------------------------------------------*
 * $Archive:: /Omni Libraries/Atrana Libraries/v4.0/Software/src/Parms/fsparms.h                  $
 *------------------------------------------------------------------------------------------------*
 * $Workfile:: fsparms.h                                                                          $
 *------------------------------------------------------------------------------------------------*
 *************************************************************************************************/

#ifndef __FSPARMS2_H
   #define __FSPARMS2_H

#define FS_PARM_VERSION_NUMBER_SZ  12  /* Size of created version string buffer */

#define PARM_NAME_MAX_LEN 32

#ifndef CONST
#define CONST const
#endif


#define MAX_LIST_COUNT 20
#define MAX_LIST_ITEM_LEN 8

typedef enum {    /* Data type indicators used in the parameter table */
   pdtCharacter,
   pdtString,
   pdtNumeric,
   pdtInteger,
   pdtLong,
   pdtBoolean,
   pdtPassword,
   pdtList,
   pdtTableEnd = 99
} EParmDataType;

typedef struct
{
int iTotalItems;
int iSelectedItem;
char pListLabels[MAX_LIST_COUNT][MAX_LIST_ITEM_LEN+1];
}_SelectList;

typedef struct ParameterTableType_ {
   EParmDataType Type; /* Data type of Address */
   char *Name;         /* Parameter name */
   char *Description;  /* Parameter description */
   void *Address;      /* Address of the variable */
   bool  Printable;    /* TRUE if printable, FALSE if not */
   bool  Editable;     /* TRUE if editable, FALSE if not */
   char  MinLen;       /* Char is used for lengths to save memory */
   char  MaxLen;
   char *Default;      /* Defaults are stored as strings to support pre-initialization */
} ParameterTableType;

typedef enum
{
	parmsErrorNone,
	parmsErrorOpeningFile,
	parmsErrorFileSeek,
	parmsErrorParmsNoInit,
	parmsErrorParmNotFound,
	parmsErrorInvalidParm,
	parmsErrorSizeMisMatch,
	parmsErrorFlashMount
}ParmsErrorType;

/**************************************************************************************************
* --------------------- P U B L I C   F U N C T I O N   P R O T O T Y P E S --------------------- *
**************************************************************************************************/




bool fs_ParmInit(CONST char *AppCode, CONST ParameterTableType *Table, CONST char *Parameters, int ParametersSize);
bool fs_ParmSetup(bool LoadDefault);
void fs_ParmLoadNew(bool SetDefaultsIfEmpty);
bool fs_ParmReportPrint(int PrinterPort, int PrinterType);
bool fs_ParmLoadFromStore(void);
bool fs_ParmSaveToStore(void);
int  fs_ParmGetTableRecordCount(void);
bool fs_ParmEdit(int PromptTimeout);
void fs_ParmGetVersionNumber(char *VersionString);

#endif
