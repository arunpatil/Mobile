#include "SDK30.h"
#include "parms.h"
#include "fsparms.h"
#include "Display.h"
#include "stdlib.h"
#include "Printer.h"
#include "UI.h"

static FILE *hPrinter;
static EPrinterResultCode error;
static prnConfig PrinterConfig;


bool fs_PrinterInit(void)
{
	error=prnrcNone;
	if(hPrinter)
		fclose(hPrinter);

	memset(&PrinterConfig,0,sizeof(prnConfig));

		hPrinter  = fopen( "PRINTER",  "w" );
		if(hPrinter<=0)
		{
			error=prnrcErrPrnOpen;
			return FALSE;
		}
		PrinterConfig.Initialized=TRUE;
return TRUE;
}

bool fs_PrinterClose(void)
{
	fclose(hPrinter);
	hPrinter=NULL;
	PrinterConfig.Initialized=FALSE;
return TRUE;
}

bool fs_PrinterGetLastError(EPrinterResultCode *Result, char *Text, int BuffSize)
{
	char Buffer[100]={0};
	*Result=error;

	switch(error)
	{
		case prnrcErrPrnAccess:
			sprintf(Buffer,"NO PRINTER ACCESS");
			break;
		case prnrcErrPrnRelease:
			sprintf(Buffer,"RELEASING PRINTER ACCESS");
			break;
		case prnrcErrPrnConfig:
			sprintf(Buffer,"PRINTER CONFIG");
			break;
		case prnrcErrPrnOpen:
			sprintf(Buffer,"PRINTER OPEN");
			break;
		case prnrcErrPrnClose:
			sprintf(Buffer,"PRINTER CLOSE");
			break;
		case prnrcErrPrnWrite:
			sprintf(Buffer,"PRINTER WRITE");
			break;
		case prnrcErrFileAccess:
			sprintf(Buffer,"PRINTER FILE ACCESS");
			break;
		case prnrcErrRspTimeout:
			sprintf(Buffer,"PRINTER RESPONSE TIMEOUT");
			break;
		case prnrcErrFmtUnsupported:
			sprintf(Buffer,"PRINTER FORMAT NOT SUPPORTED");
			break;
		case prnrcErrOutOfPaper:
			sprintf(Buffer,"PRINTER OUT OF PAPER");
			break;
		case prnrcBadParam:
			sprintf(Buffer,"PRINTER BAD PARAMETER");
			break;

		default:
			sprintf(Buffer,"UNKNOWN ERROR");
			break;

	}

	memset(Text,0,BuffSize);
	strncpy(Text,Buffer,BuffSize);
return TRUE;
}


bool fs_PrinterIsOutputPending(bool *Pending)
{
	*Pending=FALSE;
	return FALSE;
}

bool fs_PrinterSetBold(bool Value)
{
	PrinterConfig.Bold=Value?TRUE:FALSE;
	PrinterConfig.DoubleWide=PrinterConfig.Bold;
return TRUE;
}

bool fs_PrinterSetCondensed(bool Value)
{
	PrinterConfig.CondensedMode=Value?TRUE:FALSE;
return TRUE;
}

bool fs_PrinterSetInverse(bool Value)
{
	PrinterConfig.Inverse=Value?TRUE:FALSE;
return TRUE;
}

bool fs_PrinterSetDoubleHigh(bool Value)
{
	PrinterConfig.DoubleHigh=Value?TRUE:FALSE;
return TRUE;
}

bool fs_PrinterSetDoubleWide(bool Value)
{
	PrinterConfig.DoubleWide=Value?TRUE:FALSE;
	PrinterConfig.Bold=PrinterConfig.DoubleWide;
return TRUE;
}

bool fs_PrinterSetJustification(int Value)
{
	if(Value<=FS_PRINTER_JUSTIFY_CENTER)
		PrinterConfig.Justification=Value;
	else
		return FALSE;
return TRUE;
}

bool fs_PrinterResetAttributes(void)
{
	memset(&PrinterConfig,0,sizeof(prnConfig));
	return TRUE;
}

