#ifndef __PARMS_H
#define __PARMS_H

#include "sdk30.h"
#include "fsparms.h"

#define APASS_LEN_MIN                       4
#define APASS_LEN_MAX                       6
#define MAINTAIN_PASS_LEN_MIN               4
#define MAINTAIN_PASS_LEN_MAX               6
#define DEFAULT_PASS_LEN_MIN               	4
#define DEFAULT_PASS_LEN_MAX               	10
#define HEADER_LINE_MIN                     0
#define HEADER_LINE_MAX                     41
#define FOOTER_LINE_MIN                     0
#define FOOTER_LINE_MAX                     41
#define VCAPP_LEN_MIN                       1
#define VCAPP_LEN_MAX                       10
#define VCTERM_LEN_MIN                      1
#define VCTERM_LEN_MAX                      15
#define VCPHONE_LEN_MIN                     0
#define VCPHONE_LEN_MAX                     18
#define VCBAUD_LEN_MIN                      1
#define VCBAUD_LEN_MAX                      1
#define VCIP_LEN_MIN                        0
#define VCIP_LEN_MAX                        15
#define VCPORT_LEN_MIN                      0
#define VCPORT_LEN_MAX                      5
#define PHONEPBX_LEN_MIN                    0
#define PHONEPBX_LEN_MAX                    4
#define PHONE_LEN_MIN                       0
#define PHONE_LEN_MAX                       30
#define MDMFMAT_LEN_MIN                     3
#define MDMFMAT_LEN_MAX                     3
#define TCPIP_LEN_MIN                       0
#define TCPIP_LEN_MAX                       20
#define TCPPORT_LEN_MIN                     0
#define TCPPORT_LEN_MAX                     5
#define CBAMT_LEN_MIN                       1
#define CBAMT_LEN_MAX                       3
#define LID_LEN_MIN                         0
#define LID_LEN_MAX                         6
#define TID_LEN_MIN                         0
#define TID_LEN_MAX                         8
#define SID_LEN_MIN                         5
#define SID_LEN_MAX                         5
#define MID_LEN_MIN                         0
#define MID_LEN_MAX                         15
#define BATCHNUM_LEN_MIN                    1
#define BATCHNUM_LEN_MAX                    2
#define SHIFTNUM_LEN_MIN                    1
#define SHIFTNUM_LEN_MAX                    2
#define SEQNUM_LEN_MIN                      1
#define SEQNUM_LEN_MAX                      6
#define MERCHTYPE_MIN                       1
#define MERCHTYPE_MAX                       4
#define MAX_TERMS                           10
#define TERMS_TEXT_LEN_MIN                  0
#define TERMS_TEXT_LEN_MAX                  15
#define CUR_CODE_MIN						3
#define CUR_CODE_MAX						3
#define MERCH_NAME_MIN						0
#define MERCH_NAME_MAX						20
#define MERCH_ADDRESS_MIN					0
#define MERCH_ADDRESS_MAX					20
#define SELECT_LIST_MIN						0
#define SELECT_LIST_MAX						40



#define HEADER_LINE_COUNT                   5
#define FOOTER_LINE_COUNT                   5
#define MERCH_ADDRESS_COUNT                 3
#define CBAMT_COUNT                         4

#define KB_NOT_PRESENT                      0
#define KB_100                              1
#define PORT_COM1                           1
#define PORT_COM2                           2
#define PP_NOT_PRESENT                      0
#define PP_INTERNAL                         1
#define PP_EXTERNAL                         2
#define CR_NOT_PRESENT                      0
#define CR_600                              1
#define CR_1000I                            2
#define CB_PROMPT_NONE                      0
#define CB_PROMPT_SPECIFIC                  1
#define CB_PROMPT_PREDEFINED                2
#define CONFIRM_TOTAL_NONE                  0
#define CONFIRM_TOTAL_IF_CB                 1
#define CONFIRM_TOTAL_ALWAYS                2
#define RECEIPT_NONE                        0
#define RECEIPT_CUSTOMER                    1
#define RECEIPT_MERCHANT                    2
#define RECEIPT_CUSTOMER_MERCHANT           3
#define RECEIPT_MERCHANT_CUSTOMER           4
#define COMMTYPE_MODEM                      'M'
#define COMMTYPE_ETHERNET                   'E'

