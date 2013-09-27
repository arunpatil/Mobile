#include "SDK30.h"
#include "parms.h"
#include "fsparms.h"
#include "Display.h"
#include "stdlib.h"
#include "UI.h"
#include "menu.h"

 typedef void * RAMDISK_HANDLE;
 typedef void * RAMDISK_FILE;

ParameterDataType glParameters;

ParameterTableType glParameterTable[] = {
/* General Parameters */
{pdtString,     "MERCHANT_ID",           "MERCHANT ID",              (void *)(glParameters.MerchantID),
 TRUE,  TRUE,   MID_LEN_MIN,        	MID_LEN_MAX,              	"826000000000123"
},
{pdtString,     "TERMINAL_ID",           "TERMINAL ID",              (void *)(glParameters.TerminalID),
 TRUE,  TRUE,   TID_LEN_MIN,        	TID_LEN_MAX,              	"82600123"
},
/*
{pdtString,     "CURRENCY_CODE",         "CURRENCY CODE",              (void *)(glParameters.CurrencyCode),
 TRUE,  TRUE,   CUR_CODE_MIN,        	CUR_CODE_MAX,              	"GBP"
},
*/
{pdtList,     "CURRENCY_LIST",        "DEFAULT CURRENCY",              (void *)(&glParameters.slCurrecyList),
 TRUE,  TRUE,   SELECT_LIST_MIN,        SELECT_LIST_MAX,              "2,GBP,USD,EUR,IRR,LKR,PKR,RWF,TZS,UGX"
},
{pdtString,     "MERCHANT_NAME",         "MERCHANT NAME",              (void *)(glParameters.MerchantName),
 TRUE,  TRUE,   MERCH_NAME_MIN,        	MERCH_NAME_MAX,              "MERCHANT ONE"
},
{pdtString,     "MERCHANT_ADDR1",        "ADDRESS LINE 1",              (void *)(glParameters.MerchantAddress+0),
 TRUE,  TRUE,   MERCH_ADDRESS_MIN,        MERCH_ADDRESS_MAX,              "39 ST.Road, Drayton"
},
{pdtString,     "MERCHANT_ADDR2",        "ADDRESS LINE 2",              (void *)(glParameters.MerchantAddress+1),
 TRUE,  TRUE,   MERCH_ADDRESS_MIN,        MERCH_ADDRESS_MAX,              "United Kingdom"
},
{pdtString,     "MERCHANT_ADDR3",        "ADDRESS LINE 3",              (void *)(glParameters.MerchantAddress+2),
 TRUE,  TRUE,   MERCH_ADDRESS_MIN,        MERCH_ADDRESS_MAX,              ""
},

/* Host Setup */
{pdtString,     "TCP_HOST",        		"HOST IP ADDRESS",              (void *)(glParameters.TCPHostIPAddr),
 TRUE,  TRUE,   TCPIP_LEN_MIN,        TCPIP_LEN_MAX,              		"88.211.29.227"
},
{pdtLong,     "TCP_PORT",        		"PORT NUMBER",              (void *)(&glParameters.TCPHostPort),
 TRUE,  TRUE,   TCPPORT_LEN_MIN,        TCPPORT_LEN_MAX,              		"20001"
},

/* Miscellaneous Passwords */
{pdtPassword,     "MERCHANT_PASS",        "MERCHANT PASSWORD",             (void *)glParameters.AdminPassword,
 FALSE, TRUE,  APASS_LEN_MIN,          APASS_LEN_MAX,                "123456"
},
{pdtPassword,     "MAINTAIN_PASS",        "MAINTAINANCE PASSWORD",      (void *)glParameters.MaintainPassword,
 FALSE, TRUE,  MAINTAIN_PASS_LEN_MIN,  MAINTAIN_PASS_LEN_MAX,         "000000"
},
{pdtPassword,     "DEFAULT_PASS",        "DEFAULT PASSWORD",      	(void *)glParameters.DefaultPassword,
 FALSE, FALSE,  DEFAULT_PASS_LEN_MIN,  DEFAULT_PASS_LEN_MAX,         "1234567890"
},

/* Utility Parameters */
{pdtBoolean,     "PROMPT_BEEP",           "PROMPT BEEP",              (void *)(&glParameters.PromptBeep),
 TRUE,  TRUE,   1,        1,              								"0"
},
{pdtInteger,     "PROMPT_TIMEOUT",           "PROMPT TIMEOUT",         (void *)(&glParameters.PromptTimeout),
 TRUE,  TRUE,   0,        5,              								"30"
},


/* For internal use only */
{pdtBoolean,     "MERCHANT_SET",         "MERCHANT PASSWORD SET",     (void *)(&glParameters.bMerchantPassWordSet),
 FALSE, FALSE,   1,        1,              								"0"
},
{pdtInteger,     "LOCK_LEVEL",           "LOCKED STATUS",         (void *)(&glParameters.iLockLevel),
 FALSE,  FALSE,   0,        5,              								"0"
},

/*
{pdtString,     "HEADER1",              "HEADER LINE 1",              (void *)(glParameters.HeaderLines+0),
 TRUE,  TRUE,   HEADER_LINE_MIN,        HEADER_LINE_MAX,              "ROADY@S TRUCK STOPS"
},
{pdtString,     "HEADER2",              "HEADER LINE 2",              (void *)(glParameters.HeaderLines+1),
 TRUE,  TRUE,   HEADER_LINE_MIN,        HEADER_LINE_MAX,              ""
},
*/
/* The last item is the end sentinel, this parm type MUST be pdtTableEnd. */
{pdtTableEnd,   0,                      0,                            (void *)0,
 TRUE,  TRUE,   0,                      0,                            ""
}
};