bool fs_PrinterAppendLine(CONST char *Line)
{
	char Buffer[FS_PRINTER_MAX_FORMAT_SIZE+1]={0};
	char LineBuffer[FS_PRINTER_MAX_CHARS_CONDENSED+1]={0};
	volatile int MaxColumn=0;
	volatile int StartColumn=0;

	/* Restore Default Settings */
	strcat(Buffer,"\x1B""@");

	/* Set Inverse Characteristics */
	if(PrinterConfig.Inverse==TRUE)
		strcat(Buffer,"\x1B""B1");
	else
		strcat(Buffer,"\x1B""B0");

	/* Set Bold */
/*
	if(PrinterConfig.Bold==TRUE)
		strcat(Buffer,"\x1B""E");
*/
	/* Set Double High */
	if(PrinterConfig.DoubleHigh==TRUE)
		strcat(Buffer,"\x1B""H");

	/* Set Double Wide */
	if(PrinterConfig.CondensedMode==TRUE)
		strcat(Buffer,"\xF");
	else if(PrinterConfig.DoubleWide==TRUE || PrinterConfig.Bold==TRUE)
		strcat(Buffer,"\xE");
	else
		strcat(Buffer,"\x12");

	if(PrinterConfig.CondensedMode==TRUE)
		MaxColumn=FS_PRINTER_MAX_CHARS_CONDENSED;
	else
		MaxColumn=FS_PRINTER_MAX_CHARS_NORMAL;

	if((PrinterConfig.DoubleWide==TRUE || PrinterConfig.Bold==TRUE) && PrinterConfig.CondensedMode==FALSE )
		MaxColumn=MaxColumn/2;

	memset(LineBuffer,' ',MaxColumn);

	switch(PrinterConfig.Justification)
	{
		case FS_PRINTER_JUSTIFY_RIGHT:
			StartColumn=MaxColumn-(int)strlen(Line);
			break;

		case FS_PRINTER_JUSTIFY_CENTER:
			StartColumn=(MaxColumn-(int)strlen(Line))/2;
			break;

		default:
			StartColumn=0;
			break;
	}
	StartColumn=(StartColumn<0)? 0:StartColumn;
	LOG_PRINTF("StartCol=%d",StartColumn);
	//snprintf(LineBuffer+StartColumn,strlen(Line),Line);
	memcpy(LineBuffer+StartColumn,Line,((int)strlen(Line)>MaxColumn)?MaxColumn:(int)strlen(Line));
	strcat(Buffer,LineBuffer);
	/* New Line Terminator */
	strcat(Buffer,"\n");
	strcat(Buffer,"\x1B""B0");	/* Cancel Inverse Else It Will Print Inverted In Other Applications Also */
	pprintf(Buffer);

	if(!ttestall(PRINTER,500))
		return FALSE;

return TRUE;
}

bool fs_PrinterLRAppendLineLR(CONST char *Line1,CONST char *Line2)
{
	char Buffer[FS_PRINTER_MAX_FORMAT_SIZE+1]={0};
	char LineBuffer[FS_PRINTER_MAX_CHARS_CONDENSED+1]={0};
	volatile int MaxColumn=0;
	volatile int StartColumn=0;

	/* Restore Default Settings */
	strcat(Buffer,"\x1B""@");

	/* Set Inverse Characteristics */
	if(PrinterConfig.Inverse==TRUE)
		strcat(Buffer,"\x1B""B1");
	else
		strcat(Buffer,"\x1B""B0");

	/* Set Bold */
/*
	if(PrinterConfig.Bold==TRUE)
		strcat(Buffer,"\x1B""E");
*/
	/* Set Double High */
	if(PrinterConfig.DoubleHigh==TRUE)
		strcat(Buffer,"\x1B""H");

	/* Set Double Wide */
	if(PrinterConfig.CondensedMode==TRUE)
		strcat(Buffer,"\xF");
	else if(PrinterConfig.DoubleWide==TRUE || PrinterConfig.Bold==TRUE)
		strcat(Buffer,"\xE");
	else
		strcat(Buffer,"\x12");

	if(PrinterConfig.CondensedMode==TRUE)
		MaxColumn=FS_PRINTER_MAX_CHARS_CONDENSED;
	else
		MaxColumn=FS_PRINTER_MAX_CHARS_NORMAL;

	if((PrinterConfig.DoubleWide==TRUE || PrinterConfig.Bold==TRUE) && PrinterConfig.CondensedMode==FALSE )
		MaxColumn=MaxColumn/2;

	memset(LineBuffer,' ',MaxColumn);

	StartColumn=MaxColumn-(int)strlen(Line2);
	StartColumn=(StartColumn<0)? 0:StartColumn;
	LOG_PRINTF("StartCol=%d",StartColumn);
	//snprintf(LineBuffer+StartColumn,strlen(Line),Line);
	memcpy(LineBuffer,Line1,((int)strlen(Line1)>MaxColumn)?MaxColumn:(int)strlen(Line1));
	memcpy(LineBuffer+StartColumn,Line2,((int)strlen(Line2)>MaxColumn)?MaxColumn:(int)strlen(Line2));
	strcat(Buffer,LineBuffer);
	/* New Line Terminator */
	strcat(Buffer,"\n");
	strcat(Buffer,"\x1B""B0");	/* Cancel Inverse Else It Will Print Inverted In Other Applications Also */
	pprintf(Buffer);

	if(!ttestall(PRINTER,500))
		return FALSE;

return TRUE;
}


