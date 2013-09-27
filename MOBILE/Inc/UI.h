#ifndef __UI_H_
#define __UI_H_

#include "SDK30.h"
#include "Display.h"

#ifndef CONST
#define CONST const
#endif

typedef struct fs_UserInterfaceData_ {
  int   iDisplayLenMax;              /* Maximum number of characters that can be displayed
                                          per row, given the current font */
  int   iFirstNonTitleRow;           /* First row after the title section */
  int   iTestModeRow;                /* Row used to display "* TEST MODE *" */
  int   iTitleRow;                   /* Row used to display operation title */
  int   iCommRow;                    /* Row used to display communications status prompts */
  int   iPromptRow1;                 /* Standard prompt row */
  int   iPromptRow2;                 /* Standard prompt row */
  int   iPromptRow3;                 /* Standard prompt row */
  int   iPromptRow4;                 /* Standard prompt row */
  int   iBitmapPromptRow1;           /* Bitmap prompt row */
  int   iBitmapPromptRow2;           /* Bitmap prompt row */
  int   iBitmapPromptRow3;           /* Bitmap prompt row */
  int   iBitmapPromptRow4;           /* Bitmap prompt row */
  int   iEntryRow;                   /* Data entry row */
  int   iChoiceRow;                  /* Row used to display softkey choices */
  int   iLineCount;                  /* Number of lines (rows) on the screen, given the
                                          current font */
} fs_UserInterfaceData;


typedef enum
{
entryTypeAlphaNumeric,
entryTypeNumeric,
entryTypePassword,
entryTypeAmount
}entryType;
typedef enum
{
	eDLeftToRight,
	eDRightToLeft
}entryDirection;

bool fs_UIDisplaySuccessPrompts(CONST char *Prompt1, CONST char *Prompt2,
                                CONST char *Prompt3, CONST char *Prompt4, int Timeout);
bool fs_UIClearNonTitlePrompts(void);
bool fs_UIDisplayBitmap(CONST char *LogoFileName);
bool fs_UIDisplayBitmapPrompts(CONST char *, CONST char *, CONST char *, CONST char *);
bool fs_UIDisplayBootScreen(CONST char *, CONST char *, CONST char *, CONST char *);
bool fs_UIDisplayChoicePrompt(CONST char *Prompt);
bool fs_UIDisplayCommPrompt(CONST char *Prompt);
bool fs_UIDisplayErrorPrompts(CONST char *, CONST char *, CONST char *, CONST char *,
                              int Timeout);
bool fs_UIDisplayScreenWithChoicePrompt(CONST char *, CONST char *, CONST char *,
                                        CONST char *);
bool fs_UIDisplayScreenWithCommPrompt(CONST char *FirstLine, CONST char *SecondLine,
                                      CONST char *ThirdLine, CONST char *CommLine);
bool fs_UIDisplayScreen(CONST char *, CONST char *, CONST char *, CONST char *);
bool fs_UIDisplayScreen2(CONST char *, CONST char *, CONST char *, CONST char *);
bool fs_UIDisplayScreen3(CONST char *, CONST char *, CONST char *, CONST char *);
bool fs_UIDisplayTitlePrompts(CONST char *TitleDescription, bool IsTestModeOn);
bool fs_UIGetAlphaInput(char *Title,CONST char *Prompt1, CONST char *Prompt2, char *DataBuffer,
                        int MinLength, int MaxLength, int Timeout);
bool fs_UIGetHiddenNumericInput(CONST char *Prompt1, CONST char *Prompt2, char *DataBuffer,
                                int MinLength, int MaxLength, int Timeout);
bool fs_UIGetHiddenAlphaInput(CONST char *Prompt1, CONST char *Prompt2, char *DataBuffer,
                              int MinLength, int MaxLength, int Timeout);
bool fs_UIGetNumericInput(CONST char *Prompt1, CONST char *Prompt2, char *DataBuffer,
                          int MinLength, int MaxLength, int Timeout);
bool fs_UIGetMaskedNumericInput(CONST char *Prompt1, CONST char *Prompt2, char *DataBuffer,
                                CONST char *Mask, int MinLength, int MaxLength,
                                int Timeout);
bool fs_UIGetDollarInput(CONST char *Prompt1, CONST char *Prompt2, char *DataBuffer,
                         int MinLength, int MaxLength, int Timeout);
void fs_UIGetVersionNumber(char *VersionString);
bool fs_UIInit(void);

bool fs_UIGetInput(char *Title,char *Prompt1,char *Prompt2,daAlign PromptAlign,entryType entry,daAlign EntryAlign, char *DataBuffer,
                        int MinLength, int MaxLength, int Timeout);
bool fs_UIGetCustomInput(char *Title,char *Prompt1,char *Prompt2,daAlign PromptAlign,char *Pattern,daAlign EntryAlign,entryDirection EntryDir,char DefaultChar,int Min,char *DataBuffer,int Timeout);
bool fs_UIGetCustomAmountEntry(char *Title,char *Prompt1,char *Prompt2,daAlign PromptAlign,char *Currency,int Decimal,daAlign EntryAlign,int Min,int Max,double *Amount,int *ipSeparatorList,int Timeout,char *strKeyBuffer);
bool fs_UIFormatDataWithPattern(char *Data,char *Pattern,char *FormattedData,entryDirection EntryDir, char DefaultChar, int *SeparatorList);
int fs_UINumberOfOccurences(char *Buffer,char Character);
unsigned char test_Alphanumerical_Entry(ENTRY_BUFFER *buf_entry);

int ShowEntry (const char *pcTitle, const char *pcText, char *pcInput, byte ucMinDigit, byte ucMaxDigit,
		       byte ucEntryMode);
void LOG_PRINTF(char * frmt,...);
int fs_UIDisplayErrorScreen(char *Title,char *Prompt1,char *Prompt2,char *Prompt3);
bool fs_UIBuzzerPrompt(void);
int fs_UIGetChar(int PromptTimeout);
bool fs_UIBeepNavigateError(void);
bool fs_UISetEntryRow(int Row);
bool fs_UIResetParameters(void);
bool fs_UISetPromptRow(int Row);


#endif