static char ParmFileName[32+1]={0};
static char ParmDirName[32+1]={0};
static S_FS_FILE *hParmFile=NULL;
static ParmsErrorType error;
static ParameterTableType *ParmsTable;
static int ParmsDataSize;
static char *ParmsPointer;
static int ParmsRecordCount;

bool fs_ParmInit(CONST char *AppCode, CONST ParameterTableType *Table, CONST char *Parameters, int ParametersSize)
{

	ParmsDataSize=ParametersSize;
	ParmsTable=(ParameterTableType*)Table;
	ParmsPointer=(char *)Parameters;

	sprintf(ParmFileName,"%s.par",AppCode);
	sprintf(ParmDirName,"%s",AppCode);
	//memset(ParmDirName,0,sizeof(ParmDirName));

	error=parmsErrorNone;
	return TRUE;
}

bool fs_ParmSetup(bool LoadDefault)
{
	bool bValue,*pbValue;
	int iValue,*piValue;
	long lValue,*plValue;
	char cValue,*pcValue;

	error=parmsErrorNone;
	LOG_PRINTF("fs_ParmSetup()|LoadDefault=%d",LoadDefault);
	if(LoadDefault==FALSE)
	{
		for(ParmsRecordCount=0;ParmsTable[ParmsRecordCount].Type!=pdtTableEnd;ParmsRecordCount++);
			return TRUE;
	}
	for(ParmsRecordCount=0;ParmsTable[ParmsRecordCount].Type!=pdtTableEnd;ParmsRecordCount++)
	{
		switch(ParmsTable[ParmsRecordCount].Type)
		{
			case pdtBoolean:
				bValue=atoi(ParmsTable[ParmsRecordCount].Default)?1:0;
				pbValue=ParmsTable[ParmsRecordCount].Address;
				*pbValue=bValue;
				break;

			case pdtCharacter:
				cValue=ParmsTable[ParmsRecordCount].Default[0];
				pcValue=ParmsTable[ParmsRecordCount].Address;
				*pcValue=cValue;
				break;

			case pdtInteger:
				iValue=atoi(ParmsTable[ParmsRecordCount].Default);
				piValue=ParmsTable[ParmsRecordCount].Address;
				*piValue=iValue;
				break;

			case pdtLong:
			case pdtNumeric:
				lValue=atol(ParmsTable[ParmsRecordCount].Default);
				plValue=ParmsTable[ParmsRecordCount].Address;
				*plValue=lValue;
				break;

			case pdtString:
			case pdtPassword:
				strcpy(ParmsTable[ParmsRecordCount].Address,ParmsTable[ParmsRecordCount].Default);
				break;

			case pdtList:
				fs_ParmExtractListContents(ParmsTable[ParmsRecordCount].Default,(_SelectList *)ParmsTable[ParmsRecordCount].Address);
				break;

			default:
				error=parmsErrorInvalidParm;
				return FALSE;
		}
	}


return TRUE;
}

bool fs_ParmLoadFromStore(void)
{
	ParameterDataType DummyParms;

	memset(&DummyParms,0,sizeof(ParameterDataType));
	error=parmsErrorNone;

	hParmFile=OpenFlashFile(ParmDirName,ParmFileName,FALSE);
	if(hParmFile==NULL)
	{
		error=parmsErrorParmsNoInit;
		return FALSE;
	}

	if(FS_seek(hParmFile,0,SEEK_SET)!=FS_OK )
	{
		error=parmsErrorFileSeek;
		FS_close(hParmFile);
		return FALSE;
	}

	if(FS_read(&DummyParms,ParmsDataSize,1,hParmFile)!=1)
	{
		error=parmsErrorSizeMisMatch;
		FS_close(hParmFile);
		return FALSE;
	}

	memcpy(ParmsPointer,&DummyParms,ParmsDataSize);

	FS_close(hParmFile);
return TRUE;
}


bool fs_ParmSaveToStore(void)
{
	int TotalBlocks=0;

	error=parmsErrorNone;

	hParmFile=OpenFlashFile(ParmDirName,ParmFileName,TRUE);
	if(hParmFile==NULL)
	{
		error=parmsErrorParmsNoInit;
		return FALSE;
	}

	if(FS_seek(hParmFile,0,SEEK_SET)!=FS_OK)
	{
		error=parmsErrorFileSeek;
		FS_close(hParmFile);
		return FALSE;
	}
	TotalBlocks=ParmsDataSize/2048 + (ParmsDataSize%2048)?1:0;
	while(TotalBlocks)
	{
		if(FS_write(ParmsPointer,(TotalBlocks>1)?2048:(ParmsDataSize%2048),1,hParmFile)!=1)
		{
			error=parmsErrorSizeMisMatch;
			FS_close(hParmFile);
			return FALSE;
		}
		FS_flush(hParmFile);
		TotalBlocks--;
	}
	FS_close(hParmFile);

return TRUE;
}

