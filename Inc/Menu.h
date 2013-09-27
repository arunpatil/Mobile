/*
 * Menu.h
 *
 *  Created on: Jan 25, 2013
 *      Author: BNT SOFT
 */

#ifndef MENU_H_
#define MENU_H_

#ifndef NUMBER_OF_ITEMS
#define NUMBER_OF_ITEMS(a) (sizeof(a)/sizeof((a)[0]))
#endif

#define __ENTER_KEY     -1
#define __BACK_KEY      -2
#define __EXIT_KEY      -3

#ifndef __MENU_TABS__
#define __MENU_TABS__

#define _WINDOW_SIZE_LARGE_ 	4
#define _WINDOW_SIZE_MEDIUM_ 	3
#define _WINDOW_SIZE_SMALL_ 	2
#define _WINDOW_SIZE_FULL_ 		0

#define MAX_CUSTOMERS 9


/* Main Menu Tabs */
#define MENU_TAB_MAIN_MENU_ZIPPCARD 0
#define MENU_TAB_MAIN_MENU_MERCHANT 1
#define MENU_TAB_MAIN_MENU_SETUP 2
#define MENU_TAB_MAIN_MENU_EXIT 3

/* Zippcard Menu */
#define MENU_TAB_ZIPPCARD_ACTIVATION 0
#define MENU_TAB_ZIPPCARD_SALE 1
#define MENU_TAB_ZIPPCARD_REFUND 2
#define MENU_TAB_ZIPPCARD_CASH 3
#define MENU_TAB_ZIPPCARD_PAYMENT 4
#define MENU_TAB_ZIPPCARD_ENQUIRY 5
#define MENU_TAB_ZIPPCARD_CURRENCY_EXCHANGE 6
#define MENU_TAB_ZIPPCARD_PIN_CHANGE 7

/* Enquiry Menu */
#define MENU_TAB_ZIPPCARD_BALANCE_ENQUIRY 0
#define MENU_TAB_ZIPPCARD_MINI_STATEMENT 1
#define MENU_TAB_ZIPPCARD_RATE_ENQUIRY 2

/* Activate Menu */
#define MENU_TAB_ZIPPCARD_CARD_ACTIVATION 0
#define MENU_TAB_ZIPPCARD_PRE_LD_CARD_PURCHASE 1
#define MENU_TAB_ZIPPCARD_PRE_LD_CARD_ACTIVATE 2

/* Cash Menu */
#define MENU_TAB_ZIPPCARD_CASH_WITHDRAWAL 0
#define MENU_TAB_ZIPPCARD_CASH_DEPOSIT 1
#define MENU_TAB_ZIPPCARD_CASH_TRANSFER 2
#define MENU_TAB_ZIPPCARD_CASH_COLLECTION 3

/* Payment Menu */
#define MENU_TAB_ZIPPCARD_BILL_PAYMENT 0
#define MENU_TAB_ZIPPCARD_UTILITY_PAYMENT 1
#endif

#define MAX_MENU_ITEMS 10

#define LOCK_LEVEL_1_HOUR 1
#define LOCK_LEVEL_24_HOUR 2

#ifndef NULL
#define NULL 0
#endif

typedef enum
{
	MENU_TYPE_LABEL=1,
	MENU_TYPE_BRANCH,
	MENU_TYPE_IDLE_SWIPE,
	MENU_TYPE_EXIT,
	MENU_TYPE_DELIMITER=255
}_MENU_TYPE;

typedef struct
{
	char *Label;
	_MENU_TYPE Type;
	bool (*PreSelectionFunction)(int);
	int (*Function)(int);
	bool (*PostSelectionFunction)(int);
	bool ExitToMain;
	bool Enabled;
}_MENU_ITEM;

extern _MENU_ITEM MAINMENU[];

typedef struct
{
	long fRecieptAmount;
	long fRecieptTips;
	long fRecieptTotal;
	char sRecieptCreatedat[30];
	char sRecieptAddress[100];
	char sRecieptName[20];
	char sRecieptStatus[20];
	char sRecieptCurrency[20];
	char sRecieptInvoiceID[20];
	char sRecieptChargeID[20];
	char sRecieptCardType[20];
	char sRecieptCardNumber[20];
	char sRecieptChargeRemoteToken[20];
	char sRecieptChargeCreatedat[30];

}stRecieptVar;


typedef struct{
	int bAppInit;
	char szAuthToken[100];
}_global;

typedef struct
{
	int iNoOfCustomers;
	int iSelected;
	char szName[MAX_CUSTOMERS][50];
	char szUserId[MAX_CUSTOMERS][10];
}_CUSTOMER;


typedef enum
{
	ByMobileId,
	ByCustomerName
}eCustSelect;

int ManageMenu( const char *szTitle, int bRadioButtons, int nDefaultChoice,
                int nItems, const char* Items[],int TimeOut );
int MenuShowMainMenu(void);
int InitMenu(_MENU_ITEM *MENULIST,char *szMainTitle);
int SelectFromList( const char *szTitle, int bRadioButtons, int nDefaultChoice,
                int nItems, const char* Items[],int WindowSize,int TimeOut );
bool InitApplicationData(void);
bool checkMerchantPassword(int var);
bool setMerchantPassword(int var);
bool checkDefaultPassword(int var);
bool isMerchantPassWordSet(void);
bool isApplicationLocked(void);


bool OnTab(void);
int OnTabMenu(void);
int SelectOption(void);
int SelectCustomer(void);
int GetPurchaseAmount(void);
int GetTabNumber(void);
int TCPGetAuthToken(void);
int TCPGetCustomerList(void);
int SendRequest(void);
void PrintLog(char strPrint[]);
int ParseResponse(char *pucBuffer,int *InvoiceNo);
int ParseCancelResponse(char *pucBuffer, int *InvoiceNo);
int ParseAuthToken(char *pucBuffer, char *authToken);
int ParseCustomerList(char *pucBuffer);
int ParseApprovalResponse(char *sResponse);
int GetNumericEntry(char *strWindowTitle,char *strPrompt,int iMax,int iMin,char strInputString);
void DrawPrompt(char *Title,char *Text,char *Text1,int Row);
int UserLogin(void);
int GetPassword(void);
int GetUserID(void);
int GetAuthToken(void);
int GetCustomerList(void);
int LoadGlobalData(void);
int SaveGlobalData(void);

#endif /* MENU_H_ */
