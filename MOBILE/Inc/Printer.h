#ifndef __PRINTER_H_
#define __PRINTER_H_

#include "SDK30.h"
#include "parms.h"
#include "fsparms.h"
#include "Display.h"
#include "stdlib.h"
#include "Printer.h"

#ifndef CONST
#define CONST const
#endif


#define FS_PRINTER_MAX_FORMAT_SIZE 128

#define FS_PRINTER_MAX_CHARS_WIDE 12
#define FS_PRINTER_MAX_CHARS_NORMAL 24
#define FS_PRINTER_MAX_CHARS_CONDENSED 48

#define FS_PRINTER_JUSTIFY_LEFT		0
#define FS_PRINTER_JUSTIFY_RIGHT	1
#define FS_PRINTER_JUSTIFY_CENTER	2


typedef enum {
  prnsGoodStatus        = 0,
  prnsPaperLevelLow     = 3,
  prnsOutOfPaper        = 1,
  prnsMechanicalFailure = 2
} EPrinterStatusType;

typedef enum {              /* Printer result code enumeration */
   prnrcNone,               /* No result code has been generated */
   prnrcErrPrnAccess,       /* Error gaining access to printer */
   prnrcErrPrnRelease,      /* Error releasing access to printer */
   prnrcErrPrnConfig,       /* Error configuring printer interface */
   prnrcErrPrnOpen,         /* Error opening printer channel */
   prnrcErrPrnClose,        /* Error closing printer channel */
   prnrcErrPrnWrite,        /* Error writing data to printer */
   prnrcErrFileAccess,      /* Error accessing printer file */
   prnrcErrRspTimeout,      /* Error : Printer did not respond to a request within the */
                            /*   specified time-out */
   prnrcErrFmtUnsupported,  /* Selected formatting option unsupported */
   prnrcErrOutOfPaper,      /* Printer returned "Out of Paper" status */
   prnrcBadParam,           /* An invalid parameter was passed to a printer function */
   prnrcPlaceHolder
} EPrinterResultCode;

typedef struct
{
	bool Bold;
	bool DoubleHigh;
	bool DoubleWide;
	bool CondensedMode;
	bool Inverse;
	bool Initialized;
	char Justification;
}prnConfig;

/**************************************************************************************************
* -------------------- P R I V A T E   F U N C T I O N   P R O T O T Y P E S -------------------- *
**************************************************************************************************/

bool fs_PrinterInit(void);
bool fs_PrinterClose(void);
bool fs_PrinterGetLastError(EPrinterResultCode *Result, char *Text, int BuffSize);
bool fs_PrinterIsOutputPending(bool *Pending);
bool fs_PrinterAppendEAN(CONST char *EAN, bool PrintNumericCode, int Height);
bool fs_PrinterAppendUPCVerA(CONST char *UPC, bool PrintNumericCode);
bool fs_PrinterAppendUPCA(CONST char *UPC, bool PrintNumericCode, int Height);
bool fs_PrinterAppendCode39(CONST char *UPC, bool PrintNumericCode, int Height);
bool fs_PrinterAppendHexCode128(CONST char *UPCHex, int Height); /* Hex encoded data */
bool fs_PrinterAppendCode128(CONST char *UPC,  int Height);
bool fs_PrinterAppendBitmap(CONST char *FileName);
bool fs_PrinterAppendLine(CONST char *Line);
bool fs_PrinterAppendRaw(CONST char *pBuffer, int iCount);
bool fs_PrinterFormatLRAppendLine(CONST char *LeftString, CONST char *RightString);
bool fs_PrinterPrintfAppendLine(const char * /*format*/, ...);
bool fs_PrinterFormFeed(void);
bool fs_PrinterLineFeed(void);
bool fs_PrinterResetPrinter(void);
bool fs_PrinterResetAttributes(void);
bool fs_PrinterSetFont(int Font);
bool fs_PrinterSetJustification(int Value);
bool fs_PrinterSetBold(bool Value);
bool fs_PrinterSetCondensed(bool Value);
bool fs_PrinterSetInverse(bool Value);
bool fs_PrinterSetDoubleHigh(bool Value);
bool fs_PrinterSetDoubleWide(bool Value);
bool fs_PrinterSetStrong(bool Value);
bool fs_PrinterCheckOutputPending(bool Value);
bool fs_PrinterPrintBarcodesAsLogos(bool bValue);
bool fs_PrinterGetStatus(EPrinterStatusType *Status);
void fs_PrinterGetVersionNumber(char *VersionString);
bool fs_PrinterDetectPrinterPort(int *PortNumber);
bool fs_PrinterWaitForPrintCompletion(int Timeout);
bool fs_PrinterAppendLineLR(CONST char *Line1,CONST char *Line2);
bool fs_PrinterLRAppendLineLRBold(CONST char *Line1,CONST char *Line2);

#endif