bool fs_DisplayParmsError(void)
{
	char *ptr=NULL;
	FILE *hDisplay;

	hDisplay=fopen("DISPLAY","w");

		switch(error)
		{
			case parmsErrorFileSeek:
				ptr="Error:File Seek";
				break;
			case parmsErrorInvalidParm:
				ptr="Error:Invalid Parms";
				break;
			case parmsErrorOpeningFile:
				ptr="Error:File Open";
				break;
			case parmsErrorParmNotFound:
				ptr="Error:Parm Not Found";
				break;
			case parmsErrorParmsNoInit:
				ptr="Error:Parm Not Initialized";
				break;
			case parmsErrorSizeMisMatch:
				ptr="Error:Parm Size Mismatch";
				break;

			case parmsErrorFlashMount:
				ptr="Error:Flash Disk Mount";
				break;

			default:
				ptr=NULL;
				break;

		}


		if(ptr)
		{
			clrscr();
			fs_DisplayAtRowReverse(ROW2,"ERROR!",daAlignCenter);
			fs_DisplayAtRow(ROW4,ptr,daAlignCenter);
			ttestall(0,3*100);
		}

		fclose(hDisplay);

return TRUE;
}

S_FS_FILE* OpenFlashFile(char *VolumeLabel,char *FileName,bool bForceCreate)
{
	   	S_FS_PARAM_CREATE ParamCreate;
	    int ret;
	    long DiskSize;
	    char FilePath_L[32+1]={0};
	    S_FS_FILE *hFileHandle;

	    ParamCreate.AccessMode = FS_WRTMOD;
	    if(VolumeLabel && VolumeLabel[0])
	    {
	    	strcpy(ParamCreate.Label, VolumeLabel);
	    	ParamCreate.Mode = FS_WRITEONCE;
	    	ParamCreate.AccessMode = FS_WRTMOD;
	    	ParamCreate.NbFichierMax = 100;
	    	ParamCreate.IdentZone = FS_WO_ZONE_DATA;
	    	DiskSize = 51200L;//8192L;

	    	ret=FS_dskcreate(&ParamCreate ,&DiskSize);
	    }
		if(VolumeLabel && VolumeLabel[0])
		{
			sprintf(FilePath_L,"/%s",VolumeLabel);
			ret=FS_mount(FilePath_L,&ParamCreate.AccessMode);
		}
		else
			ret=FS_mount("/HOST",&ParamCreate.AccessMode);

		if(ret!=FS_OK)
		{
			error=parmsErrorFlashMount;
			return FALSE;
		}

	    if(VolumeLabel && VolumeLabel[0])
	    	sprintf(FilePath_L,"/%s/%s", VolumeLabel,FileName);
	    else
	    	sprintf(FilePath_L,"/HOST/%s", FileName);

	 	if((hFileHandle=FS_open(FilePath_L,"r+"))==NULL)
	 	{
	       if(bForceCreate==FALSE)
	       {
	    	   error=parmsErrorOpeningFile;
	       	   return FALSE;
	       }

	 		if((hFileHandle=FS_open(FilePath_L,"a"))==NULL)
	 		{
	 			error=parmsErrorOpeningFile;
	 			return FALSE;
	 		}

	 		FS_close(hFileHandle);

	 		if((hFileHandle=FS_open(FilePath_L,"r+"))==NULL)
	 		{
	 			error=parmsErrorOpeningFile;
	 			return FALSE;
	 		}
	 	}
	return hFileHandle;
}