// Specific 
#define CBFEE_MIN							1
#define CBFEE_MAX							12
#define MINCBAMT_MIN						0
#define MINCBAMT_MAX						5
#define MAXAMT_MIN							0
#define MAXAMT_MAX							9
#define MACKEY_MIN							0
#define MACKEY_MAX							12
#define	PINKEY_MIN							0
#define PINKEY_MAX							12
#define PROG_LABEL_MIN						5
#define PROG_LABEL_MAX						21
#define SW_VER_MIN							0
#define SW_VER_MAX							3
#define PASSWORD_MIN						4
#define PASSWORD_MAX						9
#define	MGRPWD_MIN							4
#define MGRPWD_MAX							9
#define AMOUNT_TABLE_MIN					0
#define AMOUNT_TABLE_MAX					30
#define TIMEZONE_MIN						0
#define TIMEZONE_MAX						3
#define AUTOCLOSE_MIN						4
#define AUTOCLOSE_MAX						4
#define HOSTPHONE1_MIN						7
#define HOSTPHONE1_MAX						10
#define HOSTPHONE2_MIN						7
#define HOSTPHONE2_MAX						10
#define PBAX_MIN							0
#define PBAX_MAX							5
#define NPAUSES_MIN							0
#define NPAUSES_MAX							2
#define MODEM_MIN							6
#define MODEM_MAX							10
#define MODEMINIT_MIN						18
#define MODEMINIT_MAX						25
#define MDMBAUD_MIN							7
#define	MDMBAUD_MAX							8
#define MEDIA_MIN							8
#define	MEDIA_MAX							8
#define	IPADDR_MIN							8
#define	IPADDR_MAX							16
#define PORTNO_MIN							1
#define PORTNO_MAX							5
#define ZP_MIN								10
#define ZP_MAX								15
#define ZPSEC_MIN							10
#define ZPSEC_MAX							15
#define ZA_MIN								4
#define	ZA_MAX								10
#define	ZT_MIN								4
#define ZT_MAX								15
#define MDMFORMAT_MIN						8
#define MDMFORMAT_MAX						8
#define SURCHARGE_LEN_MIN					0
#define SURCHARGE_LEN_MAX					4
#define FIXED_FEE_LEN_MIN					0
#define FIXED_FEE_LEN_MAX					4
#define ZIPCODE_LEN							5
#define DATETIME_LEN_MAX					14

#ifndef FS_DATE_TIME_LEN_MAX
#define FS_DATE_TIME_LEN_MAX 16 /* DDMMYYHHMMSSwyyy 	w=weekday, yyy=day of the year*/
#endif

#define MAX_CURRENCIES_COUNT 20