bool fs_PrinterLRAppendLineLRBold(CONST char *Line1,CONST char *Line2)
{
	char Buffer[FS_PRINTER_MAX_FORMAT_SIZE+1]={0};
	char LineBuffer[FS_PRINTER_MAX_CHARS_CONDENSED+1]={0};
	volatile int MaxColumn=0;
	volatile int StartColumn=0;

	/* Restore Default Settings */
	strcat(Buffer,"\x1B""@");

	/* Set Inverse Characteristics */
	if(PrinterConfig.Inverse==TRUE)
		strcat(Buffer,"\x1B""B1");
	else
		strcat(Buffer,"\x1B""B0");

	/* Set Bold */
/*
	if(PrinterConfig.Bold==TRUE)
		strcat(Buffer,"\x1B""E");
*/
	/* Set Double High */
	if(PrinterConfig.DoubleHigh==TRUE)
		strcat(Buffer,"\x1B""H");

	/* Set Double Wide */
	if(PrinterConfig.CondensedMode==TRUE)
		strcat(Buffer,"\xF");
	else
		strcat(Buffer,"\x12");

	if(PrinterConfig.CondensedMode==TRUE)
		MaxColumn=FS_PRINTER_MAX_CHARS_CONDENSED;
	else
		MaxColumn=FS_PRINTER_MAX_CHARS_NORMAL;

	StartColumn=MaxColumn-(int)strlen(Line2)*2*(PrinterConfig.CondensedMode?2:1);
	StartColumn=(StartColumn<0)? 0:StartColumn;
	memset(LineBuffer,' ',StartColumn);
	LOG_PRINTF("StartCol=%d",StartColumn);
	//snprintf(LineBuffer+StartColumn,strlen(Line),Line);
	memcpy(LineBuffer,Line1,strlen(Line1));
	LOG_PRINTF("LRBold|LineBuffer=%s",LineBuffer);
	strcat(LineBuffer,"\xE");	/* Double Wide */
	LOG_PRINTF("LRBold|LineBuffer=%s",LineBuffer);
	strcat(LineBuffer,Line2);
	LOG_PRINTF("LRBold|LineBuffer=%s",LineBuffer);
	strcat(Buffer,LineBuffer);
	/* New Line Terminator */
	strcat(Buffer,"\n");
	strcat(Buffer,"\x1B""B0");	/* Cancel Inverse Else It Will Print Inverted In Other Applications Also */
	pprintf(Buffer);

	if(!ttestall(PRINTER,500))
		return FALSE;

return TRUE;
}


bool fs_PrinterFormFeed(void)
{
	//fs_PrinterLineFeed();
	fs_PrinterLineFeed();
	fs_PrinterLineFeed();
	return TRUE;

}

bool fs_PrinterLineFeed(void)
{
	pprintf("\x1B""@""\xC\n");
	ttestall(PRINTER,100);
return TRUE;
}