S_FS_FILE* OpenRamFile(char *VolumeLabel,char *FileName)
{
	   	S_FS_PARAM_CREATE ParamCreate;
	    int ret;
	    long DiskSize;
	    char FilePath_L[32+1]={0};
	    S_FS_FILE *hFileHandle;

	    if(VolumeLabel && VolumeLabel[0])
	    {
	     // create the volume Appli1
	     strcpy(ParamCreate.Label, VolumeLabel);
	     ParamCreate.Mode = FS_WRITEMANY;
	     ParamCreate.AccessMode = FS_WRTMOD;
	     ParamCreate.NbFichierMax = 100;
	     ParamCreate.IdentZone = FS_WO_ZONE_DATA;
	     DiskSize = 51200L;//8192L;

	     ret=FS_dskcreate(&ParamCreate ,&DiskSize);

	     clrscr();
	     if (ret==FS_OK)
	    	 printf ("create : OK \n");
	     if (ret==FS_KO)
	    	 printf ("create : ERROR\n");
	     if (ret==FS_NAMEUSED)
	    	 printf ("create : Name used\n");
	     if (ret==FS_NOMEM)
	    	 printf ("not enough space : %ld \n", DiskSize);
	     ttestall(0,3*100);

	    }
	     //if(ret==FS_NAMEUSED)

			if(VolumeLabel && VolumeLabel[0])
			{
				sprintf(FilePath_L,"/%s",VolumeLabel);
				ret=FS_mount(FilePath_L,&ParamCreate.AccessMode);
			}
			else
				ret=FS_mount("/SYSTEM",&ParamCreate.AccessMode);

				_clrscr();
	    	    if (ret==FS_OK)
	    	       printf ("mount : OK \n");
	    	     if (ret==FS_KO)
	    	       printf ("mount : ERROR\n");
	    	     if (ret==FS_NAMEUSED)
	    	       printf ("mount : Name used\n");
	    	     if (ret==FS_NOFORMAT )
	    	    	 printf ("Not Formatted");
	    	     if (ret==FS_NOACCESS )
	    	    	 printf ("No Access");
	    	     ttestall(0,3*100);

	    if(VolumeLabel && VolumeLabel[0])
	    	sprintf(FilePath_L,"/%s/%s", VolumeLabel,FileName);
	    else
	    	sprintf(FilePath_L,"/SYSTEM/%s", FileName);

	    LOG_PRINTF("FilePath_L=%s",FilePath_L);
	    if(FS_exist(FilePath_L)==FS_OK)
	    	LOG_PRINTF("File Exists");
	    else
	    	LOG_PRINTF("File Does Not Exist");


	 	if((hFileHandle=FS_open(FilePath_L,"r"))==NULL)
	 	{

	 	   clrscr();
	       printf("File Not Present");
	       LOG_PRINTF("OpenRamFile() | File Not Present");
	       ttestall(0,3*100);


	 		if((hFileHandle=FS_open(FilePath_L,"a"))==NULL)
	 		{
	 			error=parmsErrorOpeningFile;
	 			return FALSE;
	 		}

	 		FS_close(hFileHandle);

	 		if((hFileHandle=FS_open(FilePath_L,"r+"))==NULL)
	 		{
	 			error=parmsErrorOpeningFile;
	 			return FALSE;
	 		}
	 	}

	  _clrscr();
      printf("File Opened\n%ld KB\t",FS_length(hFileHandle));
      ttestall(0,3*100);


	return hFileHandle;
}