typedef struct ParameterDataType_ {

  /* Miscellaneous Passwords */
  char  AdminPassword[APASS_LEN_MAX+1];
  char  MaintainPassword[MAINTAIN_PASS_LEN_MAX+1];
  char  DefaultPassword[DEFAULT_PASS_LEN_MAX+1];

  /* General Parameters */
  char  HeaderLines[HEADER_LINE_COUNT][HEADER_LINE_MAX+1];
  char  FooterLines[FOOTER_LINE_COUNT][FOOTER_LINE_MAX+1];
  char  TerminalID[TID_LEN_MAX+1];
  char  MerchantID[MID_LEN_MAX+1];
  //char  CurrencyCode[CUR_CODE_MAX+1];
  char  MerchantName[MERCH_NAME_MAX+1];
  char  MerchantAddress[MERCH_ADDRESS_COUNT][MERCH_ADDRESS_MAX+1];
  _SelectList slCurrecyList;

  /* Host Setup */
  char  TCPHostIPAddr[TCPIP_LEN_MAX+1];
  long  TCPHostPort;

  /* Utility Parameters */
  bool  ResultBeep;
  bool  PromptBeep;
  int   MenuActivityTimeout;
  int   PromptTimeout;

  /* Application Specific : For internal use only */
  bool  bMerchantPassWordSet;
  int   iLockLevel;
  int   iDefaultPasswordTries;
  char  strUnlockDateTime[FS_DATE_TIME_LEN_MAX+1];

#if 0
  char  DownloadApp[VCAPP_LEN_MAX+1];
  char  DownloadTerm[VCTERM_LEN_MAX+1];
  char  DownloadPhone[VCPHONE_LEN_MAX+1];
  int   DownloadBaud;
  char  DownloadIP[VCIP_LEN_MAX+1];
  char  DownloadPort[VCPORT_LEN_MAX+1];
  int   DemoMode;
  bool  CheckExpDate;
  char  CommType;
  char  CommTypePref;
  int   CommTries;
  bool  CommAutoDetect;
  int   CommConnectTimeout;
  int   CommENQTimeout;
  int   CommResponseTimeout;
  int   CommEOTTimeout;
  char  PhoneAccessCode[PHONEPBX_LEN_MAX+1];
  char  PhonePrimary[PHONE_LEN_MAX+1];
  char  PhoneSecondary[PHONE_LEN_MAX+1];
  int   ModemBaud;
  char  ModemCommFormat[MDMFMAT_LEN_MAX+1];
  int   ModemDialType;
  char  TCPHostIPAddr1[TCPIP_LEN_MAX+1];
  long  TCPHostPort1;
  char  TCPHostIPAddr2[TCPIP_LEN_MAX+1];
  long  TCPHostPort2;
  char  TCPTermDNS1Addr[TCPIP_LEN_MAX+1];
  char  TCPTermDNS2Addr[TCPIP_LEN_MAX+1];
  char  TCPTermGatewayAddr[TCPIP_LEN_MAX+1];
  char  TCPTermSubnetMask[TCPIP_LEN_MAX+1];
  char  TCPTermAddr[TCPIP_LEN_MAX+1];
  bool  KeyBeep;
  bool  ResultBeep;
  bool  PromptBeep;
  int   MenuActivityTimeout;
  int   PromptTimeout;
  int   ClerkIDPrompt;
  int   PwdRetry;
  bool  NewParamIndicator;

  /* App specific params start here */

  int   CRType;
  int   CRPort;
  int   KBType;
  int   KBPort;
  int   PPType;
  int   PPPort;
  int   CashBackPrompt;
  char  FixedCashBackAmounts[CBAMT_COUNT][CBAMT_LEN_MAX+1];
  int   ConfirmAmountPrompt;
  bool  MaskAccount;
  bool  MaskExpDate;
  int   ReceiptOptions;
  bool  ReceiptPrompt;
  char  LocationID[LID_LEN_MAX+1];
  char  TerminalID[TID_LEN_MAX+1];
  char  StoreID[SID_LEN_MAX+1];
  char  MerchantID[MID_LEN_MAX+1];
  int   BatchNumber;
  int   ShiftNumber;
  long  SequenceNumber;
  long  PaperSequenceNumber;
  long  QuickSpecialPostingNumber;
  long  SignupSequenceNumber;
  long  DemoSequenceNumber;
  char  MerchantType[MERCHTYPE_MAX+1];
  char  AutoBatchCloseDateTime[DATETIME_LEN_MAX+1];
  char  AutoDownloadDateTime[DATETIME_LEN_MAX+1];
  int   AutoDownloadType;
  char  AutoUpdateConfigDateTime[DATETIME_LEN_MAX+1];
  char  TermsText[MAX_TERMS][TERMS_TEXT_LEN_MAX+1];
  int   DefaultTerms;
  bool  Unlocked;

  /* Not spec'ed */

  char  MenuLine1[16+1];        /* Used in place of the first element. */
  int   BatchFill;              /* # of times to append a batch record.  Used for testing. */
  int   PrinterType;
  int   PrinterPort;
  long  EESLSendRetries;
  long  EESLRetryInterval;
  long  EventPollInterval;
  bool  Debug;
  bool  ManualMICREntry;
  int   CRSendTimeout;
  int   BatchRecSize;
  long  BatchRecCount;

  /* Internal use */
  //char  ApplicationParent[FS_FILE_NAME_MAX_LEN + 1];
  bool  BinInit;
  //CardBINDataType CardBINs[MAX_BINS];
  int   CommActive;
  char  EthType;
  short WiFiChannel;
  //char  WiFiSSID[FS_TCP_WIFI_MAX_SSID_SIZE];
  short WiFiWEPMode;
  short WiFiWEPKeyIndex;
  //char  WiFiWEPKey[FS_TCP_WIFI_MAX_WEP_KEY_VALUE];
  bool  SecretPassword;

  /*	 Parameters		*/
  char	strCBFEE[CBFEE_MAX+1];
  bool	bCBPROMPT;
  char	strMINCBAMT[MINCBAMT_MAX+1];
  char	strMAXAMT[MAXAMT_MAX+1];
  char	strMACKEY[MACKEY_MAX+1];
  char	strPINKEY[PINKEY_MAX+1];
  bool	bEXTPINPAD;
  char	strPROG_LABEL[PROG_LABEL_MAX+1];
  char	strSW_VER[SW_VER_MAX+1];
  char	strPASSWORD[PASSWORD_MAX+1];
  char	strMGRPWD[MGRPWD_MAX];
  bool  bDUP_RECEIPT;
  char	strAMOUNT_TABLE[AMOUNT_TABLE_MAX];
  bool	bEXACT_AMOUNT;
  bool	bSIGN_LINE;
  char	strTIMEZONE[TIMEZONE_MAX+1];
  char	strAUTOCLOSE[AUTOCLOSE_MAX+1];
  char	strHOSTPHONE1[HOSTPHONE1_MAX+1];
  char	strHOSTPHONE2[HOSTPHONE2_MAX+1];
  char	strPBAX[PBAX_MAX+1];
  char	strNPAUSES[NPAUSES_MAX+1];
  char	strMODEM[MODEM_MAX+1];
  char	strMODEMINIT[MODEMINIT_MAX+1];
  char	strMDMBAUD[MDMBAUD_MAX+1];
  char	strMEDIA[MEDIA_MAX+1];
  char	strIPADDR[IPADDR_MAX+1];
  char	strPORTNO[PORTNO_MAX+1];
  char	strZP[ZP_MAX+1];
  char	strZPSEC[ZPSEC_MAX+1];
  char	strZA[ZA_MAX+1];
  char	strZT[ZT_MAX+1];
  char	strMDMFORMAT[MDMFORMAT_MAX+1];

  int	cntAmtScreens;
  int	AmtScreenLevel;
  int	numAmtOptions;
  bool  exactAmtSelected;
  bool  IsTerminalEnrolled;
  char  strSurcharge[SURCHARGE_LEN_MAX+1];
  char  strFixedFee[FIXED_FEE_LEN_MAX+1];
  char  ZipCode[ZIPCODE_LEN+1];
#endif

  //BatchRecordType ReversalRecord;
} ParameterDataType;

extern ParameterDataType  glParameters;
extern ParameterTableType glParameterTable[];

bool fs_DisplayParmsError(void);
S_FS_FILE* OpenFlashFile(char *VolumeLabel,char *FileName,bool bForceCreate);
S_FS_FILE* OpenRamFile(char *VolumeLabel,char *FileName);
bool fs_ExtractParametersFromFile(char *ParameterFile);
void fs_RemoveLeftPadding(char *buf, char pad_char);
void fs_RemoveRightPadding(char *buf);
bool fs_PutDldParameter(char *parameter,char *value);
bool fs_ReadOneLine(S_FS_FILE *fptr,char *buffer,int size);
bool fs_ParmExtractListContents(char *inputString,_SelectList *slSelectList);
char  *fs_ParmGetSelectedItem(_SelectList *slSelectList);
#endif /* __PARMS_H */