bool fs_ParmEdit(int PromptTimeout)
{
	bool bValue,*pbValue;
	int iValue,*piValue;
	long lValue,*plValue;
	char cValue,*pcValue;
	char Buffer[100]={0};
	char strPromptBuffer[100]={0};
	char strPasswordBuffer[100]={0};
	int ParmsRecordCountLocal;
	int WindowHeight,WindowWidth;
	bool bSaveChanges=FALSE;
	int iLoopCounter;
	bool bRedrawWindow=FALSE;
	_SelectList slListDummy;
	char	*pItems[MAX_LIST_COUNT];

	int iSelectedItem;

	if(hParmFile==NULL)
	{
		error=parmsErrorParmsNoInit;
		return FALSE;
	}
	PromptTimeout=glParameters.PromptTimeout;

	if(!ParmsRecordCount)
	{
		error=parmsErrorParmNotFound;
		return FALSE;
	}

	GetScreenSize(&WindowHeight,&WindowWidth);
	error=parmsErrorNone;
	fs_DisplayInit();
	fs_DisplayClearAll();
	DrawWindowFont(0,0,WindowWidth-1,WindowHeight-1,1,"EDIT PARAMETERS",_SMALL_);

		/* Locate First Editable Parameter */
		for(ParmsRecordCountLocal=0;((ParmsTable[ParmsRecordCountLocal].Type!=pdtTableEnd) && (ParmsTable[ParmsRecordCountLocal].Editable==FALSE));ParmsRecordCountLocal++);

		if(ParmsRecordCountLocal>=ParmsRecordCount)
		{
			error=parmsErrorParmNotFound;
			return FALSE;
		}

		for(;ParmsTable[ParmsRecordCountLocal].Type!=pdtTableEnd;)
		{

			fs_DisplayAtRowOverWrite(ROW2,ParmsTable[ParmsRecordCountLocal].Description,daAlignCenter);

			memset(Buffer,0,sizeof(Buffer));
			switch(ParmsTable[ParmsRecordCountLocal].Type)
			{
				case pdtBoolean:
					pbValue=ParmsTable[ParmsRecordCountLocal].Address;
					bValue=*pbValue;
					if(bValue)
						fs_DisplayAtRowOverWrite(ROW4,"ON",daAlignCenter);
					else
						fs_DisplayAtRowOverWrite(ROW4,"OFF",daAlignCenter);
					break;

				case pdtCharacter:
					pcValue=ParmsTable[ParmsRecordCountLocal].Address;
					cValue=*pcValue;
					fs_DisplayAtRowOverWrite(ROW4,pcValue,daAlignCenter);
					break;

				case pdtInteger:
					piValue=ParmsTable[ParmsRecordCountLocal].Address;
					iValue=*piValue;
					sprintf(Buffer,"%d",iValue);
					fs_DisplayAtRowOverWrite(ROW4,Buffer,daAlignCenter);
					break;

				case pdtLong:
				case pdtNumeric:
					plValue=ParmsTable[ParmsRecordCountLocal].Address;
					lValue=*plValue;
					sprintf(Buffer,"%ld",lValue);
					fs_DisplayAtRowOverWrite(ROW4,Buffer,daAlignCenter);
					break;

				case pdtString:
					fs_DisplayAtRowOverWrite(ROW4,ParmsTable[ParmsRecordCountLocal].Address,daAlignCenter);
					break;

				case pdtPassword:
					memset(strPromptBuffer,0,sizeof(strPromptBuffer));
					iLoopCounter=(int)strlen(ParmsTable[ParmsRecordCountLocal].Address);
					while(iLoopCounter--)
						strcat(strPromptBuffer,"*");
					fs_DisplayAtRowOverWrite(ROW4,strPromptBuffer,daAlignCenter);
					break;

				case pdtList:
					fs_DisplayAtRowOverWrite(ROW4,fs_ParmGetSelectedItem(ParmsTable[ParmsRecordCountLocal].Address),daAlignCenter);
					break;

				default:
					error=parmsErrorInvalidParm;
					return FALSE;
			}
			fs_DisplayAtRowOverWrite(ROW6,"Prev                       Edit    Next",daAlignLeft);
			fs_UIResetParameters();
			switch(fs_UIGetChar(glParameters.PromptTimeout))
			{
				int ParamCountBackup;
				case T_SK1:
				case T_SKBAS:
					for(ParamCountBackup=ParmsRecordCountLocal--; ((ParmsRecordCountLocal>=0)?(ParmsTable[ParmsRecordCountLocal].Type!=pdtTableEnd) && (ParmsTable[ParmsRecordCountLocal].Editable==FALSE):TRUE);ParmsRecordCountLocal--)
					{
						if(ParmsRecordCountLocal<0)
						{
							fs_UIBeepNavigateError();
							ParmsRecordCountLocal=ParamCountBackup;
							break;
						}
					}
					break;

				case T_SK4:
				case T_SKHAUT:

					for(ParamCountBackup=ParmsRecordCountLocal++; (ParmsTable[ParmsRecordCountLocal].Editable==FALSE);ParmsRecordCountLocal++)
					{
						if(ParmsTable[ParmsRecordCountLocal].Type==pdtTableEnd)
						{
							fs_UIBeepNavigateError();
							ParmsRecordCountLocal=ParamCountBackup;
							break;
						}
					}
					if(ParmsTable[ParmsRecordCountLocal].Type==pdtTableEnd)
					{
						fs_UIBeepNavigateError();
						ParmsRecordCountLocal=ParamCountBackup;
					}
					break;

				case T_SK3:
					/*
					fs_DisplayInit();
					fs_DisplayClearAll();
					DrawWindowFont(0,0,WindowWidth-1,WindowHeight-1,1,"EDIT PARAMETERS",_SMALL_);
					*/
					fs_UISetEntryRow(ROW4);
					fs_UISetPromptRow(ROW2);
					switch(ParmsTable[ParmsRecordCountLocal].Type)
					{
						case pdtCharacter:
						case pdtString:
						if(fs_UIGetInput("EDIT PARAMETERS",ParmsTable[ParmsRecordCountLocal].Description,"",daAlignCenter,entryTypeAlphaNumeric,daAlignCenter,
								Buffer,ParmsTable[ParmsRecordCountLocal].MinLen,
								ParmsTable[ParmsRecordCountLocal].MaxLen,0))
						{
							strcpy(ParmsTable[ParmsRecordCountLocal].Address,Buffer);
							bSaveChanges=TRUE;
						}
						break;

						case pdtInteger:
						case pdtLong:
						case pdtNumeric:
						if(fs_UIGetInput("EDIT PARAMETERS",ParmsTable[ParmsRecordCountLocal].Description,"",daAlignCenter,entryTypeNumeric,daAlignCenter,
								Buffer,ParmsTable[ParmsRecordCountLocal].MinLen,
								ParmsTable[ParmsRecordCountLocal].MaxLen,glParameters.PromptTimeout))
						{
							plValue=ParmsTable[ParmsRecordCountLocal].Address;
							*plValue=atol(Buffer);
							bSaveChanges=TRUE;
						}
						break;

						case pdtPassword:
							fs_UISetEntryRow(ROW6);
							fs_UISetPromptRow(ROW3);
							bRedrawWindow=TRUE;
							if(fs_UIGetInput("EDIT PASSWORD","CURRENT",ParmsTable[ParmsRecordCountLocal].Description,daAlignCenter,entryTypePassword,daAlignCenter,
									Buffer,ParmsTable[ParmsRecordCountLocal].MinLen,
									ParmsTable[ParmsRecordCountLocal].MaxLen,0))
							{
								if(!strcmp(ParmsTable[ParmsRecordCountLocal].Address,Buffer))
								{
									while(fs_UIGetInput("EDIT PASSWORD","ENTER NEW",ParmsTable[ParmsRecordCountLocal].Description,daAlignCenter,entryTypePassword,daAlignCenter,
																		Buffer,ParmsTable[ParmsRecordCountLocal].MinLen,
																		ParmsTable[ParmsRecordCountLocal].MaxLen,0))
									{
										if(fs_UIGetInput("EDIT PASSWORD","RE-ENTER NEW",ParmsTable[ParmsRecordCountLocal].Description,daAlignCenter,entryTypePassword,daAlignCenter,
																												strPasswordBuffer,ParmsTable[ParmsRecordCountLocal].MinLen,
																												ParmsTable[ParmsRecordCountLocal].MaxLen,0))
										{
											if(!strcmp(Buffer,strPasswordBuffer))
											{
												strcpy(ParmsTable[ParmsRecordCountLocal].Address,strPasswordBuffer);
												bSaveChanges=TRUE;
												break;
											}
											else
											{
												fs_DisplayClearWindowContents();
												fs_DisplayAtRowOverWrite(ROW3,"PASSWORD",daAlignCenter);
												fs_DisplayAtRowOverWrite(ROW4,"DOES NOT MATCH!",daAlignCenter);
												fs_DisplayAtRowOverWrite(ROW6," ",daAlignCenter);
												ttestall(0,300);
											}

										}
									}
								}
								else
								{
									fs_DisplayClearWindowContents();
									fs_DisplayAtRowOverWrite(ROW3,"WRONG",daAlignCenter);
									fs_DisplayAtRowOverWrite(ROW4,ParmsTable[ParmsRecordCountLocal].Description,daAlignCenter);
									fs_DisplayAtRowOverWrite(ROW6," ",daAlignCenter);
									ttestall(0,300);
								}


							}

							break;


						case pdtBoolean:
							pbValue=ParmsTable[ParmsRecordCountLocal].Address;
							//fs_DisplayAtRowOverWrite(ROW2,ParmsTable[ParmsRecordCountLocal].Description,daAlignCenter);
							fs_DisplayAtRowOverWrite(ROW6,"OFF                                               ON",daAlignLeft);
							CONTINUE_ENTRY:
							switch(fs_UIGetChar(glParameters.PromptTimeout))
							{
								case T_ANN:
								case FALSE:
									break;

								case T_SK4:
									*pbValue=TRUE;
									bSaveChanges=TRUE;
									break;
								case T_SK1:
									*pbValue=FALSE;
									bSaveChanges=TRUE;
									break;

								default:
									goto CONTINUE_ENTRY;
							}

						break;

						case pdtList:
							memset(&slListDummy,0,sizeof(_SelectList));
							memcpy(&slListDummy,ParmsTable[ParmsRecordCountLocal].Address,sizeof(_SelectList));
							for(iLoopCounter=0;iLoopCounter<slListDummy.iTotalItems;iLoopCounter++)
								pItems[iLoopCounter]=slListDummy.pListLabels[iLoopCounter];
							iSelectedItem=SelectFromList("SELECT",TRUE,
									(((slListDummy.iSelectedItem-1)>0)?(slListDummy.iSelectedItem-1):0),slListDummy.iTotalItems,(const char **)pItems,((MAX_LIST_ITEM_LEN>8)? _WINDOW_SIZE_MEDIUM_:_WINDOW_SIZE_SMALL_),PromptTimeout);

							switch(iSelectedItem)
							{
								case __EXIT_KEY:
								case __BACK_KEY:
									break;

								default:
									slListDummy.iSelectedItem=iSelectedItem+1;	/* function returns value from 0 to max */
									memcpy(ParmsTable[ParmsRecordCountLocal].Address,&slListDummy,sizeof(_SelectList));
									bSaveChanges=TRUE;
									break;
							}
							bRedrawWindow=TRUE;
							break;

						default:
							break;
					}
					if((bRedrawWindow==TRUE) || (ParmsTable[ParmsRecordCountLocal].MaxLen>20))	/* To Correct Corrupted Window Borders */
					{
						fs_DisplayInit();
						fs_DisplayClearAll();
						DrawWindowFont(0,0,WindowWidth-1,WindowHeight-1,1,"EDIT PARAMETERS",_SMALL_);
						bRedrawWindow=FALSE;
					}
					fs_UIResetParameters();
					break;

				case FALSE:
				case T_ANN:
					if(bSaveChanges==TRUE)
						fs_ParmSaveToStore();
					return FALSE;

				default:
					break;

			}
		}

	if(bSaveChanges==TRUE)
		fs_ParmSaveToStore();

return TRUE;
}

bool fs_ExtractParametersFromFile(char *ParameterFile)
{

	S_FS_FILE *fp;
	char Buffer[1000]={0};
	char Key[32+1]={0};
	char Value[128+1]={0};
	char *Separator;
	int CharCount=0;
	char strFileName[100]={0};


	if((fp=OpenFlashFile("",ParameterFile,FALSE))==NULL)
	{
		return FALSE;
	}
	while(fs_ReadOneLine(fp,Buffer,sizeof(Buffer)))
	{
		fs_RemoveRightPadding(Buffer);
		if(((Separator = strchr(Buffer,'='))!=0) ||
			((Separator = strchr(Buffer,' '))!=0))
		{
			memset(Key,0,sizeof(Key));
			strncpy(Key,Buffer,(Separator-Buffer));

			strcpy(Buffer,Separator+1);
			CharCount=0;
			while((Separator=strchr(Buffer,'"'))!=0)
			{
				if(CharCount==0)
					*Separator=' ';
				else
					*Separator='\0';
				CharCount++;
			}
			fs_RemoveLeftPadding(Buffer,' ');
			strcpy(Value,Buffer);
			LOG_PRINTF("ExtractParametersFromFile() | Key=%s",Key);
			LOG_PRINTF("ExtractParametersFromFile() | Value=%s",Value);
			fs_PutDldParameter(Key,Value);
		}
	}
	FS_close(fp);

	sprintf(strFileName,"/HOST/%s",ParameterFile);
	FS_unlink(strFileName);
return TRUE;
}
/**
 * bool fs_ReadOneLine(S_FS_FILE *fptr,char *Buffer,int size)
 * Description: This function reads one line from the file handle provided. It functions same as fgets().
 *
 * @param fptr: File Handle
 * @param buffer : Buffer to store the string read from the file handle
 * @param size : Size of the buffer
 * @return: Returns TRUE if line is read successfully else FALSE
 */

bool fs_ReadOneLine(S_FS_FILE *fptr,char *buffer,int size)
{
bool flagCarriageReturn=FALSE,flagSuccess=FALSE;
char *memPtr=0;
char *varPtr=0;
int lSize=0;
static int siTotalLinesRead;

	/* Memory Allocation */
	memPtr=(char*)umalloc(size);
	if(!memPtr)
	{
		return FALSE;
	}
	memset(memPtr,0,size);
	memset(buffer,0,size);
	varPtr=memPtr;

	/* Read single byte till we do not get LF or CR or EOF */
	while((FS_read(varPtr,1,1,fptr))==1)
	{
		if(*varPtr==(char)EOF)
		{
			break;
		}
		/* Skip byte if its first byte & is LF or CR because combination of CR & LF may be used in windows*/
		else if(!lSize && !flagCarriageReturn && (*varPtr==CR	|| *varPtr==LF))
			flagCarriageReturn=TRUE;
		else if(*varPtr==CR	|| *varPtr==LF)
			break;
		else
		{
			varPtr++;
			lSize++;
		}
		if(lSize>=size)
			break;
	}
	siTotalLinesRead++;
	if(lSize)
	{
		memcpy(buffer,memPtr,lSize);
		flagSuccess=TRUE;
	}
	/* Free memory allocated */
	ufree(memPtr);
return flagSuccess;
}



/*	Good for removing leading zeros, this function will point to the first character
 *	of buf, that is not a pad_char.  It then copies the substring back into buf.
 *
 *	IMPORTANT: You MUST null-terminate buf PRIOR to calling this function.
 */
void fs_RemoveLeftPadding(char *buf, char pad_char)
{
	char	*temp;
	int		i, len;

	len = strlen(buf);

	for(i = 0; buf[i] == pad_char && i < len; i++){}	// Advance to the first non-pad_char

	if(i != len)	// If we made it to len, then the entire buffer is made up of pad characters
	{
		temp = &buf[i];		// harpoon the substring
		strcpy(buf, temp);	// drag it over the pad characters
	}
	else {
		buf[0]='\0';
	}
}

/*	Most of our manager files will be left-justified, right-padded with spaces.  We will start with the last character
 *	in the string, and reduce the pointer position if it is a space.  We will continue until we find a non-space character
 *	in our string.  We then null-terminate the position to the right of the non-space character.
 *
 *	IMPORTANT: Although buf is right-padded with spaces, buf MUST be NULL-terminated PRIOR to calling this function!
 */

void fs_RemoveRightPadding(char *buf)
{
	int	last;
	int	i;

	last = strlen(buf) - 1;			// Set the pointer to look at the first character BEFORE the null terminator.

	for(i = last; i >= 0 && buf[i] <= 32; i--){	}	// "rewind" the pointer back to a non-space position.

	buf[i+1] = '\0';				// Null-terminate.
}

/** bool fs_PutDldParameter(char *parameter,char *value)
 *  Description: This function is used to store the environment variable in system file
 *
 * @param parameter : Parameter name
 * @param value : Value to be stored for the parameter
 * @return : Returns TRUE if succeeded else FALSE
 */
bool fs_PutDldParameter(char *parameter,char *value)
{
	bool *pbValue;
	int *piValue;
	long *plValue;
	char *pcValue;
	int lParmsRecordCount;

	error=parmsErrorNone;	/* Reset 'error' variable */
	for(lParmsRecordCount=0;ParmsTable[lParmsRecordCount].Type!=pdtTableEnd;lParmsRecordCount++)
	{
		/* Check for every record in parameter table until parameter name matches*/
		if(!strcmp(ParmsTable[lParmsRecordCount].Name,parameter))
		{
			if(value && *value)
			{
				switch(ParmsTable[lParmsRecordCount].Type)
				{
					/* Set values depending upon the parameter type */
					case pdtBoolean:
						pbValue=ParmsTable[lParmsRecordCount].Address;
						*pbValue=(atoi(value))?TRUE:FALSE;
						LOG_PRINTF("fs_PutDldParameter() |*pbValue=%d",*pbValue);
						break;

					case pdtCharacter:
						pcValue=ParmsTable[lParmsRecordCount].Address;
						*pcValue=value[0];
						LOG_PRINTF("fs_PutDldParameter() |*pcValue=%c",*pcValue);
						break;

					case pdtInteger:
						piValue=ParmsTable[lParmsRecordCount].Address;
						*piValue=atoi(value);
						LOG_PRINTF("fs_PutDldParameter() |*piValue=%d",*piValue);
						break;

					case pdtLong:
					case pdtNumeric:
						plValue=ParmsTable[lParmsRecordCount].Address;
						*plValue=atol(value);
						LOG_PRINTF("fs_PutDldParameter() |*plValue=%ld",*plValue);
						break;

					case pdtString:
					case pdtPassword:
						strncpy(ParmsTable[lParmsRecordCount].Address,value,ParmsTable[lParmsRecordCount].MaxLen);
						LOG_PRINTF("fs_PutDldParameter() |string=%s",ParmsTable[lParmsRecordCount].Address);
						break;

					case pdtList:
						fs_ParmExtractListContents(value,(_SelectList*)ParmsTable[lParmsRecordCount].Address);
						LOG_PRINTF("fs_PutDldParameter() |List=%s",value);
						break;

				default:
					/* Throw an error if no match found */
					error=parmsErrorInvalidParm;
					return FALSE;
			}
			}
			break;
		}
	}

return TRUE;
}

/**
 * bool fs_ParmExtractListContents(char *inputString,_SelectList *slSelectList)
 * @param inputString : Its is input srting from which list has to be extracted. It is in format :
 * "default item(int), ITEM1,ITEM2,......"
 * @param slSelectList: Pointer in which the string to be extracted
 * @return: Returns TRUE if one or more items found in list & properly extracted
 */

bool fs_ParmExtractListContents(char *inputString,_SelectList *slSelectList)
{

	_SelectList slSelectListDummy;
	char *pItemPointer;
	char *pVarPointer;
	bool bResult=FALSE;
	bool bContinueLoop=TRUE;
	int iTrimLength;

	memset(&slSelectListDummy,0,sizeof(_SelectList));

	pItemPointer=inputString;

	/* Extract the default item count */
	if(pItemPointer && *pItemPointer>='0' && *pItemPointer<='9')
	{
		slSelectListDummy.iSelectedItem =atoi(pItemPointer);
		if(slSelectListDummy.iSelectedItem>MAX_LIST_COUNT)
			slSelectListDummy.iSelectedItem=MAX_LIST_COUNT;
		else if(slSelectListDummy.iSelectedItem < 0)
			slSelectListDummy.iSelectedItem=0;

		/* Break if any of below mentioned characters found */
		(pVarPointer=strchr(pItemPointer,',')) ||	/* comma */
		(pVarPointer=strchr(pItemPointer,';')) ||	/* semi colon */
		(pVarPointer=strchr(pItemPointer,' '));		/* space */

		if(!pVarPointer || (*(pVarPointer+1)=='\0'))
			return FALSE;
		pItemPointer=pVarPointer+1;
	}
	/* Extract all item labels */
	for(bContinueLoop=TRUE;bContinueLoop && (slSelectListDummy.iTotalItems<MAX_LIST_COUNT);slSelectListDummy.iTotalItems++)
	{

		/* Stop at any of the below mentioned characters */
		(pVarPointer=strchr(pItemPointer,',')) ||	/* comma */
		(pVarPointer=strchr(pItemPointer,';')) ||	/* semicolon */
		(pVarPointer=strchr(pItemPointer,' '));		/* space */

		if(!pVarPointer || (*(pVarPointer+1)=='\0'))
			bContinueLoop=FALSE;

		/* Calculate the length of the specific item */
		iTrimLength=((pVarPointer>pItemPointer)?(pVarPointer-pItemPointer):(int)strlen(pItemPointer));
		iTrimLength=(iTrimLength>MAX_LIST_ITEM_LEN)?MAX_LIST_ITEM_LEN:iTrimLength;

		/* Set result flag if at least one item found */
		if(bResult==FALSE && iTrimLength!=0)
			bResult=TRUE;

		strncpy(&slSelectListDummy.pListLabels[slSelectListDummy.iTotalItems][0],pItemPointer,iTrimLength);
		LOG_PRINTF("fs_ExctractListContents() | slSelectListDummy.iTotalItems=%d",slSelectListDummy.iTotalItems);
		LOG_PRINTF("fs_ExctractListContents() | slSelectListDummy.pListLabel=%s",slSelectListDummy.pListLabels[slSelectListDummy.iTotalItems]);

		if(bContinueLoop==TRUE)
			pItemPointer=pVarPointer+1;
	}

	if(slSelectListDummy.iSelectedItem>slSelectListDummy.iTotalItems)
		slSelectListDummy.iSelectedItem=slSelectListDummy.iTotalItems;

	if(bResult==TRUE)
		memcpy(slSelectList,&slSelectListDummy,sizeof(_SelectList));

return TRUE;
}

/**
 * char  *fs_ParmGetSelectedItem(_SelectList *slSelectList)
 * Gets the selected item from the list type
 * @param slSelectList: Pointer to list variable type
 * @return: Returns the pointer to selected item if valid else NULL pointer
 */

char  *fs_ParmGetSelectedItem(_SelectList *slSelectList)
{

	/* check if valid item present at the selected location */
	if( (slSelectList->iSelectedItem>=0) &&
		slSelectList->pListLabels[(slSelectList->iSelectedItem>0)?(slSelectList->iSelectedItem-1):0]
	  )
		return (char *) &slSelectList->pListLabels[(slSelectList->iSelectedItem>0)?(slSelectList->iSelectedItem-1):0][0];
return "";	/* If not found the return NULL */
}

