#include "SDK30.H"
#include "Menu.h"
#include "Display.h"
#include "fsparms.h"
#include "parms.h"
#include "ui.h"
#include "Printer.h"
#include "Utils.h"
#include "Common.h"
#include "Sale.h"
#include "TlvTree.h"
#include "LinkLayer.h"
#include <SSL_.h>
#include <stdlib.h>
#include <ctype.h>

#define RESEND 2;

int ZippcardFunction(int argument);
bool fs_ParmEdit(int PromptTimeout);
int ConnectEthernet();
int PerformConnectionSSL(const char *szHostName, unsigned int nPort,
		const char *szProfile);
void AddProfile(char szName[]);
void PrintReceipt(int status);
int ParseApprovalResponse(char *sResponse);

char App_Name[10] = "MOBILE";
char strRequestBuffer[4096];
char strTabNum[5];
static char gUserID[8];
static char gPassword[9];
static char gAuthToken[100];
double fAmount;
char strAmount[22];
char strTransactionType[10];
stRecieptVar sRecieptVar;
_CUSTOMER customerList;
eCustSelect IDTypeSelection;
_global globalData;



#undef LOG_PRINTF
#define LOG_PRINTF(...) {}


/**
 * Declaration for ethernet connection By Ashish on 30May
 */
#define MAX_SND  512
#define MAX_RSP  512
#define TCPIP_TIMEOUT 30*100
#define __CHN_SEPARATOR__  '|'

static LL_HANDLE tvSession[1] = { NULL };
static TLV_TREE_NODE piConfig = NULL;
static TLV_TREE_NODE piPhysicalConfig = NULL;
static TLV_TREE_NODE piTransportConfig = NULL;

bool CheckSetupPassword(int Var) {
	static int Counter = 0;

	_clrscr();

	Counter++;
	if (Counter % 2)
		printf("SETUP PASSWORD\nVERIFIED");
	else
		printf("INVALID\nSETUP PASSWORD");

	ttestall(0, 2 * 100); // Wait for 2s.

	return (bool) (Counter % 2);
}

int SetupFunction(int Var) {
	FILE *hDisplay;
	char EntryBuffer[100] = { 0 };
	char *Items[] = { "ITEM0", "ITEM1", "ITEM2", "ITEM3", "ITEM4", "ITEM5",
			"ITEM6", "ITEM7", "ITEM8", "ITEM9" };

	hDisplay = fopen("DISPLAY", "w");

	fs_PrinterInit();
	/*
	 fs_DisplayInit();
	 fs_UIInit();
	 */
	/*
	 printf("ENTERED SETUP\nFUNCTION");
	 ttestall(0, 3*100);                    // Wait for 2s.
	 */
	//clrscr();
	/*
	 fs_DisplayAtRowReverse(0,"LINE0",daAlignCenter);
	 fs_DisplayAtRow(1,"LINE1 CENTER",daAlignCenter);
	 fs_DisplayAtRow(2,"LINE2 RIGHT",daAlignRight);
	 fs_DisplayAtRow(3,"LINE3",daAlignCenter);
	 fs_DisplayAtRow(4,"LINE4",daAlignCenter);
	 fs_DisplayAtRow(5,"LINE5",daAlignCenter);
	 fs_DisplayAtRow(6,"LINE6",daAlignCenter);
	 ttestall(0,3*100);
	 */
	//memset(&glParameters,0,sizeof(ParameterDataType));
	//gotoxy(0,0);
	/*
	 printf("Selected=%d",SelectFromList( "TITLE", TRUE, 5,10,(const char **)Items,_WINDOW_SIZE_SMALL_,30 ));
	 ttestall(0,200);

	 gotoxy(0,0);
	 printf("Selected=%d",SelectFromList( "TITLE", TRUE, 5,10,(const char **)Items,_WINDOW_SIZE_MEDIUM_,30 ));
	 ttestall(0,200);

	 gotoxy(0,0);
	 printf("Selected=%d",SelectFromList( "TITLE", TRUE, 5,10,(const char **)Items,_WINDOW_SIZE_LARGE_,30 ));
	 ttestall(0,200);

	 gotoxy(0,0);
	 printf("Selected=%d",SelectFromList( "TITLE", TRUE, 5,10,(const char **)Items,_WINDOW_SIZE_FULL_,30 ));
	 ttestall(0,200);


	 if(
	 !fs_ParmInit("ZIPP", glParameterTable, (char *)&glParameters, sizeof(ParameterDataType)) ||
	 //!fs_ParmSetup(FALSE) ||
	 //!fs_ParmSaveToStore()	||
	 !fs_ParmLoadFromStore()
	 )
	 fs_DisplayParmsError();

	 fs_DisplayAtRowReverse(ROW2,"Admin Password",daAlignCenter);
	 fs_DisplayAtRow(ROW3,glParameters.AdminPassword,daAlignCenter);
	 fs_DisplayAtRowReverse(ROW4,"Header 1",daAlignCenter);
	 fs_DisplayAtRow(ROW5,glParameters.HeaderLines[0],daAlignCenter);

	 fs_UIGetInput("SALE","ENTER AMOUNT","CHARACTERS",daAlignLeft,entryTypeAmount,
	 daAlignLeft,EntryBuffer,5,10,60) &&

	 fs_UIGetInput("SALE","ENTER AMOUNT","CHARACTERS",daAlignRight,entryTypeAmount,
	 daAlignRight,EntryBuffer,5,10,60) &&

	 fs_UIGetInput("SALE","ENTER AMOUNT","CHARACTERS",daAlignCenter,entryTypeAmount,
	 daAlignCenter,EntryBuffer,5,10,60);


	 fs_UIGetInput("SALE","ENTER ALPHANUM","CHARACTERS",daAlignLeft,entryTypeAlphaNumeric,
	 daAlignLeft,EntryBuffer,5,10,60) &&

	 fs_UIGetInput("SALE","ENTER ALPHANUM","CHARACTERS",daAlignRight,entryTypeAlphaNumeric,
	 daAlignRight,EntryBuffer,5,10,60) &&

	 fs_UIGetInput("SALE","ENTER ALPHANUM","CHARACTERS",daAlignCenter,entryTypeAlphaNumeric,
	 daAlignCenter,EntryBuffer,5,10,60);

	 fs_UIGetInput("SALE","ENTER NUMERIC","CHARACTERS",daAlignLeft,entryTypeNumeric,
	 daAlignLeft,EntryBuffer,5,10,60) &&

	 fs_UIGetInput("SALE","ENTER NUMERIC","CHARACTERS",daAlignRight,entryTypeNumeric,
	 daAlignRight,EntryBuffer,5,10,60) &&

	 fs_UIGetInput("SALE","ENTER NUMERIC","CHARACTERS",daAlignCenter,entryTypeNumeric,
	 daAlignCenter,EntryBuffer,5,10,60);

	 fs_UIGetInput("SALE","ENTER PASSWORD","CHARACTERS",daAlignLeft,entryTypePassword,
	 daAlignLeft,EntryBuffer,5,5,60) &&

	 fs_UIGetInput("SALE","ENTER PASSWORD","CHARACTERS",daAlignRight,entryTypePassword,
	 daAlignRight,EntryBuffer,5,5,60) &&

	 fs_UIGetInput("SALE","ENTER PASSWORD","CHARACTERS",daAlignCenter,entryTypePassword,
	 daAlignCenter,EntryBuffer,5,5,60);
	 */
	/*
	 ShowEntry("ENTER 0","ENTER VALUE",EntryBuffer,5,10,0);
	 ShowEntry("ENTER 1","ENTER VALUE",EntryBuffer,5,10,1);
	 ShowEntry("ENTER 2","ENTER VALUE",EntryBuffer,5,10,2);
	 if(strlen(EntryBuffer))
	 {
	 fs_DisplayClearAll();
	 fs_DisplayAtRow(ROW5,EntryBuffer,daAlignCenter);
	 ttestall(0,3*100);
	 }
	 */
	/*
	 //fs_PrinterSetBold(TRUE)&&
	 fs_PrinterLRAppendLineLR("left","right") &&
	 fs_PrinterLRAppendLineLRBold("left","right") &&
	 fs_PrinterLRAppendLineLR("1.30","") &&
	 fs_PrinterLRAppendLineLR("","") &&
	 fs_PrinterSetCondensed(TRUE) &&
	 fs_PrinterLRAppendLineLRBold("left","right") &&
	 fs_PrinterSetDoubleWide(TRUE)&&
	 fs_PrinterSetInverse(TRUE)&&
	 fs_PrinterSetDoubleHigh(TRUE)&&
	 fs_PrinterLRAppendLineLR("","12.30") &&
	 fs_PrinterAppendLine("LEFT")&&
	 fs_PrinterLRAppendLineLRBold("left","right") &&
	 fs_PrinterAppendLine("left")&&
	 fs_PrinterSetInverse(FALSE)&&
	 //fs_PrinterAppendLine("ABCDEFGHIJKLMNOPQRSTUVWXYZ")&&
	 fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_CENTER)&&
	 fs_PrinterLRAppendLineLRBold("left","right") &&
	 fs_PrinterSetDoubleHigh(FALSE)&&
	 fs_PrinterAppendLine("CENTER")&&
	 fs_PrinterLRAppendLineLR("left","right") &&
	 fs_PrinterSetCondensed(FALSE) &&
	 fs_PrinterLRAppendLineLRBold("left","right") &&
	 fs_PrinterAppendLine("center")&&
	 //fs_PrinterAppendLine("ABCDEFGHIJKLMNOPQRSTUVWXYZ")&&
	 fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_RIGHT)&&
	 fs_PrinterAppendLine("RIGHT")&&
	 fs_PrinterLRAppendLineLRBold("left","right") &&
	 fs_PrinterAppendLine("right")&&
	 //fs_PrinterAppendLine("ABCDEFGHIJKLMNOPQRSTUVWXYZ")&&
	 fs_PrinterSetBold(FALSE)&&
	 //fs_PrinterSetDoubleWide(FALSE)&&
	 fs_PrinterSetDoubleHigh(FALSE)&&
	 fs_PrinterLRAppendLineLRBold("Transaction:","SALE") &&
	 fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_LEFT)&&
	 fs_PrinterSetInverse(TRUE)&&
	 fs_PrinterLRAppendLineLRBold("Amount:","$1500.98") &&
	 fs_PrinterAppendLine("LEFT")&&
	 fs_PrinterSetInverse(FALSE)&&
	 fs_PrinterAppendLine("left")&&
	 fs_PrinterLRAppendLineLR("LEFT","RIGHT") &&
	 fs_PrinterLRAppendLineLRBold("left","RIGHT") &&
	 fs_PrinterSetInverse(FALSE)&&
	 //fs_PrinterAppendLine("ABCDEFGHIJKLMNOPQRSTUVWXYZ")&&
	 fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_CENTER)&&
	 fs_PrinterSetInverse(TRUE)&&
	 fs_PrinterAppendLine("CENTER")&&
	 fs_PrinterLRAppendLineLRBold("Card Type:","Credit") &&
	 fs_PrinterLRAppendLineLR("LEFT","RIGHT") &&
	 fs_PrinterSetInverse(FALSE)&&
	 fs_PrinterAppendLine("center")&&
	 //fs_PrinterAppendLine("ABCDEFGHIJKLMNOPQRSTUVWXYZ")&&
	 fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_RIGHT)&&
	 fs_PrinterLRAppendLineLRBold("left","right") &&
	 fs_PrinterAppendLine("RIGHT")&&
	 fs_PrinterLRAppendLineLR("left","right") &&
	 fs_PrinterAppendLine("right")&&
	 fs_PrinterSetInverse(TRUE)&&
	 fs_PrinterLRAppendLineLRBold("left","right") &&
	 fs_PrinterLRAppendLineLR("left","right") &&
	 fs_PrinterAppendLine("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	 fs_PrinterFormFeed();
	 */
	fs_PrinterClose();

	fclose(hDisplay);

	return TRUE;
}

int BillPaymentFunction(int Var) {
	FILE *hDisplay;

	hDisplay = fopen("DISPLAY", "w");

	printf("ENTERED BILL\nPAYMENT");
	ttestall(0, 3 * 100); // Wait for 2s.

	fclose(hDisplay);

	return TRUE;
}

int UtilityPaymentFunction(int Var) {
	FILE *hDisplay;

	hDisplay = fopen("DISPLAY", "w");

	printf("ENTERED UTILITY\nPAYMENT");
	ttestall(0, 3 * 100); // Wait for 2s.

	fclose(hDisplay);

	return TRUE;
}

_MENU_ITEM MAINMENU[] =
		{
				{ "ZIPPCARD", MENU_TYPE_BRANCH, NULL, ZippcardFunction, NULL,
						NULL, 1 },
				{ "ACTIVATE", MENU_TYPE_BRANCH, NULL, ZippcardFunction, NULL,
						NULL, 1 },
				{ "CARD ACTIVATION", MENU_TYPE_LABEL, NULL, ZippcardFunction,
						NULL, NULL, 1 },
				{ "PRE LOAD PURCHASE", MENU_TYPE_LABEL, NULL, ZippcardFunction,
						NULL, NULL, 1 },
				{ "PRE LOAD ACTIVATION", MENU_TYPE_LABEL, NULL,
						ZippcardFunction, NULL, NULL, 1 },
				{ "", MENU_TYPE_DELIMITER, NULL, NULL, NULL, NULL, NULL }, /* Delimiter */
				{ "SALE", MENU_TYPE_LABEL, NULL, doSale, NULL, NULL, 1 },
				{ "REFUND", MENU_TYPE_LABEL, NULL, ZippcardFunction, NULL, NULL,
						1 },
				{ "CASH", MENU_TYPE_BRANCH, NULL, ZippcardFunction, NULL, NULL,
						1 },
				{ "CASH WITHDRAWAL", MENU_TYPE_LABEL, NULL, NULL, NULL, NULL, 1 },
				{ "CASH DEPOSIT", MENU_TYPE_LABEL, NULL, NULL, NULL, NULL, 1 },
				{ "CASH COLLECTION", MENU_TYPE_LABEL, NULL, NULL, NULL, NULL, 1 },
				{ "", MENU_TYPE_DELIMITER, NULL, NULL, NULL, NULL, NULL }, /* Delimiter */
				{ "CARD TRANSFER", MENU_TYPE_LABEL, NULL, ZippcardFunction,
						NULL, NULL, 1 },
				{ "PAYMENTS", MENU_TYPE_BRANCH, NULL, ZippcardFunction, NULL,
						NULL, 1 },
				{ "BILL PAYMENT", MENU_TYPE_LABEL, NULL, BillPaymentFunction,
						NULL, NULL, 1 },
				{ "UTILITY PAYMENT", MENU_TYPE_LABEL, NULL,
						UtilityPaymentFunction, NULL, NULL, 1 },
				{ "", MENU_TYPE_DELIMITER, NULL, NULL, NULL, NULL, NULL }, /* Delimiter */
				{ "ENQUIRY", MENU_TYPE_BRANCH, NULL, ZippcardFunction, NULL,
						NULL, 1 },
				{ "BALANCE ENQUIRY", MENU_TYPE_LABEL, NULL, BillPaymentFunction,
						NULL, NULL, 1 },
				{ "MINI STATEMENT", MENU_TYPE_LABEL, NULL,
						UtilityPaymentFunction, NULL, NULL, 1 },
				{ "RATE ENQUIRY", MENU_TYPE_LABEL, NULL, UtilityPaymentFunction,
						NULL, NULL, 1 },
				{ "CURRENCY EXCHANGE", MENU_TYPE_LABEL, NULL, ZippcardFunction,
						NULL, NULL, 1 },
				{ "", MENU_TYPE_DELIMITER, NULL, NULL, NULL, NULL, NULL }, /* Delimiter */
				{ "PIN CHANGE", MENU_TYPE_LABEL, NULL, ZippcardFunction, NULL,
						NULL, 1 },
				{ "", MENU_TYPE_DELIMITER, NULL, NULL, NULL, NULL, NULL }, /* Delimiter */
				{ "MERCHANT", MENU_TYPE_BRANCH, checkMerchantPassword,
						ZippcardFunction, NULL, NULL, 1 },
				{ "PROCESS REVERSAL", MENU_TYPE_LABEL, NULL, ZippcardFunction,
						NULL, NULL, 1 },
				{ "SETTLEMENT", MENU_TYPE_LABEL, NULL, ZippcardFunction, NULL,
						NULL, 1 },
				{ "KEY EXCHANGE", MENU_TYPE_LABEL, NULL, ZippcardFunction, NULL,
						NULL, 1 },
				{ "REPRINT LAST", MENU_TYPE_LABEL, NULL, ZippcardFunction, NULL,
						NULL, 1 },
				{ "UPDATE APPLICATION", MENU_TYPE_LABEL, NULL, ZippcardFunction,
						NULL, NULL, 1 },
				{ "APPLICATION VERSION", MENU_TYPE_LABEL, NULL,
						ZippcardFunction, NULL, NULL, 1 },
				{ "", MENU_TYPE_DELIMITER, NULL, NULL, NULL, NULL, NULL }, /* Delimiter */
				{ "SETUP", MENU_TYPE_LABEL, NULL, fs_ParmEdit, NULL, NULL, 1 },
				{ "EXIT", MENU_TYPE_EXIT, NULL, ZippcardFunction, NULL, NULL, 1 },
				{ "", MENU_TYPE_DELIMITER, NULL, NULL, NULL, NULL, NULL } /* Delimiter */
		};

/*

 static const char *MenuListMain[] =
 {
 "ZIPPCARD",
 "MERCHANT",
 "SETUP",
 "EXIT"
 };

 static const char *MenuListZippcard[] =
 {
 "ACTIVATE",
 "SALE",
 "REFUND",
 "CASH",
 "PAYMENTS",
 "ENQUIRY",
 "CURRENCY EXCHANGE",
 "PIN CHANGE"
 };

 static const char *MenuListMerchant[] =
 {
 "PROCESS REVERSAL",
 "SETTLMENT",
 "KEY EXCHANGE",
 "REPRINT LAST",
 "UPDATE APPLICATION",
 "APPLICATION VERSION"
 };

 static const char *MenuListSetup[] =
 {
 "MERCHANT ID",
 "TERMINAL ID",
 "CURRENCY CODE",
 "MERCHANT NAME",
 "MERCHANT ADDRESS",
 "MERCHANT PASSWORD",
 "IP ADDRESS",
 "PORT NUMBER",
 "BATCH NUMBER",
 "MAINTENANCE PASSWORD"
 };

 static const char *MenuListActivate[] =
 {
 "CARD ACTIVATION",
 "PRE LOAD PURCHASE",
 "PRE LOAD ACTIVATION"
 };

 static const char *MenuListEnquiry[] =
 {
 "RATE ENQUIRY",
 "BALANCE ENQUIRY",
 "MINI STATEMENT"
 };

 static const char *MenuListPayment[] =
 {
 "BILL PAYMENT",
 "UTILITY PAYMENT"
 };

 static const char *MenuListCash[] =
 {
 "CASH WITHDRAWAL",
 "CASH DEPOSIT",
 "CASH TRANSFER"
 };


 int MenuShowMainMenu(void)
 {
 FILE *hDisplay;
 int bContinueLevel0=1;
 int bContinueLevel1=1;
 int bContinueLevel2=1;
 int iSelect=0;
 static int iDefaultZippcardMenu=0;
 static int iDefaultMainMenu=0;
 static int iDefaultCashMenu=0;
 static int iDefaultEnquiryMenu=0;
 static int iDefaultPaymentMenu=0;
 static int iDefaultActivateMenu=0;

 // Menu management
 hDisplay =fopen("DISPLAY", "w");                                  // Open display driver
 do
 {
 switch(ManageMenu("ZIPPCARD", 0, iDefaultMainMenu, NUMBER_OF_ITEMS(MenuListMain), MenuListMain))
 {
 case MENU_TAB_MAIN_MENU_ZIPPCARD:
 bContinueLevel1=1;
 while(bContinueLevel1==1)
 {
 iSelect=ManageMenu("ZIPP TRANSACTIONS", 0, iDefaultZippcardMenu, NUMBER_OF_ITEMS(MenuListZippcard), MenuListZippcard);
 switch(iSelect)
 {
 case MENU_TAB_ZIPPCARD_ACTIVATION:
 iDefaultActivateMenu=MENU_TAB_ZIPPCARD_ACTIVATION;
 while(bContinueLevel1==1)
 {
 iSelect=ManageMenu("ZIPP TRANSACTIONS", 0, iDefaultZippcardMenu, NUMBER_OF_ITEMS(MenuListZippcard), MenuListZippcard);
 switch(iSelect)
 {
 case MENU_TAB_ZIPPCARD_ACTIVATION:
 iDefaultActivateMenu=MENU_TAB_ZIPPCARD_ACTIVATION;
 break;

 case MENU_TAB_ZIPPCARD_SALE:
 iDefaultZippcardMenu=MENU_TAB_ZIPPCARD_SALE;
 break;

 default:
 bContinueLevel1=0;
 break;
 }
 }
 bContinueLevel0=1;
 break;



 break;

 case MENU_TAB_ZIPPCARD_SALE:
 iDefaultZippcardMenu=MENU_TAB_ZIPPCARD_SALE;
 break;

 default:
 bContinueLevel1=0;
 break;
 }
 }
 bContinueLevel0=1;
 break;

 case MENU_TAB_MAIN_MENU_MERCHANT:
 bContinueLevel1=1;
 while(bContinueLevel1==1)
 switch(ManageMenu("MERCHANT MENU", 0, iDefaultMainMenu, NUMBER_OF_ITEMS(MenuListMerchant), MenuListMerchant))
 {

 case MENU_TAB_ZIPPCARD_CARD_ACTIVATION:
 break;

 default:
 bContinueLevel1=0;
 break;

 }
 bContinueLevel0=1;
 break;

 case MENU_TAB_MAIN_MENU_SETUP:
 bContinueLevel1=1;
 while(bContinueLevel1==1)
 switch(ManageMenu("SETUP MENU", 0, iDefaultMainMenu, NUMBER_OF_ITEMS(MenuListSetup), MenuListSetup))
 {

 case 0:
 break;

 default:
 bContinueLevel1=0;
 break;

 }
 bContinueLevel0=1;
 break;

 case MENU_TAB_MAIN_MENU_EXIT:
 bContinueLevel0=0;
 break;

 default:
 bContinueLevel0=1;
 break;                                  // Abort key pressed
 }
 } while(bContinueLevel0==1);

 fclose(hDisplay);                                                 // Close display driver

 return FCT_OK;



 }
 */
int HandleMenu(void) {

	return 1;
}

int ZippcardFunction(int argument) {
	return 1;
}

int InitMenu(_MENU_ITEM *MENULIST, char *szMainTitle) {
	int TotalItems = 0;
	int MenuSelected = 0;
	int MenuId[MAX_MENU_ITEMS] = { 0 };
	char *MenuItems[MAX_MENU_ITEMS] = { 0 };
	int LoopCounter = 0;
	int IsBranchItem = 0;
	int ItemSelected = 0;
	int MenuLevel = 0;
	int ParsingLevel = 0;
	int MenuLevelIds[MAX_MENU_ITEMS] = { 0 };
	int SelectedItems[MAX_MENU_ITEMS] = { 0 };
	int TitleId[MAX_MENU_ITEMS] = { 0 };
	bool (*PreSelectionFunction)(int);
	int (*Function)(int);
	bool (*PostSelectionFunction)(int);
	int TimeOut = 20;

	SelectedItems[0] = 0;
	for (; TRUE;) {
		for (TotalItems = 0, IsBranchItem = FALSE,ParsingLevel = MenuLevel,LoopCounter =
				MenuSelected; TRUE; LoopCounter++) {
			/*
			 if(!IsBranchItem && (MENULIST[LoopCounter].Type==MENU_TYPE_DELIMITER || !MENULIST[LoopCounter].Label) )
			 break;
			 else if (MENULIST[LoopCounter].Type==MENU_TYPE_DELIMITER || !MENULIST[LoopCounter].Label )
			 IsBranchItem=FALSE;

			 if( MENULIST[LoopCounter].Enabled && !IsBranchItem && (MENULIST[LoopCounter].Type==MENU_TYPE_LABEL || MENULIST[LoopCounter].Type==MENU_TYPE_BRANCH) )
			 {
			 MenuId[TotalItems]=LoopCounter;
			 MenuItems[TotalItems++]=MENULIST[LoopCounter].Label;
			 }

			 if(!IsBranchItem && (MENULIST[LoopCounter].Type==MENU_TYPE_BRANCH))
			 IsBranchItem=TRUE;
			 */

			if (TotalItems < MAX_MENU_ITEMS && MENULIST[LoopCounter].Enabled
					&& (ParsingLevel == MenuLevel)
					&& (MENULIST[LoopCounter].Type == MENU_TYPE_LABEL
							|| MENULIST[LoopCounter].Type == MENU_TYPE_BRANCH
							|| MENULIST[LoopCounter].Type == MENU_TYPE_EXIT)) {
				MenuId[TotalItems] = LoopCounter;
				MenuItems[TotalItems++] = MENULIST[LoopCounter].Label;
			}

			if (MENULIST[LoopCounter].Type == MENU_TYPE_BRANCH)
				ParsingLevel++;
			else if (ParsingLevel > MenuLevel
					&& MENULIST[LoopCounter].Type == MENU_TYPE_DELIMITER)
				ParsingLevel--;
			else if ((ParsingLevel == MenuLevel)
					&& (MENULIST[LoopCounter].Type == MENU_TYPE_DELIMITER))
				break;
		}

		ItemSelected = ManageMenu(
				MenuLevel ? MENULIST[TitleId[MenuLevel]].Label : szMainTitle,
				NULL, SelectedItems[MenuLevel], TotalItems,
				(const char **) MenuItems, MenuLevel ? TimeOut : 0);

		switch (ItemSelected) {
		case __EXIT_KEY:
			MenuSelected = 0;
			MenuLevel = 0;
			memset(&SelectedItems, 0, sizeof(SelectedItems));
			break;

		case __BACK_KEY:
			SelectedItems[MenuLevel] = 0; /* Clear Selection */
			if (MenuLevel > 0)
				MenuLevel--;
			MenuSelected = MenuLevelIds[MenuLevel];
			break;

		default:
			SelectedItems[MenuLevel] = ItemSelected;
			PreSelectionFunction =
					MENULIST[MenuId[ItemSelected]].PreSelectionFunction;
			PostSelectionFunction =
					MENULIST[MenuId[ItemSelected]].PostSelectionFunction;
			Function = MENULIST[MenuId[ItemSelected]].Function;

			/* Executes Functions if associated any */
			if (PreSelectionFunction) {
				if (!PreSelectionFunction(0))
					break;
			}
			if (Function) {
				if (!Function(0))
					break;
			}

			if (PostSelectionFunction) {
				if (!PostSelectionFunction(0))
					break;
			}

			if (MENULIST[MenuId[ItemSelected]].Type == MENU_TYPE_EXIT)
				return __EXIT_KEY;
			else if (MENULIST[MenuId[ItemSelected]].Type == MENU_TYPE_BRANCH) {
				//MenuLevelIds[MenuLevel++]=MenuId[ItemSelected];
				MenuLevelIds[MenuLevel++] = MenuId[0]; /* First Menu Item */
				TitleId[MenuLevel] = MenuId[ItemSelected];
				MenuSelected = MenuId[ItemSelected] + 1; /* Next Menu Items */
			} else if (MENULIST[MenuId[ItemSelected]].Type == MENU_TYPE_LABEL
					&& MENULIST[MenuId[ItemSelected]].ExitToMain) {
				MenuSelected = 0;
				MenuLevel = 0;
				memset(&SelectedItems, 0, sizeof(SelectedItems));
			}

			break;

		}
	}
}

int SelectFromList(const char *szTitle, int bRadioButtons, int nDefaultChoice,
		int nItems, const char* Items[], int WindowSize, int TimeOut) {
	FILE *hDisplay;
	int DisplayHeaderStatus;

	StructList Menu;
	int nY;
	int nMaxX = 0;
	int nMaxY = 0;

	ENTRY_BUFFER Entry;

	int i;
	int nInput;
	int nReturn;

	//hDisplay = fopen( "DISPLAY", "w" );

	// Get Screen size.
	GetScreenSize(&nMaxY, &nMaxX);

	// For the menu height of the menu,
	if (WindowSize == _WINDOW_SIZE_SMALL_ || WindowSize == _WINDOW_SIZE_MEDIUM_)
		nY = (nMaxY - 1) / _WINDOW_SIZE_SMALL_;
	else if (WindowSize == _WINDOW_SIZE_LARGE_)
		nY = (nMaxY - 1) / _WINDOW_SIZE_LARGE_ + 5;
	else
		nY = 0;
	DisplayHeaderStatus = StateHeader(0); // disable display header

	if ((nDefaultChoice < 0) || (nDefaultChoice >= nItems)) {
		nDefaultChoice = 0;
	}

	//CreateGraphics(_MEDIUM_);

	memset(&Menu, 0, sizeof(Menu));
	Menu.MyWindow.left = WindowSize ? ((nMaxX - 1) / WindowSize) : 0;
	Menu.MyWindow.top = nY;
	Menu.MyWindow.rigth = nMaxX - 1;
	Menu.MyWindow.bottom = nMaxY - 1;
	if (nMaxY == 128) {
		Menu.MyWindow.nblines = 10;
	} else {
		Menu.MyWindow.nblines = 5;
	}

	Menu.MyWindow.fontsize = _MEDIUM_;
	Menu.MyWindow.type = _PROPORTIONNEL_;
	Menu.MyWindow.font = 0;
	Menu.MyWindow.correct = _ON_;
	Menu.MyWindow.offset = 0;
	Menu.MyWindow.shortcommand = _OFF_;
	if (bRadioButtons) {
		Menu.MyWindow.selected = _ON_;
	} else {
		Menu.MyWindow.selected = _OFF_;
	}

	Menu.MyWindow.thickness = 1;
	Menu.MyWindow.border = _ON_;
	Menu.MyWindow.popup = _POPUP_;
	Menu.MyWindow.first = nDefaultChoice;
	Menu.MyWindow.current = nDefaultChoice;
	Menu.MyWindow.time_out = TimeOut; //60;
	Menu.MyWindow.title = (unsigned char*) szTitle;

	for (i = 0; i < nItems; i++) {
		Menu.tab[i] = (unsigned char*) Items[i];
	}

	G_List_Entry((void*)&Menu);
	ttestall(ENTRY, 0);
	nInput = Get_Entry((void*)&Entry);

	/*
	 clrscr();
	 printf("Entry=%d",nInput);
	 ttestall(0,3*100);
	 */
	switch (nInput) {
	case CR_ENTRY_OK:
		nReturn = Entry.d_entry[0];
		break;

	case CR_ENTRY_TIME_OUT:
	case CR_ENTRY_CANCEL:
		nReturn = __EXIT_KEY;
		break;

	default:
		nReturn = __BACK_KEY;
		break;
	}
	StateHeader(DisplayHeaderStatus); // move display header in previous state
	//fclose( hDisplay );

	return nReturn;
}

bool InitApplicationData(void) {
	NO_SEGMENT No;
	unsigned char FlagDownload;
	unsigned char Type[100] = { 0 };
	static int siFirstInit = FALSE;
	char temp[1] = { 0 };
	No = ApplicationGetCurrent(); // Return the application number
	first_init(No, &FlagDownload, Type);
	raz_init(No);

	fs_UIInit();
	fs_DisplayInit();

	memset(&glParameters, 0, sizeof(ParameterDataType));
	if (!fs_ParmInit(App_Name, glParameterTable, (char *) &glParameters,
			sizeof(ParameterDataType))) {
		fs_DisplayParmsError();
		return FALSE;
	}
	if (FlagDownload == 0xFF && siFirstInit == FALSE) /* First Execution */
	{

		fs_DisplayClearAll();
		//fs_DisplayAtRow(ROW2,"UPDATING PARAMETER",daAlignCenter);
		//fs_DisplayAtRow(ROW3,"FILES",daAlignCenter);
		//fs_DisplayAtRow(ROW5,"Please Wait",daAlignCenter);
		//	fs_UIGetCustomInput(App_Name,"Application Init.","Please Wait",daAlignCenter,"*",daAlignRight,eDRightToLeft,' ',0,temp,1);
		ttestall(0, 200);
		if (!fs_ParmSetup(TRUE) || !fs_ParmSaveToStore())
			fs_DisplayParmsError();

		fs_ExtractParametersFromFile("ON-TAB.PRM") && fs_ParmSaveToStore();
		siFirstInit = TRUE;
		/*
		 fs_DisplayClearAll();
		 fs_DisplayAtRow(ROW2,"RESTARTING",daAlignCenter);
		 fs_DisplayAtRow(ROW5,"Please Wait",daAlignCenter);
		 ttestall(0,500);
		 */
		//OEM_exit(EXIT_TERM_INIT);
	}
#if 0
	fs_ParmLoadFromStore() &&
	fs_ParmSetup(FALSE) &&
	fs_ExtractParametersFromFile("ON-TAB.PRM") &&
	fs_ParmSaveToStore(); /* Required to save if & only if parameters are extracted from ZIPP.PRM */
#endif
	else
	{
		if (!fs_ParmLoadFromStore()) {
			fs_ParmSetup(TRUE);
			if (!fs_ParmSaveToStore() || !fs_ParmLoadFromStore()) {
				fs_DisplayParmsError();
				return FALSE;
			}
		} else {
			fs_ParmSetup(FALSE);
		}
		fs_ExtractParametersFromFile("ON-TAB.PRM") && fs_ParmSaveToStore(); /* Required to save if & only if parameters are extracted from ZIPP.PRM */
	}

	return TRUE;
}

/** bool checkDefaultPassword(int var)
 *  This function verifies the default password after fresh download
 * @param var: Not being used
 * @return : Returns TRUE if verified else FALSE
 */

bool checkMerchantPassword(int var) {
	char strBuffer[100] = { 0 };
	int iTries = 3;

	/* Password has been disabled */
	if (!strlen(glParameters.AdminPassword))
		return TRUE;

	fs_UIBuzzerPrompt(); /* Buzzer Beep */
	while (iTries--) /* 3 Tries */
	{
		/* Get input from user */
		if (fs_UIGetInput("ZIPPCARD", "ENTER MERCHANT", "PASSWORD",
				daAlignCenter, entryTypePassword, daAlignCenter, strBuffer,
				APASS_LEN_MIN, APASS_LEN_MAX, glParameters.PromptTimeout)) {
			if (strcmp(strBuffer, glParameters.AdminPassword)) {
				/* Display Error if password does not match */
				fs_UIDisplayErrorScreen("ERROR!", "Invalid Merchant",
						"Password", "");
				fs_DisplayClearAll();
			} else
				return TRUE;
		} else
			return FALSE;
	}
	return FALSE;
}

/** bool checkDefaultPassword(int var)
 *  This function verifies the default password after fresh download
 * @param var: Not being used
 * @return : Returns TRUE if verified else FALSE
 */

bool checkDefaultPassword(int var) {
	char strBuffer[50] = { 0 };
	char strHourMonth[50] = { 0 };
	int iTries = 3;
	char strLockDateTime[FS_DATE_TIME_LEN_MAX + 1] = { 0 };
	long lHours, lMinutes;

	/* Check if merchant password has been set earlier */
	if (isMerchantPassWordSet())
		return TRUE;

	/* Check if posted unlock date time has been elapsed */
	if (glParameters.iLockLevel >= LOCK_LEVEL_1_HOUR
			&& !fs_CheckTimeElapsed(glParameters.strUnlockDateTime)) {
		/* If not unlocked then display time to unlock */
		if (fs_GetTimeDifference(glParameters.strUnlockDateTime, NULL, NULL,
				&lHours, &lMinutes)) {
			if (lHours == 1)
				sprintf(strBuffer, "%ld Hour", lHours);
			else if (lHours != 0)
				sprintf(strBuffer, "%ld Hours", lHours);

			if (lHours && lMinutes)
				strcat(strBuffer, " & ");

			if (lMinutes == 1)
				sprintf(strHourMonth, "%ld Minute", lMinutes);
			else if (lMinutes != 0)
				sprintf(strHourMonth, "%ld Minutes", lMinutes);
			strcat(strBuffer, strHourMonth);

			fs_UIDisplayErrorPrompts("APPLICATION LOCKED !", " ",
					"PLEASE TRY AFTER", strBuffer, 3);
		} else
			(glParameters.iLockLevel == LOCK_LEVEL_1_HOUR) ?
					fs_UIDisplayErrorScreen("ZIPPCARD",
							"APPLICATION LOCKED FOR", "1 HOUR", "") :
					fs_UIDisplayErrorScreen("ZIPPCARD",
							"APPLICATION LOCKED FOR", "24 HOURS", "");

		fs_DisplayClearAll();
		return FALSE;
	} else if (glParameters.iLockLevel == LOCK_LEVEL_1_HOUR
			&& fs_CheckTimeElapsed(glParameters.strUnlockDateTime)) {
		fs_UIDisplayErrorScreen("WARNING !", "WRONG PASSWORD",
				"Will Lock Application", "FOR 24 HOURS");
	} else if (glParameters.iLockLevel == LOCK_LEVEL_24_HOUR
			&& fs_CheckTimeElapsed(glParameters.strUnlockDateTime)) {
		glParameters.iLockLevel = 0;
		glParameters.iDefaultPasswordTries = 0;
		fs_ParmSaveToStore();
	}

	iTries =
			(glParameters.iLockLevel == LOCK_LEVEL_1_HOUR) ?
					1 : (3 - glParameters.iDefaultPasswordTries);
	fs_UIBuzzerPrompt(); /* Buzzer Beep */
	while (iTries--) /* 3 Tries */
	{
		/* Get input from user */
		if (fs_UIGetInput("ZIPPCARD", "ENTER DEFAULT", "PASSWORD",
				daAlignCenter, entryTypePassword, daAlignCenter, strBuffer,
				DEFAULT_PASS_LEN_MIN, DEFAULT_PASS_LEN_MAX,
				glParameters.PromptTimeout)) {
			if (strcmp(strBuffer, glParameters.DefaultPassword)) {
				/* Display Error if password does not match */
				fs_UIDisplayErrorScreen("ERROR!", "Invalid Default", "Password",
						"");
				glParameters.iDefaultPasswordTries++;
				fs_ParmSaveToStore();
				fs_DisplayClearAll();
			} else {
				glParameters.iLockLevel = 0;
				glParameters.iDefaultPasswordTries = 0;
				fs_ParmSaveToStore();
				return TRUE;
			}
		} else
			return FALSE;
	}
	glParameters.iLockLevel++;
	LOG_PRINTF("glParameters.strUnlockDateTime=%s",
			glParameters.strUnlockDateTime);
	LOG_PRINTF("glParameters.iLockLevel=%d", glParameters.iLockLevel);
	fs_ClockGetString(strLockDateTime)
			&& fs_GetNextDateTime(
					(glParameters.iLockLevel == LOCK_LEVEL_1_HOUR) ?
							FS_OFFSET_1_HOUR : FS_OFFSET_24_HOURS
							, strLockDateTime, glParameters.strUnlockDateTime)
			&& fs_ParmSaveToStore()
			&& (glParameters.iLockLevel == LOCK_LEVEL_1_HOUR) ?
			fs_UIDisplayErrorScreen("ZIPPCARD", "APPLICATION LOCKED",
					"FOR 1 HOUR", "") :
			fs_UIDisplayErrorScreen("ZIPPCARD", "APPLICATION LOCKED",
					"FOR 24 HOURS", "");

	LOG_PRINTF("strLockDateTime=%s", strLockDateTime);
	LOG_PRINTF("glParameters.strUnlockDateTime=%s",
			glParameters.strUnlockDateTime);

	return FALSE;
}

/**
 *  bool checkMaintenancePassword(int var)
 *  This function verifies the maintenance password of the system
 * @param var: Not being used
 * @return : Returns TRUE if verified else FALSE
 */

bool checkMaintenancePassword(int var) {
	char strBuffer[100] = { 0 };
	int iTries = 3;

	fs_UIBuzzerPrompt(); /* Buzzer Beep */
	while (iTries--) /* 3 Tries */
	{
		/* Get input from user */
		if (fs_UIGetInput("ZIPPCARD", "ENTER MAINTENANCE", "PASSWORD",
				daAlignCenter, entryTypePassword, daAlignCenter, strBuffer,
				MAINTAIN_PASS_LEN_MIN, MAINTAIN_PASS_LEN_MAX,
				glParameters.PromptTimeout)) {
			if (strcmp(strBuffer, glParameters.MaintainPassword)) {
				/* Display Error if password does not match */
				fs_UIDisplayErrorScreen("ERROR!", "Invalid Maintenance",
						"Password", "");
				fs_DisplayClearAll();
			} else
				return TRUE;
		} else
			return FALSE;
	}
	return FALSE;
}

/**
 * bool setMerchantPassword(int var)
 * This function sets the merchant password. Used to set merchant password after fresh download.
 * @param var: Not used
 * @return :  Returns TRUE if successfully set else FALSE
 */

bool setMerchantPassword(int var) {
	char strBuffer[100] = { 0 };
	char strPasswordBuffer[100] = { 0 };
	bool bSaveChanges = FALSE;

	/* Check if merchant password has been set earlier */
	if (isMerchantPassWordSet())
		return TRUE;

	/* Get input from user */
	while (fs_UIGetInput("SET PASSWORD", "ENTER NEW", "MERCHANT PASSWORD",
			daAlignCenter, entryTypePassword, daAlignCenter, strBuffer,
			APASS_LEN_MIN, APASS_LEN_MAX, 0)) {
		if (fs_UIGetInput("SET PASSWORD", "RE-ENTER NEW", "MERCHANT PASSWORD",
				daAlignCenter, entryTypePassword, daAlignCenter,
				strPasswordBuffer, APASS_LEN_MIN, APASS_LEN_MAX, 0)) {
			if (!strcmp(strBuffer, strPasswordBuffer)) {
				/* If both entries match then make changes in parameter table */
				strcpy(glParameters.AdminPassword, strPasswordBuffer);
				bSaveChanges = TRUE;
				break;
			} else {
				/* Display Error if password does not match */
				fs_DisplayClearWindowContents();
				fs_DisplayAtRowOverWrite(ROW3, "PASSWORD", daAlignCenter);
				fs_DisplayAtRowOverWrite(ROW4, "DOES NOT MATCH!",
						daAlignCenter);
				fs_DisplayAtRowOverWrite(ROW6, " ", daAlignCenter);
				ttestall(0, 300);
			}

		}
	}
	/* If password has been changed successfully, then update the parameter table */
	if (bSaveChanges == TRUE) {
		glParameters.bMerchantPassWordSet = TRUE; /* Update the password flag in parameter table */
		fs_ParmSaveToStore();
		return TRUE;
	}

	return FALSE;
}

/**
 * bool isMerchantPassWordSet(void)
 * Used to get the status if merchant password has been set for the first time
 * @return : Returns TRUE if merchant password has been set else FALSE
 */

bool isMerchantPassWordSet(void) {
	return (glParameters.bMerchantPassWordSet == TRUE) ? TRUE : FALSE;
}

/**
 * bool isApplicationLocked(void)
 * Used to know whether the application has been locked due to invalid password entry
 * @return: Returns TRUE if application has been locked else FALSE
 */

bool isApplicationLocked(void) {
	return (glParameters.iLockLevel > 0) ? TRUE : FALSE;
}

void InitRecieptVar() {
	/*
	sRecieptVar.fRecieptAmount = 0.00;
	sRecieptVar.fRecieptTips = 0.00;
	sRecieptVar.fRecieptTotal = 0.00;
	strcpy(sRecieptVar.sRecieptAddress, "");
	strcpy(sRecieptVar.sRecieptCreatedat, "");
	strcpy(sRecieptVar.sRecieptName, "");
	strcpy(sRecieptVar.sRecieptStatus, "");
	*/
	memset(&sRecieptVar,0,sizeof(stRecieptVar));

}

bool OnTab(void) {
	int iRet;
	int prec;
	static int iLogin=FALSE;
	prec = StateHeader(0);
	DrawPrompt(App_Name, "Application Init..", "", 1);
	ttestall(0, 50);
	/*
	if(iLogin==FALSE)
	{
		if(!UserLogin())
			return FALSE;
		iLogin=TRUE;
	}
	*/
	if((LoadGlobalData()==FALSE) || globalData.bAppInit==FALSE)
	{
		if(!UserLogin())
			return FALSE;
		globalData.bAppInit=TRUE;
		SaveGlobalData();
	}

	while (1) {
		iRet = 0;
		InitRecieptVar();
		fAmount = 0;
		iRet = OnTabMenu();
		if (!iRet)
			break;
		iRet = GetPurchaseAmount();
		if (!iRet)
			continue;
		iRet = SelectOption();
		if (!iRet)
			continue;
		if (iRet == 1)
		{
			iRet =	GetCustomerList() &&
					SelectCustomer() ;
			IDTypeSelection=ByCustomerName;
		}
		else
		{
			iRet = GetTabNumber();
			IDTypeSelection=ByMobileId;
		}
		if (!iRet)
			continue;
		SendRequest();
		continue;
	}
	StateHeader(prec);
	return FALSE;
}

int OnTabMenu() {
	FILE *hKeyboard;
	int ch, nEvent;
	int iBottom, iRight, FontHeight;
	SetType(_NORMALE_);
	GetScreenSize(&iBottom, &iRight);
	SetArea(0, 0, iRight - 3, iBottom - 3, _OFF_);
	DrawWindow(0, 0, iRight - 1, iBottom - 1, 1, App_Name);
	FontHeight = GetPoliceSize();
	SetArea(10, 10, iRight - 3, iBottom - 3, _OFF_);
	//DrawString("1. READ FILE",3,FontHeight,_OFF_);
	DrawString("F1. SALE", 10, 2 * FontHeight, _OFF_);
	DrawString("F2. REFUND", 10, 3 * FontHeight, _OFF_);
	while (1) {
		LOG_PRINTF("INSIDE WHILE LOOP");
		hKeyboard = fopen("KEYBOARD", "r");
		nEvent = ttestall(KEYBOARD, 2000);
		if (nEvent == KEYBOARD) {
			LOG_PRINTF("INSIDE IF CONDITION");
			ch = getchar();
			fclose(hKeyboard);
			switch (ch) {
			case T_ANN:
				return FALSE;
			case 25:
				LOG_PRINTF("SALE SELECTED");
				strcpy(strTransactionType, "SALE");
				break;
			case 36:
				LOG_PRINTF("REFUND SELECTED");
				strcpy(strTransactionType, "REFUND");
				break;
			default:
				LOG_PRINTF("ENTERED KEY IS : %d", ch);
				continue;
			}
		} else {
			fclose(hKeyboard);
			return FALSE;
		}
		break;
	}
	return TRUE;
}

int SelectOption() {
	FILE *hKeyboard;
	int ch, nEvent;
	int iBottom, iRight, FontHeight;
	SetType(_PROPORTIONNEL_);
	GetScreenSize(&iBottom, &iRight);
	SetArea(0, 0, iRight - 3, iBottom - 3, _OFF_);
	DrawWindow(0, 0, iRight - 1, iBottom - 1, 1, App_Name);
	FontHeight = GetPoliceSize();
	SetArea(10, 10, iRight - 3, iBottom - 3, _OFF_);
	//DrawString("1. READ FILE",3,FontHeight,_OFF_);
	DrawString("F1.     SELECT  CUSTOMER.", 3, 2 * FontHeight, _OFF_);
	DrawString("F2.     ENTER  MOBILE  ID.", 3, 3 * FontHeight, _OFF_);
	while (1) {
		LOG_PRINTF("INSIDE WHILE LOOP");
		hKeyboard = fopen("KEYBOARD", "r");
		nEvent = ttestall(KEYBOARD, 2000);
		if (nEvent == KEYBOARD) {
			LOG_PRINTF("INSIDE IF CONDITION");
			ch = getchar();
			fclose(hKeyboard);
			switch (ch) {
			case T_ANN:
				return FALSE;
			case 25:
				LOG_PRINTF("CUSTOMER SELECTED");
				return 1;
			case 36:
				LOG_PRINTF("MOBILE ID SELECTED");
				return 2;
			default:
				LOG_PRINTF("ENTERED KEY IS : %d", ch);
				continue;
			}
		} else {
			fclose(hKeyboard);
			return FALSE;
		}
		break;
	}
	return TRUE;
}

int SelectCustomer() {
	FILE *hKeyboard;
	int ch, nEvent;
	int iBottom, iRight, FontHeight;
	int flag = 0;
	int iLoop,iTotalCustomers;
	char strCustomerList[9][50] = { 0 };

	iTotalCustomers=customerList.iNoOfCustomers;
	for(iLoop=0;iLoop<iTotalCustomers;iLoop++)
		sprintf(strCustomerList[iLoop],"%2d) %s",iLoop+1,customerList.szName[iLoop]);

	SetType(_PROPORTIONNEL_);
	GetScreenSize(&iBottom, &iRight);
	SetArea(0, 0, iRight - 3, iBottom - 3, _OFF_);
	DrawWindow(0, 0, iRight - 1, iBottom - 1, 1, App_Name);
	FontHeight = GetPoliceSize();
	SetArea(10, 10, iRight - 3, iBottom - 3, _OFF_);
	//DrawString("1. READ FILE",3,FontHeight,_OFF_);
	DrawString("SELECT CUSTOMER:", 3, FontHeight, _OFF_);
	while (1) {
		SetArea(2, FontHeight + 10, iRight - 3, iBottom - 3, _OFF_);
		clrscr();

		for(iLoop=0;iLoop<3;iLoop++)
		{
			if((iLoop+flag*3)<iTotalCustomers)
				DrawString(strCustomerList[iLoop+flag*3], 3, (3+iLoop) * FontHeight, _OFF_);
		}

		hKeyboard = fopen("KEYBOARD", "r");
		nEvent = ttestall(KEYBOARD, 2000);
		if (nEvent == KEYBOARD) {
			ch = getchar();
			LOG_PRINTF("SelectCustomer() |ch=%d ch=%c",ch,ch);
			fclose(hKeyboard);
			switch (ch) {
			case 36:
				if ((flag != 2) && (iTotalCustomers>((flag+1)*3)))
					flag++;
				continue;
			case 35:
				if (flag != 0)
					flag--;
				continue;
			case T_ANN:
				return FALSE;
			default:
				if((ch>'0') && (ch<=(iTotalCustomers+48)))
				{
					customerList.iSelected=ch-'1';
					return TRUE;
				}
				continue;
			}
		} else {
			fclose(hKeyboard);
			continue;
		}
	}

	return FALSE;
}

void DisplayAmount(char *strHeading, char *strDisplay) {
	int iBottom, iRight, len;
	//int prec;
	SetType(_NORMALE_);
	len = strlen(strDisplay) * 8;
	LOG_PRINTF("IN Display Amount");
	GetScreenSize(&iBottom, &iRight);
	SetArea(0, 0, iRight - 1, iBottom - 1, _OFF_);
	DrawWindow(0, 0, iRight - 1, iBottom - 1, 1, App_Name);
	SetArea(10, 10, iRight - 3, 40, _OFF_);
	DrawString(strHeading, 10, 20, _OFF_);
	SetArea(10, 60, iRight - 3, iBottom - 3, _OFF_);
	DrawString(strDisplay, iRight - len - 2, 50, _OFF_);
}

void DisplayPassword(char *strHeading, char *strDisplay) {
	int iBottom, iRight, len;
	char strPassword[9]={0};
	int iLoop;
	//int prec;
	SetType(_NORMALE_);
	len = strlen(strDisplay) * 8;
	LOG_PRINTF("IN Display Amount");
	for(iLoop=strlen(strDisplay);iLoop;iLoop--)
		strPassword[iLoop-1]='*';	/* Show Asterisk */
	GetScreenSize(&iBottom, &iRight);
	SetArea(0, 0, iRight - 1, iBottom - 1, _OFF_);
	DrawWindow(0, 0, iRight - 1, iBottom - 1, 1, App_Name);
	SetArea(10, 10, iRight - 3, 40, _OFF_);
	DrawString(strHeading, 10, 20, _OFF_);
	SetArea(10, 60, iRight - 3, iBottom - 3, _OFF_);
	DrawString(strPassword, iRight - len - 2, 50, _OFF_);
}

int GetPurchaseAmount() {
	int cVal = 0, count = 0, len = 0;
	int i = 0, j = 0;
	char strTempAmount[20] = { 0 };
	unsigned int nEvent;
	LOG_PRINTF("IN PURCHASE");
	FILE *hKeyboard;

	strcpy(strTempAmount, "0.00");
	DisplayAmount("ENTER AMOUNT", "0.00");
	hKeyboard = fopen("KEYBOARD", "r");
	nEvent = ttestall(KEYBOARD, 2000);
	if (nEvent == KEYBOARD) {
		while (1) {
			count = 0;
			cVal = getchar();
			LOG_PRINTF("KEY : %d", cVal);
			if (cVal == 48 && strlen(strTempAmount) == 0) {
				DisplayAmount("ENTER AMOUNT", "0.00");
				continue;
			}
			if ((cVal >= 48 && cVal <= 57)) {
				len = strlen(strTempAmount);
				if (len >= 11)
					continue;
				if (strlen(strTempAmount) == 4 && strTempAmount[0] == '0') {
					strTempAmount[0] = strTempAmount[2];
					strTempAmount[2] = strTempAmount[3];
					strTempAmount[3] = cVal;
				} else {
					i = 0;
					while (strTempAmount[i] != '.') {
						i++;
					}
					strTempAmount[i] = strTempAmount[i + 1];
					strTempAmount[i + 1] = '.';
					strTempAmount[len] = cVal;
					strTempAmount[len + 1] = '\0';
				}

				LOG_PRINTF(strTempAmount);
				DisplayAmount("ENTER AMOUNT", strTempAmount);

			}
			if (cVal == T_CORR) {
				LOG_PRINTF("clear Key");
				len = strlen(strTempAmount);
				if (len > 4) {
					i = 0;
					while (strTempAmount[i] != '.')
						i++;
					strTempAmount[i] = strTempAmount[i - 1];
					strTempAmount[i - 1] = '.';
					strTempAmount[len - 1] = '\0';
				} else {
					LOG_PRINTF(strTempAmount);
					strTempAmount[3] = strTempAmount[2];
					strTempAmount[2] = strTempAmount[0];
					strTempAmount[0] = '0';
				}
				LOG_PRINTF(strTempAmount);
				DisplayAmount("ENTER AMOUNT", strTempAmount);
			}
			if (cVal == T_ANN) {
				fclose(hKeyboard);
				return FALSE;
			}
			if (cVal == T_VAL) {
				if (strcmp(strTempAmount, "0.00") == 0)
					continue;
				fclose(hKeyboard);
				j = 0;
				for (i = 0; i < strlen(strTempAmount); i++) {
					if (strTempAmount[i] == '.') {
						continue;
					}
					strAmount[j] = strTempAmount[i];
					j++;
				}
				strAmount[j] = '\0';
				LOG_PRINTF(strAmount);
				return TRUE;
			} else {
				continue;
			}
		}
	}
	fclose(hKeyboard);
	return FALSE;

}

int GetPurchaseAmount1() {
	int ipSeparatorList[4] = { 11, 8, 5, 0 }; /* List must be NULL terminated */
	char buf_entry[22];

	/*long lAmount;
	 TAB_ENTRY_FCT   table;
	 table.mask=10;
	 table.time_out=10;*/
	unsigned char cr;
	StructList Menu;
	int MAXX;
	int MAXY;
	int prec;
	TAB_ENTRY_STRING NumEntry;

	while (1) {
		//GetAmount(1,"USD",1,&lAmount,"ENTER AMOUNT",&table);
		//prec=StateHeader(0);
		memset(strAmount, 0, sizeof(strAmount));
		GetScreenSize(&MAXY, &MAXX);
		SetArea(0, 0, MAXX - 1, MAXY - 1, _OFF_);
		//GetScreenSize(&MAXY,&MAXX);
		SetArea(0, 0, MAXX, MAXY, _OFF_);
		Menu.MyWindow.left = 0;
		Menu.MyWindow.top = 0;
		Menu.MyWindow.rigth = (MAXX - 1);
		Menu.MyWindow.bottom = (MAXY - 1);
		Menu.MyWindow.nblines = 64;
		Menu.MyWindow.fontsize = _MEDIUM_;
		Menu.MyWindow.type = _PROPORTIONNEL_;
		Menu.MyWindow.font = 0;
		Menu.MyWindow.shortcommand = _OFF_;
		Menu.MyWindow.selected = _OFF_;
		Menu.MyWindow.offset = 0;
		Menu.MyWindow.thickness = 1;
		Menu.MyWindow.border = _ON_;
		Menu.MyWindow.popup = _NOPOPUP_;
		Menu.MyWindow.first = 0;
		Menu.MyWindow.time_out = 20;
		Menu.MyWindow.current = 0;
		Menu.MyWindow.title = "ON_TAB";
		Menu.tab[0] = "ENTER AMOUNT ";
		Menu.tab[1] = "0.00";
		Menu.tab[2] = "FOOTER";
		NumEntry.mask = 0; // F2 + F3
		NumEntry.time_out = 30;
		NumEntry.line = 32;
		NumEntry.column = MAXY - 1;
		NumEntry.echo = ECHO_NORMAL;
		NumEntry.nb_max = 10;
		NumEntry.nb_min = 10;
		G_Numerical_Entry(&Menu, &NumEntry);
		ttestall(ENTRY, 0);
		cr = Get_Entry(buf_entry);
		if (cr == CR_ENTRY_CANCEL || cr == CR_ENTRY_TIME_OUT) {
			StateHeader(prec);
			return FALSE;
		}
		LOG_PRINTF("Key Pressed : %d", cr);
		if (cr == 1 || cr == 0) {
			PrintLog("OK..");
			if (strlen(&buf_entry[1]) < 3 || strlen(&buf_entry[1]) >= 10) {
				DrawPrompt(App_Name, "INVALID AMOUNT", "", 3);
				ttestall(0, 100);
				//		StateHeader(prec);
				continue;

			}
		}
		//	StateHeader(prec);
		strcpy(strAmount, &buf_entry[1]);
		PrintLog(strAmount);
		return (cr);
	}
	/*
	 fs_UIInit();
	 fs_DisplayInit();
	 clearCommonDataAll();
	 if(!fs_UIGetCustomAmountEntry(App_Name,"Please Enter","Purchase Amount",daAlignCenter,"USD",2,daAlignRight,3,MAX_AMOUNT_LEN,&fAmount,ipSeparatorList,60,strAmount))
	 */
	/*return FALSE;
	 PrintLog(strAmount);
	 return TRUE;*/
}

int GetTabNumber() {
	int cVal = 0, count = 0, len = 0;
	int i = 0, j = 0;
	char strTempTAB[5] = { 0 };
	unsigned int nEvent;
	LOG_PRINTF("IN TAB NO");
	FILE *hKeyboard;
	memset(strTabNum, 0, sizeof(strTabNum));
	strcpy(strTempTAB, "____");
	DisplayAmount("ENTER MOBILE ID", strTempTAB);
	hKeyboard = fopen("KEYBOARD", "r");
	nEvent = ttestall(KEYBOARD, 2000);
	count = 0;
	if (nEvent == KEYBOARD) {
		while (1) {
			cVal = getchar();
			LOG_PRINTF("KEY : %d", cVal);
			if ((cVal >= 48 && cVal <= 57)) {
				len = strlen(strTempTAB);
				if (count >= 4)
					continue;
				strTempTAB[0] = strTempTAB[1];
				strTempTAB[1] = strTempTAB[2];
				strTempTAB[2] = strTempTAB[3];
				strTempTAB[3] = cVal;
				LOG_PRINTF(strTempTAB);
				DisplayAmount("ENTER MOBILE ID.", strTempTAB);
				count++;
			}
			if (cVal == T_CORR) {
				if (strcmp(strTempTAB, "____") == 0)
					continue;
				LOG_PRINTF("clear Key");
				len = strlen(strTempTAB);
				LOG_PRINTF(strTempTAB);
				strTempTAB[3] = strTempTAB[2];
				strTempTAB[2] = strTempTAB[1];
				strTempTAB[1] = strTempTAB[0];
				strTempTAB[0] = '_';
				if (count > 0)
					count--;
				LOG_PRINTF(strTempTAB);
				DisplayAmount("ENTER MOBILE ID.", strTempTAB);

			}
			if (cVal == T_ANN) {
				fclose(hKeyboard);
				return FALSE;
			}
			if (cVal == T_VAL) {
				if (count < 4)
					continue;
				fclose(hKeyboard);
				strcpy(strTabNum, strTempTAB);
				LOG_PRINTF(strTabNum);
				return TRUE;
			} else {
				continue;
			}
		}
	}
	fclose(hKeyboard);
	return FALSE;
}

int GetUserID() {
	int cVal = 0, count = 0, len = 0;
	int i = 0, j = 0;
	char strTempTAB[8] = { 0 };
	unsigned int nEvent;
	LOG_PRINTF("IN USER ID");
	FILE *hKeyboard;

	memset(gUserID, 0, sizeof(gUserID));
	memset(strTempTAB, '_', sizeof(strTempTAB)-1);
	DisplayAmount("ENTER POS KEY", strTempTAB);
	hKeyboard = fopen("KEYBOARD", "r");
	nEvent = ttestall(KEYBOARD, 2000);
	count = 0;
	if (nEvent == KEYBOARD) {
		while (1) {
			cVal = getchar();
			LOG_PRINTF("KEY : %d", cVal);
			if ((cVal >= 48 && cVal <= 57)) {
				len = strlen(strTempTAB);
				if (count >= 7)
					continue;
				strTempTAB[count] = cVal;
				strTempTAB[count+1] = '\0';
				LOG_PRINTF(strTempTAB);
				DisplayAmount("ENTER POS KEY", strTempTAB);
				count++;
			}
			if (cVal == T_CORR) {
				if (count == 0)
					continue;
				LOG_PRINTF("clear Key");
				len = strlen(strTempTAB);
				LOG_PRINTF(strTempTAB);
				strTempTAB[count-1] = '\0';
				if (count > 0)
					count--;
				LOG_PRINTF(strTempTAB);
				DisplayAmount("ENTER POS KEY", strTempTAB);
			}
			if (cVal == T_ANN) {
				fclose(hKeyboard);
				return FALSE;
			}
			if (cVal == T_VAL) {
				if (count < 1)
					continue;
				fclose(hKeyboard);
				strcpy(gUserID, strTempTAB);
				LOG_PRINTF(strTabNum);
				return TRUE;
			} else {
				continue;
			}
		}
	}
	fclose(hKeyboard);
	return FALSE;
}


int GetPassword() {
	int cVal = 0, count = 0, len = 0;
	int i = 0, j = 0;
	char strTempTAB[9] = { 0 };
	unsigned int nEvent;
	LOG_PRINTF("IN GET PASSWORD");
	FILE *hKeyboard;

	memset(gPassword, 0, sizeof(gPassword));
	memset(strTempTAB, '_', sizeof(strTempTAB)-1);

	DisplayAmount("ENTER PASSWORD", strTempTAB);
	hKeyboard = fopen("KEYBOARD", "r");
	nEvent = ttestall(KEYBOARD, 2000);
	count = 0;
	if (nEvent == KEYBOARD) {
		while (1) {
			cVal = getchar();
			LOG_PRINTF("KEY : %d", cVal);
			if ((cVal >= 48 && cVal <= 57)) {
				len = strlen(strTempTAB);
				if (count >= 8)
					continue;
				strTempTAB[count] = cVal;
				strTempTAB[count+1] = '\0';
				LOG_PRINTF(strTempTAB);
				DisplayPassword("ENTER PASSWORD", strTempTAB);
				count++;
			}
			if (cVal == T_CORR) {
				if (count == 0)
						continue;
				LOG_PRINTF("clear Key");
				len = strlen(strTempTAB);
				LOG_PRINTF(strTempTAB);
				strTempTAB[count-1] = '\0';
				if (count > 0)
					count--;
				LOG_PRINTF(strTempTAB);
				DisplayPassword("ENTER PASSWORD", strTempTAB);
			}
			if (cVal == T_ANN) {
				fclose(hKeyboard);
				return FALSE;
			}
			if (cVal == T_VAL) {
				if (count < 1)
					continue;
				fclose(hKeyboard);
				strcpy(gPassword, strTempTAB);
				LOG_PRINTF(gPassword);
				return TRUE;
			} else {
				continue;
			}
		}
	}
	fclose(hKeyboard);
	return FALSE;
}

int UserLogin() {

	if(	!GetUserID()  	||
		!GetPassword() 	||
		!GetAuthToken()
		)
	return FALSE;

	return TRUE;
}

int SendRequest() {
	int iRet;
	char strPrint[20] = { 0 };
	DrawPrompt(App_Name, "Sending / Receiving", "", 0);
	iRet = ConnectEthernet();
	sprintf(strPrint, "Return : %d", iRet);
	PrintLog(strPrint);
	return iRet;
}

int GetAuthToken() {
	int iRet;
	char strPrint[20] = { 0 };
	DrawPrompt(App_Name, "Logging In", "Please Wait", 0);
	iRet = TCPGetAuthToken();
	sprintf(strPrint, "Return : %d", iRet);
	PrintLog(strPrint);
	return iRet;
}

int GetCustomerList() {
	int iRet;
	char strPrint[20] = { 0 };
	DrawPrompt(App_Name, "Getting List", "Please Wait", 0);
	iRet = TCPGetCustomerList();
	sprintf(strPrint, "Return : %d", iRet);
	PrintLog(strPrint);
	return iRet;
}

void PrintLog(char strPrint[]) {
	/*int i=0;
	 fs_PrinterInit();
	 while(strlen(strPrint)>i)
	 {
	 fs_PrinterAppendLine(&strPrint[i]);
	 i=i+24;
	 }
	 fs_PrinterClose();*/
	LOG_PRINTF(strPrint);
}

static const char *parseStr(char *pcDst, const char *pcSrc, int iDim) {
	while (*pcSrc) {
		if (iDim == 0) // Buffer pcDst overflow
			return 0; // Parsing failed

		if (*pcSrc == __CHN_SEPARATOR__) { // Separator found
			pcSrc++; // Skip separator
			break;
		}

		*pcDst++ = *pcSrc++;
		iDim--;
	}
	*pcDst = 0;
	return pcSrc;
}



int TCPGetAuthToken() {
	// Local variables
	// ***************
	char strPrint[2000] = { 0 };
	int iRet = 0, ret = 0, retVal=FALSE;

	PrintLog("Inside TCPGetAuthToken()");

	piConfig = TlvTree_New(LL_TAG_LINK_LAYER_CONFIG);
	piPhysicalConfig = TlvTree_AddChild(piConfig, LL_TAG_PHYSICAL_LAYER_CONFIG,
			NULL, 0);
	TlvTree_AddChildInteger(piPhysicalConfig, LL_PHYSICAL_T_LINK,
			LL_PHYSICAL_V_ETHERNET, LL_PHYSICAL_L_LINK);
	piTransportConfig = TlvTree_AddChild(piConfig,
			LL_TAG_TRANSPORT_LAYER_CONFIG, NULL, 0);
	TlvTree_AddChildInteger(piTransportConfig, LL_TRANSPORT_T_PROTOCOL,
			LL_TRANSPORT_V_TCPIP, LL_TRANSPORT_L_PROTOCOL);
//	TlvTree_AddChildString( piTransportConfig, LL_TCPIP_T_SSL_PROFILE, szProfile);
	TlvTree_AddChildString(piTransportConfig, LL_TCPIP_T_HOST_NAME,
			"http://ontab-demo.herokuapp.com");
	TlvTree_AddChildInteger(piTransportConfig, LL_TCPIP_T_PORT, 80,
			LL_TCPIP_L_PORT);

	iRet = LL_Configure(tvSession, piConfig);
	sprintf(strPrint, "CONFIG RET : %d", iRet);
	PrintLog(strPrint);
	if (iRet == LL_ERROR_OK) {
		int iLength = 0;
		int eventret = 1;
		int icheck = 1;
		int iNbrBytes = 0;
		char pucBuffer[4096] = { 0 };
		char strAuthToken[100] = { 0 };
		char GET_REQUEST[1000] = { 0 };
		int InvoiceNo = 1;
		char tempRequest[200] = { 0 };
		sprintf(tempRequest,
				"login=%s&password=%s",
				gUserID, gPassword);
		sprintf(GET_REQUEST, "%s\r\n%s\r\n%s\r\n%s %d\r\n%s\r\n\r\n%s",
				"POST /api/v1/login HTTP/1.1",
				"Host: ontab-demo.herokuapp.com", "Accept: */*",
				"Content-Length:", (int)strlen(tempRequest),
				"Content-Type: application/x-www-form-urlencoded",tempRequest);

		iRet = LL_Connect(tvSession[0]);
		if (iRet == LL_ERROR_OK) {

			sprintf(strPrint, "Connect RET : %d", iRet);
			PrintLog(strPrint);
			Sending: iLength = 0;
			iNbrBytes = 0;
			memset(pucBuffer, 0, sizeof(pucBuffer));
			PrintLog(GET_REQUEST);
			iRet = LL_Send(tvSession[0], strlen(GET_REQUEST), GET_REQUEST,
					1000);
			sprintf(strPrint, "Sending RET : %d", iRet);
			PrintLog(strPrint);
			iRet = 0;
			LL_ClearReceiveBuffer(tvSession[0]);
			while (1) {
				LL_ClearReceiveBuffer(tvSession[0]);
				eventret = LL_WaitEvent(tvSession[0], LL_WAIT_RECEIVED_DATA,
						1500);
				LOG_PRINTF("Wait Event Ret : %d", eventret);
				if (eventret == 0) {
					iRet = LL_GetLastError(tvSession[0]);
					LOG_PRINTF("Last Error Ret : %d", eventret);
					break;
				}
				while (1) {
					iNbrBytes = LL_Receive(tvSession[0], sizeof(pucBuffer),
							pucBuffer + iLength, 300);
					iLength += iNbrBytes;
					iRet = LL_GetLastError(tvSession[0]);
					LOG_PRINTF("no bytes : %d , Length : %d", iNbrBytes,
							iLength);
					LOG_PRINTF("Last Error Ret : %d", iRet);
					if (iRet != LL_ERROR_OK && iRet != LL_ERROR_TIMEOUT) {
						LOG_PRINTF("Error Ret : %d", iRet);
						icheck = 0;
						break;
					}
					if (iRet == LL_ERROR_TIMEOUT) {
						LOG_PRINTF("TIME OUT BREAK");
						break;
					}
					if (strstr(pucBuffer, "}}}") != NULL) {
						break;
					}
					if (iNbrBytes == 0) {
						LOG_PRINTF("No Of Bytes 0 break");
						break;
					}
				}
				break;
			}

			if (icheck > 0 && eventret > 0) {
				PrintLog("Before Parse");
				iRet = ParseAuthToken(pucBuffer, gAuthToken);
				sprintf(strPrint, "Parser Return  : %d ", iRet);
				PrintLog(strPrint);
				PrintLog("After ParseAuthToken()");
				switch (iRet) {
				case 0:
					DrawPrompt(App_Name, "LOGIN SUCCESS !!",
							"", 2);
					retVal=TRUE;
					break;
				case -1:
					DrawPrompt(App_Name, "Invalid User ID Or",
							"Password", 2);
					break;
				default: //else
					DrawPrompt(App_Name, "LOGIN FAILED !!", "", 2);
					break;
			}
			}
			else {
				DrawPrompt(App_Name, "Recieve Error", "", 1);
			}
			ret = LL_ClearReceiveBuffer(tvSession[0]);
			sprintf(strPrint, "Recieve Clear : %d ", ret);
			PrintLog(strPrint);
			ret = LL_Disconnect(tvSession[0]);
			sprintf(strPrint, "DisConnect : %d ", ret);
			PrintLog(strPrint);
		} else if (iRet == -1015) {
			DrawPrompt(App_Name, "ETHERNET NOT CONNECTED", "", 3);
		} else {
			DrawPrompt(App_Name, "Connection Failed", "", 3);
		}
		LL_Configure(tvSession, NULL);
	} else {
		DrawPrompt(App_Name, "MISSING LINKLAYER", "", 3);
	}
	TlvTree_Release(piConfig);
	return retVal;
}



int TCPGetCustomerList() {
	// Local variables
	// ***************
	char strPrint[2000] = { 0 };
	int iRet = 0, ret = 0, retVal=FALSE;

	PrintLog("Inside TCPGetCustomerList()");

	piConfig = TlvTree_New(LL_TAG_LINK_LAYER_CONFIG);
	piPhysicalConfig = TlvTree_AddChild(piConfig, LL_TAG_PHYSICAL_LAYER_CONFIG,
			NULL, 0);
	TlvTree_AddChildInteger(piPhysicalConfig, LL_PHYSICAL_T_LINK,
			LL_PHYSICAL_V_ETHERNET, LL_PHYSICAL_L_LINK);
	piTransportConfig = TlvTree_AddChild(piConfig,
			LL_TAG_TRANSPORT_LAYER_CONFIG, NULL, 0);
	TlvTree_AddChildInteger(piTransportConfig, LL_TRANSPORT_T_PROTOCOL,
			LL_TRANSPORT_V_TCPIP, LL_TRANSPORT_L_PROTOCOL);
//	TlvTree_AddChildString( piTransportConfig, LL_TCPIP_T_SSL_PROFILE, szProfile);
	TlvTree_AddChildString(piTransportConfig, LL_TCPIP_T_HOST_NAME,
			"http://ontab-demo.herokuapp.com");
	TlvTree_AddChildInteger(piTransportConfig, LL_TCPIP_T_PORT, 80,
			LL_TCPIP_L_PORT);

	iRet = LL_Configure(tvSession, piConfig);
	sprintf(strPrint, "CONFIG RET : %d", iRet);
	PrintLog(strPrint);
	if (iRet == LL_ERROR_OK) {
		int iLength = 0;
		int eventret = 1;
		int icheck = 1;
		int iNbrBytes = 0;
		char pucBuffer[4096] = { 0 };
		char GET_REQUEST[1000] = { 0 };
		char tempRequest[200] = { 0 };
		sprintf(tempRequest,
				"auth_token=%s",
				gAuthToken);
		sprintf(GET_REQUEST, "%s\r\n%s\r\n%s\r\n%s %d\r\n%s\r\n\r\n%s",
				"GET /api/v1/checkins HTTP/1.1",
				"Host: ontab-demo.herokuapp.com", "Accept: */*",
				"Content-Length:", (int)strlen(tempRequest),
				"Content-Type: application/x-www-form-urlencoded",tempRequest);

		iRet = LL_Connect(tvSession[0]);
		if (iRet == LL_ERROR_OK) {

			sprintf(strPrint, "Connect RET : %d", iRet);
			PrintLog(strPrint);
			Sending: iLength = 0;
			iNbrBytes = 0;
			memset(pucBuffer, 0, sizeof(pucBuffer));
			PrintLog(GET_REQUEST);
			iRet = LL_Send(tvSession[0], strlen(GET_REQUEST), GET_REQUEST,
					1000);
			sprintf(strPrint, "Sending RET : %d", iRet);
			PrintLog(strPrint);
			iRet = 0;
			LL_ClearReceiveBuffer(tvSession[0]);
			while (1) {
				LL_ClearReceiveBuffer(tvSession[0]);
				eventret = LL_WaitEvent(tvSession[0], LL_WAIT_RECEIVED_DATA,
						1500);
				LOG_PRINTF("Wait Event Ret : %d", eventret);
				if (eventret == 0) {
					iRet = LL_GetLastError(tvSession[0]);
					LOG_PRINTF("Last Error Ret : %d", eventret);
					break;
				}
				while (1) {
					iNbrBytes = LL_Receive(tvSession[0], sizeof(pucBuffer),
							pucBuffer + iLength, 300);
					iLength += iNbrBytes;
					iRet = LL_GetLastError(tvSession[0]);
					LOG_PRINTF("no bytes : %d , Length : %d", iNbrBytes,
							iLength);
					LOG_PRINTF("Last Error Ret : %d", iRet);
					if (iRet != LL_ERROR_OK && iRet != LL_ERROR_TIMEOUT) {
						LOG_PRINTF("Error Ret : %d", iRet);
						icheck = 0;
						break;
					}
					if (iRet == LL_ERROR_TIMEOUT) {
						LOG_PRINTF("TIME OUT BREAK");
						break;
					}
					if (strstr(pucBuffer, "}}}") != NULL) {
						break;
					}
					if (iNbrBytes == 0) {
						LOG_PRINTF("No Of Bytes 0 break");
						break;
					}
				}
				break;
			}

			if (icheck > 0 && eventret > 0) {
				PrintLog("Before Parse");
				iRet = ParseCustomerList(pucBuffer);
				sprintf(strPrint, "Parser Return  : %d ", iRet);
				PrintLog(strPrint);
				PrintLog("After ParseAuthToken()");
				switch (iRet) {
				case 0:
					retVal=TRUE;
					break;
				case -2:
					DrawPrompt(App_Name, "No Customers Checked in", "", 2);
					break;
				default: //else
					DrawPrompt(App_Name, "No Customers Checked in", "", 2);
					break;
			}
			}
			else {
				DrawPrompt(App_Name, "Recieve Error", "", 1);
			}
			ret = LL_ClearReceiveBuffer(tvSession[0]);
			sprintf(strPrint, "Recieve Clear : %d ", ret);
			PrintLog(strPrint);
			ret = LL_Disconnect(tvSession[0]);
			sprintf(strPrint, "DisConnect : %d ", ret);
			PrintLog(strPrint);
		} else if (iRet == -1015) {
			DrawPrompt(App_Name, "ETHERNET NOT CONNECTED", "", 3);
		} else {
			DrawPrompt(App_Name, "Connection Failed", "", 3);
		}
		LL_Configure(tvSession, NULL);
	} else {
		DrawPrompt(App_Name, "MISSING LINKLAYER", "", 3);
	}
	TlvTree_Release(piConfig);
	return retVal;
}


int ConnectEthernet() {
	// Local variables
	// ***************
	char strPrint[2000] = { 0 };
	int iRet = 0, ret = 0;
	int iKey=0;
	int bCancelRequest=FALSE;
	int bCancelRequestSent=FALSE;

	PrintLog("Inside Connect");

	piConfig = TlvTree_New(LL_TAG_LINK_LAYER_CONFIG);
	piPhysicalConfig = TlvTree_AddChild(piConfig, LL_TAG_PHYSICAL_LAYER_CONFIG,
			NULL, 0);
	TlvTree_AddChildInteger(piPhysicalConfig, LL_PHYSICAL_T_LINK,
			LL_PHYSICAL_V_ETHERNET, LL_PHYSICAL_L_LINK);
	piTransportConfig = TlvTree_AddChild(piConfig,
			LL_TAG_TRANSPORT_LAYER_CONFIG, NULL, 0);
	TlvTree_AddChildInteger(piTransportConfig, LL_TRANSPORT_T_PROTOCOL,
			LL_TRANSPORT_V_TCPIP, LL_TRANSPORT_L_PROTOCOL);
//	TlvTree_AddChildString( piTransportConfig, LL_TCPIP_T_SSL_PROFILE, szProfile);
	TlvTree_AddChildString(piTransportConfig, LL_TCPIP_T_HOST_NAME,
			"http://ontab-demo.herokuapp.com");
	TlvTree_AddChildInteger(piTransportConfig, LL_TCPIP_T_PORT, 80,
			LL_TCPIP_L_PORT);

	iRet = LL_Configure(tvSession, piConfig);
	sprintf(strPrint, "CONFIG RET : %d", iRet);
	PrintLog(strPrint);
	if (iRet == LL_ERROR_OK) {
		int iLength = 0;
		int eventret = 1;
		int icheck = 1;
		int iNbrBytes = 0;
		char pucBuffer[4096] = { 0 };
		char GET_REQUEST[1000] = { 0 };
		char CANCEL_REQUEST[1000] = { 0 };
		int InvoiceNo = 1;
		char tempRequest[200] = { 0 };
		char tempCancelRequest[200] = { 0 };

		if(IDTypeSelection==ByCustomerName)
		{
			sprintf(tempRequest,
					"auth_token=%s&customer_id=%s&invoice[amount]=%s",gAuthToken,
					customerList.szUserId[customerList.iSelected],strAmount);
		}
		else
		{
			sprintf(tempRequest,
					"auth_token=%s&invoice[amount]=%s&tab_num=%s",gAuthToken,
					strAmount, strTabNum);
		}
		sprintf(GET_REQUEST, "%s\r\n%s\r\n%s\r\n%s %d\r\n%s\r\n\r\n%s",
				"POST /api/v1/invoices HTTP/1.1",
				"Host: ontab-demo.herokuapp.com", "Accept: */*",
				"Content-Length:", strlen(tempRequest),
				"Content-Type: application/x-www-form-urlencoded",tempRequest);

		iRet = LL_Connect(tvSession[0]);
		if (iRet == LL_ERROR_OK) {

			sprintf(strPrint, "Connect RET : %d", iRet);
			PrintLog(strPrint);
			Sending: iLength = 0;
			iNbrBytes = 0;
			memset(pucBuffer, 0, sizeof(pucBuffer));
			if(bCancelRequest==TRUE)
			{
				DrawPrompt(App_Name, "Sending Cancel Request",
						"Please Wait", 0);
				PrintLog(CANCEL_REQUEST);
				iRet = LL_Send(tvSession[0], strlen(CANCEL_REQUEST), CANCEL_REQUEST,
						1000);
			}
			else {
				PrintLog(GET_REQUEST);
				iRet = LL_Send(tvSession[0], strlen(GET_REQUEST), GET_REQUEST,
						1000);
			}
			sprintf(strPrint, "Sending RET : %d", iRet);
			PrintLog(strPrint);
			iRet = 0;
			LL_ClearReceiveBuffer(tvSession[0]);
			while (1) {
				LL_ClearReceiveBuffer(tvSession[0]);
				eventret = LL_WaitEvent(tvSession[0], LL_WAIT_RECEIVED_DATA,
						1500);
				LOG_PRINTF("Wait Event Ret : %d", eventret);
				if (eventret == 0) {
					iRet = LL_GetLastError(tvSession[0]);
					LOG_PRINTF("Last Error Ret : %d", eventret);
					break;
				}
				while (1) {
					iNbrBytes = LL_Receive(tvSession[0], sizeof(pucBuffer),
							pucBuffer + iLength, 300);
					iLength += iNbrBytes;
					iRet = LL_GetLastError(tvSession[0]);
					LOG_PRINTF("no bytes : %d , Length : %d", iNbrBytes,
							iLength);
					LOG_PRINTF("Last Error Ret : %d", iRet);
					if (iRet != LL_ERROR_OK && iRet != LL_ERROR_TIMEOUT) {
						LOG_PRINTF("Error Ret : %d", iRet);
						icheck = 0;
						break;
					}
					if (iRet == LL_ERROR_TIMEOUT) {
						LOG_PRINTF("TIME OUT BREAK");
						break;
					}
					if (strstr(pucBuffer, "}}}") != NULL) {
						break;
					}
					if (iNbrBytes == 0) {
						LOG_PRINTF("No Of Bytes 0 break");
						break;
					}
				}
				break;
			}

			/*   while(iRet>500)
			 {
			 iRet=LL_Receive( tvSession[0], 500, pucBuffer,100);
			 sprintf(strPrint,"Recieving RET : %d ",iRet);
			 PrintLog(strPrint);
			 LL_WaitEvent  (tvSession[0],LL_WAIT_RECEIVED_DATA ,100);

			 }*/
			if (icheck > 0 && eventret > 0) {
				PrintLog("Before Parse");
				if(bCancelRequest==TRUE && bCancelRequestSent==FALSE)
					iRet = ParseCancelResponse(pucBuffer, &InvoiceNo);
				else
					iRet = ParseResponse(pucBuffer, &InvoiceNo);
				sprintf(strPrint, "Parser Return  : %d ", iRet);
				PrintLog(strPrint);
				PrintLog("After Parse");
				switch (iRet) {
				case 1: //if(iRet==1)
				{
					DrawPrompt(App_Name, "TRANSACTION SUCCESS",
							"PRINTING RECIEPT", 0);
					PrintReceipt(0);
					break;
				}
				case 0: //else if(iRet==0)
				{
					memset(GET_REQUEST, 0, sizeof(GET_REQUEST));
					sprintf(
							tempRequest,
							"GET /api/v1/invoices/%d?auth_token=%s HTTP/1.1",
							InvoiceNo,gAuthToken);
					sprintf(GET_REQUEST, "%s\r\n%s\r\n%s\r\n\r\n", tempRequest,
							"Host: ontab-demo.herokuapp.com", "Accept: */*");
					if(bCancelRequest || bCancelRequestSent)
					{
						DrawPrompt(App_Name, "Cancel Request Sent",
								"Please Wait", 0);
						bCancelRequestSent=TRUE;
					}
					else
						DrawPrompt(App_Name, "Bill Sent Successfully",
								"Waiting for Approval", 0);

					do{
						iKey=fs_UIGetChar(3);
					}while(iKey!=0 && iKey!=23);

					if(bCancelRequest)
						bCancelRequest=FALSE;

					if(iKey==23)
					{
						memset(CANCEL_REQUEST, 0, sizeof(CANCEL_REQUEST));
						memset(tempCancelRequest, 0, sizeof(tempCancelRequest));
						sprintf(
								tempCancelRequest,
								"%d?auth_token=%s&invoice[status]=merchant_voided",
								InvoiceNo,gAuthToken);
						sprintf(CANCEL_REQUEST, "%s%s %s\r\n%s\r\n%s\r\n\r\n",
							"PUT /api/v1/invoices/",tempCancelRequest,"HTTP/1.1",
							"Host: ontab-demo.herokuapp.com", "Accept: */*"
/*
							"Content-Length:", strlen(tempCancelRequest),
							"Content-Type: application/x-www-form-urlencoded");
*/
							);
						bCancelRequest=TRUE;
					}
					ret = LL_ClearReceiveBuffer(tvSession[0]);
					goto Sending;
					break;
				}
				case -1: //else if(iRet==-1)
				{
					DrawPrompt(App_Name, "CUSTOMER NOT FOUND", "TRY AGAIN", 2);
					break;
				}
				case 2: {
					DrawPrompt(App_Name, "Customer Rejected", "", 0);
					PrintReceipt(1);
					break;

				}
				case 3: {
					DrawPrompt(App_Name, "Discarded", "", 2);
					break;

				}
				case 4: {
					DrawPrompt(App_Name, sRecieptVar.sRecieptStatus, "", 0);
					PrintReceipt(2);
					break;
				}
				case 5: {
					ret = LL_ClearReceiveBuffer(tvSession[0]);
					goto Sending;
					break;
				}
				default: //else
				{
					// PrintLog(pucBuffer);
					DrawPrompt(App_Name, "INVALID RESPONSE", "", 2);
					break;
				}
				}
			} else {
				DrawPrompt(App_Name, "Recieve Error", "", 1);
			}
			ret = LL_ClearReceiveBuffer(tvSession[0]);
			sprintf(strPrint, "Recieve Clear : %d ", ret);
			PrintLog(strPrint);
			ret = LL_Disconnect(tvSession[0]);
			sprintf(strPrint, "DisConnect : %d ", ret);
			PrintLog(strPrint);
		} else if (iRet == -1015) {
			DrawPrompt(App_Name, "ETHERNET NOT CONNECTED", "", 3);
		} else {
			DrawPrompt(App_Name, "Connection Failed", "", 3);
		}
		LL_Configure(tvSession, NULL);
	} else {
		DrawPrompt(App_Name, "MISSING LINKLAYER", "", 3);
	}

	TlvTree_Release(piConfig);
	return iRet;
}

int ParseResponse(char *pucBuffer, int *InvoiceNo) {
	int i, j = 0, iRet;
	char sResponse[4096], status[50];
	char sHeader[4096];
	char invoice[10];
	char sPrint[200];
	char *sstatus;
	int idatalen;
	//int iRet=0;
	memset(sResponse, 0, sizeof(sResponse));
	memset(sHeader, 0, sizeof(sHeader));
	memset(status, 0, sizeof(status));
	memset(sPrint, 0, sizeof(sPrint));
	memset(invoice, 0, sizeof(invoice));
	PrintLog("Inside Parser");
	for (i = 0; i < strlen(pucBuffer); i++) {
		if (pucBuffer[i] == '{')
			break;
		sHeader[i] = pucBuffer[i];
	}
	sHeader[i + 1] = '\0';
	strcpy(sResponse, &pucBuffer[i]);
	PrintLog(sHeader);
	PrintLog(&pucBuffer[i]);
	/*sdatalen=strstr(sHeader,"Content-Length:");
	 if(sdatalen!=NULL)
	 {
	 sdatalen+=16;
	 idatalen=atoi(sdatalen);
	 LOG_PRINTF("CONTENT LENGTH : \"%s\"",sdatalen);
	 LOG_PRINTF("CONTENT LENGTH : %d",idatalen);

	 }
	 else
	 {
	 return RESEND;
	 }
	 if((idatalen+strlen(sHeader))>(strlen(pucBuffer)))
	 {
	 return RESEND;
	 }*/
	for (i = 0; i < strlen(sResponse); i++) {
		if (sResponse[i] == 34) {
			i++;
			j = 0;
			while (sResponse[i] != 34) {
				status[j] = sResponse[i];
				j++;
				i++;
			}
			status[j] = '\0';
			i++;
			PrintLog(status);
			break;
		}
	}

	if (strcmp(status, "errors") == 0)
		return -1;
	else if (strstr(sResponse, "pending") != NULL) {
		i++;
		j = 0;
		while (sResponse[i] != 44) /* 44 is ASCII OF COMMA(,) */
		{
			invoice[j] = sResponse[i];
			i++;
			j++;
		}
		invoice[j] = '\0';
		*InvoiceNo = atoi(invoice);
		sprintf(sPrint, "Invoice No. : %d : %s", *InvoiceNo, invoice);
		PrintLog(sPrint);
		return 0;
	} else if (strstr(sResponse, "\"status\":\"paid\"") != NULL) {
		iRet = ParseApprovalResponse(sResponse);
		PrintLog("After ParseApprovalResponse()");
		ttestall(0, 100);
		return 1;
	} else if (strstr(sResponse, "\"status\":\"customer_rejected\"") != NULL) {
		iRet = ParseApprovalResponse(sResponse);
		PrintLog("After ParseApprovalResponse()");
		ttestall(0, 100);
		return 2;
	} else if (strstr(sResponse, "\"status\":") != NULL) {
		iRet = ParseApprovalResponse(sResponse);
		return 4;
	} else {
		PrintLog(sResponse);
		return -2;
	}

}


int ParseCancelResponse(char *pucBuffer, int *InvoiceNo) {

	char sResponse[4096], status[50];
	char sHeader[4096];
	char invoice[10];
	char sPrint[200];

	//int iRet=0;
	memset(sResponse, 0, sizeof(sResponse));
	memset(sHeader, 0, sizeof(sHeader));
	memset(status, 0, sizeof(status));
	memset(sPrint, 0, sizeof(sPrint));
	memset(invoice, 0, sizeof(invoice));
	PrintLog("Inside ParseCancelResponse()");

	return 0;
}

int ParseAuthToken(char *pucBuffer, char *authToken) {
	int i, j = 0;
	char sResponse[4096], status[100];
	char sHeader[4096];
	char strAuthToken[100]={0};
	char sPrint[200];
	memset(sResponse, 0, sizeof(sResponse));
	memset(sHeader, 0, sizeof(sHeader));
	memset(status, 0, sizeof(status));
	memset(sPrint, 0, sizeof(sPrint));

	PrintLog("Inside ParseAuthToken()");
	for (i = 0; i < strlen(pucBuffer); i++) {
		if (pucBuffer[i] == '{')
			break;
		sHeader[i] = pucBuffer[i];
	}
	sHeader[i + 1] = '\0';
	strcpy(sResponse, &pucBuffer[i]);
	PrintLog(sHeader);
	PrintLog(&pucBuffer[i]);

	if (strcasestr(sResponse, "invalid"))	/* If invalid user or password */
		return -1;

	for (i = 0; i < strlen(sResponse); i++) {
		if (sResponse[i] == '"') {	/* '"' */
			i++;
			j = 0;
			while (sResponse[i] != '"' && sResponse[i] != '\0') {
				status[j] = sResponse[i];
				j++;
				i++;
			}
			status[j] = '\0';
			i++;
			PrintLog(status);
			break;
		}
	}

	if (!strcasestr(status, "authentication_token"))	/* If it does not find label */
		return -2;

	i++;
	while (sResponse[i] != '"' && sResponse[i] != '\0') /* Find next '"' character */
		i++;
	if(sResponse[i] != '"')	/* Character '"' not found */
		return -1;

	i++;
	j = 0;
	while (sResponse[i] != '"' && sResponse[i] != '\0') /* Copy data till we get '"' character */
	{
		strAuthToken[j++]=sResponse[i++];
	}
	strAuthToken[j] = '\0';
	strcpy(authToken,strAuthToken);
	sprintf(sPrint, "Auth Token : %s", strAuthToken);
	PrintLog(sPrint);
	return 0;

}

int ParseCustomerList(char *pucBuffer) {
	int i, j = 0;
	int iLoop;
	char sResponse[4096], status[100];
	char sHeader[4096];
	char strAuthToken[100]={0};
	char sPrint[4000];
	char *pResp,*pName,*pUID;
	memset(sResponse, 0, sizeof(sResponse));
	memset(sHeader, 0, sizeof(sHeader));
	memset(status, 0, sizeof(status));
	memset(sPrint, 0, sizeof(sPrint));

	memset(&customerList,0,sizeof(_CUSTOMER));

	PrintLog("Inside ParseCustomerList()");

	pResp=strchr(pucBuffer,'{');
	if(pResp=='\0')
		return -1;


	while(pResp && *pResp && strcasestr(pResp,"id") && strcasestr(pResp,"name") )
	{

		pName=strcasestr(pResp,"name");
		pUID=strcasestr(pResp,"id");

		if(*(pName+4)=='"')	/* Check if Name is mentioned in double qoutes */
			pName+=5;
		while(*pName!='"' && *pName)	/* Find '"' quotes */
			pName++;
		if(*pName=='\0')
			break;

		pName++;	/* Point to next character of '"' */

		j=0;
		while((*pName)!='"' && *pName && (j<50))	/* Find '"' quotes */
		{
			customerList.szName[customerList.iNoOfCustomers][j++]=*pName;
			pName++;
		}
		if(*pName=='\0')
			break;

		if(*(pUID+2)=='"')	/* Check if user_id is mentioned in double qoutes */
			pUID+=3;

		while(((*pUID)<'0') || ((*pUID)>'9'))	/* Find first digit */
			pUID++;
		if(*pUID=='\0')
			break;

		j=0;
		while(*pUID>='0' && *pUID<='9' && *pUID && (j<10))
		{
			customerList.szUserId[customerList.iNoOfCustomers][j++]=*pUID;
			sprintf(sPrint, "ParseCustomerList() : UID[%d]=%c",j-1,*pUID);
			PrintLog(sPrint);
			pUID++;
		}
		if(*pUID=='\0')
			break;

		customerList.iNoOfCustomers++;

		//pResp=pUID;	/* Point to next data set */
		pResp=strchr(pName,'}');	/* Point to next data set */

		sprintf(sPrint, "ParseCustomerList() : Name[%d]=%s",customerList.iNoOfCustomers-1, customerList.szName[customerList.iNoOfCustomers-1]);
		PrintLog(sPrint);
		sprintf(sPrint, "ParseCustomerList() : UID[%d]=%s", customerList.iNoOfCustomers-1,customerList.szUserId[customerList.iNoOfCustomers-1]);
		PrintLog(sPrint);
	}

	if(customerList.iNoOfCustomers)
	return 0;

	return -2;
}


void PrintAddress() {
	char addresslines[5][20] = { 0 };
	int i = 0, j = 0, k = 0;
	while (strlen(sRecieptVar.sRecieptAddress) >= i) {
		addresslines[j][k] = sRecieptVar.sRecieptAddress[i];
		if (sRecieptVar.sRecieptAddress[i] == 44) {
			k++;
			addresslines[j][k] = '\0';
			j++;
			k = -1;
		}
		i++;
		k++;
	}
	addresslines[j][k] = '\0';
	for (i = 0; i <= j; i++)
		fs_PrinterAppendLine(addresslines[i]);

}

void print_datetime() {
	char sdatetime[30] = { 0 };
	int i = 0, ilen = 0;
	ilen = strlen(sRecieptVar.sRecieptCreatedat);
	for (i = 0; i < ilen; i++) {
		if (isalpha(sRecieptVar.sRecieptCreatedat[i]) != 0) {
			sdatetime[i] = ' ';
		} else {
			sdatetime[i] = sRecieptVar.sRecieptCreatedat[i];
		}
	}
	sdatetime[ilen] = 0;
	LOG_PRINTF("Date : %s", sdatetime);
	fs_PrinterAppendLine(sdatetime);
}

void PrintStatus() {
	char sStatusUppercase[50] = { 0 };
	char sStatus1[20] = { 0 };
	char sStatus2[20] = { 0 };
	int len = 0, i = 0, j = 0;
	len = strlen(sRecieptVar.sRecieptStatus);
	for (i = 0; i < len; i++)
		sStatusUppercase[i] = toupper(sRecieptVar.sRecieptStatus[i]);
	sStatusUppercase[len] = '\0';
	LOG_PRINTF("STATUS In Upper Case : %s", sStatusUppercase);
	j = 0;
	i = 0;
	while (sStatusUppercase[i] != '_' && sStatusUppercase[i] != '\0') {
		sStatus1[j] = sStatusUppercase[i];
		i++;
		j++;
	}
	sStatus1[j] = '\0';
	LOG_PRINTF("Status 1 : %s", sStatus1);
	if (sStatusUppercase[i] == '_') {
		i++;
		j = 0;
		while (sStatusUppercase[i] != '_' && sStatusUppercase[i] != '\0') {
			sStatus2[j] = sStatusUppercase[i];
			i++;
			j++;
		}
		sStatus2[j] = '\0';
		LOG_PRINTF("Status 2 : %s", sStatus2);
	}

	fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_CENTER);
	fs_PrinterAppendLine(sStatus1);
	if (sStatus2[0] != '\0')
		fs_PrinterAppendLine(sStatus2);

}

void PrintReceipt(int status) {
	char amountLeft[30] = { 0 };
	char tipsLeft[20] = { 0 };
	char amountRight[30] = { 0 };
	char tipsRight[20] = { 0 };
	char totalAmountLeft[40] = { 0 };
	char totalAmountRight[40] = { 0 };
	char tempamount[20] = { 0 };
	char invoiceLine[40] = { 0 };
	char cardLine[40] = { 0 };
	char tokenLine[40] = { 0 };

	sRecieptVar.fRecieptTotal = sRecieptVar.fRecieptAmount
			+ sRecieptVar.fRecieptTips;
	sprintf(tempamount, "%ld.%02ld", sRecieptVar.fRecieptAmount / 100,
			sRecieptVar.fRecieptAmount % 100);
	sprintf(amountLeft, "Amount:");
	sprintf(amountRight, "$%s", tempamount);

	sprintf(tempamount, "%ld.%02ld", sRecieptVar.fRecieptTips / 100,
			sRecieptVar.fRecieptTips % 100);
	sprintf(tipsLeft, "Tips:");
	sprintf(tipsRight, "$%s", tempamount);

	sprintf(tempamount, "%ld.%02ld", sRecieptVar.fRecieptTotal / 100,
			sRecieptVar.fRecieptTotal % 100);
	sprintf(totalAmountLeft, "Total(%s):",sRecieptVar.sRecieptCurrency[0]?sRecieptVar.sRecieptCurrency:"CAD");
	sprintf(totalAmountRight, "$%s",tempamount);

	sprintf(invoiceLine,"INV#: %s CHG#:%s",sRecieptVar.sRecieptInvoiceID,sRecieptVar.sRecieptChargeID);
	if(sRecieptVar.sRecieptCardType[0])
		sprintf(cardLine,"%s XXXX %s",sRecieptVar.sRecieptCardType,sRecieptVar.sRecieptCardNumber);
	else
		strcpy(cardLine,"   ");
	sprintf(tokenLine,"AUTH: %s",sRecieptVar.sRecieptChargeRemoteToken);

	fs_PrinterInit();
	fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_CENTER);
	fs_PrinterAppendLine(sRecieptVar.sRecieptName);
	PrintAddress();
	fs_PrinterSetCondensed(FALSE);
	fs_PrinterAppendLine("");
	fs_PrinterSetBold(TRUE);
	fs_PrinterAppendLine(strTransactionType);
	fs_PrinterSetBold(FALSE);
	fs_PrinterAppendLine("");
	fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_LEFT);

	fs_PrinterAppendLine(invoiceLine);
	fs_PrinterAppendLine(cardLine);
	fs_PrinterAppendLine(tokenLine);

	fs_PrinterAppendLine("");
	print_datetime();
	//fs_PrinterAppendLine(sRecieptVar.sRecieptCreatedat);
	fs_PrinterAppendLine("");
	fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_LEFT);
	//fs_PrinterAppendLine(amount);
	fs_PrinterLRAppendLineLR(amountLeft,amountRight);
	//fs_PrinterAppendLine("Taxes : ");
	//fs_PrinterAppendLine(tips);
	fs_PrinterLRAppendLineLR(tipsLeft,tipsRight);
	fs_PrinterAppendLine("");
	//fs_PrinterAppendLine(totalAmount);
	fs_PrinterLRAppendLineLR(totalAmountLeft,totalAmountRight);
	fs_PrinterAppendLine("");
	fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_CENTER);
	fs_PrinterSetBold(TRUE);
	fs_PrinterSetInverse(TRUE);
//	fs_PrinterAppendLine(sRecieptVar.sRecieptStatus);
	fs_PrinterSetBold(TRUE);
	/*if(status==0)
	 {
	 fs_PrinterAppendLine("PAID");
	 }
	 else if(status==1)
	 {
	 fs_PrinterAppendLine("CUSTOMER");
	 fs_PrinterAppendLine("REJECTED");
	 }
	 else*/
	{
		PrintStatus();

	}
	fs_PrinterSetBold(FALSE);
	fs_PrinterSetInverse(FALSE);
	fs_PrinterAppendLine("");
	fs_PrinterSetJustification(FS_PRINTER_JUSTIFY_CENTER);
	fs_PrinterAppendLine("************************");
	fs_PrinterAppendLine("Thank you for payment");
	fs_PrinterFormFeed();
	fs_PrinterClose();
	//ttestall(0,100);
}

int ParseApprovalResponse(char *sResponse) {

	int i = 0, j = 0, len = 0;
	char Key[100] = { 0 };
	int iBelongToCharges=FALSE;
	int bObtainedChargeToken=FALSE;
	int bChargeID=FALSE;
	int bInvoiceID=FALSE;
	char tempAmount[20] = { 0 }, sPrint[500] = { 0 }, tempString[500] = { 0 };
	len = strlen(sResponse);
	while (i < len) {
		if (sResponse[i] == 34) {
			memset(Key, 0, sizeof(Key));
			i++;
			j = 0;
			while (sResponse[i] != 34) {
				Key[j] = sResponse[i];
				j++;
				i++;
			}
			Key[j] = '\0';
			i++;
			PrintLog(Key);
			//ttestall(0,100);
		} else {
			i++;
			//	ttestall(0,100);
			continue;
		}

		PrintLog(Key);
		if ((iBelongToCharges==FALSE) && strcmp(Key, "amount") == 0) {
			j = 0;
			i++;
			memset(tempAmount, 0, sizeof(tempAmount));
			while (sResponse[i] != 44) {
				tempAmount[j] = sResponse[i];
				i++;
				j++;
			}
			tempAmount[j] = '\0';
			sRecieptVar.fRecieptAmount = atol(tempAmount);
			memset(sPrint, 0, sizeof(sPrint));
			sprintf(sPrint, "Amount. : %ld , %s", sRecieptVar.fRecieptAmount,
					tempAmount);
			PrintLog(sPrint);
			i++;
		}
		else if ((iBelongToCharges==FALSE) && (bInvoiceID==FALSE) && strcasecmp(Key, "id") == 0) {
				j = 0;
				i++;
				memset(tempString, 0, sizeof(tempString));
				while (sResponse[i] != 44) {
					tempString[j] = sResponse[i];
					i++;
					j++;
				}
				tempString[j] = '\0';
				strcpy(sRecieptVar.sRecieptInvoiceID, tempString);
				sprintf(sPrint, "Invoice ID : %s ", sRecieptVar.sRecieptInvoiceID);
				PrintLog(sPrint);
				i++;
				bInvoiceID=TRUE;
		}
		else if (strcmp(Key, "tips") == 0) {
			j = 0;
			i++;
			memset(tempAmount, 0, sizeof(tempAmount));
			while (sResponse[i] != 44) {
				tempAmount[j] = sResponse[i];
				i++;
				j++;
			}
			tempAmount[j] = '\0';
			if (strcmp(tempAmount, "null") == 0)
				sRecieptVar.fRecieptTips = 0.00;
			else
				sRecieptVar.fRecieptTips = atol(tempAmount);
			memset(sPrint, 0, sizeof(sPrint));
			sprintf(sPrint, "Tips. : %ld , %s", sRecieptVar.fRecieptTips,
					tempAmount);
			PrintLog(sPrint);
			i++;

		} else if (strcmp(Key, "created_at") == 0) {
			if (sRecieptVar.sRecieptCreatedat[0] == '\0') {
				j = 0;
				memset(tempString, 0, sizeof(tempString));
				while (sResponse[i] != 34) {
					i++;
				}
				i++;
				while (sResponse[i] != 34) {
					tempString[j] = sResponse[i];
					i++;
					j++;
				}

				tempString[j] = '\0';
				strcpy(sRecieptVar.sRecieptCreatedat, tempString);
				sprintf(sPrint, "Created at. : %s ",
						sRecieptVar.sRecieptCreatedat);
				PrintLog(sPrint);
				i++;
			} else {
				while (sResponse[i] != 44) {
					i++;
				}
				i++;
			}
		} else if (strcmp(Key, "point_of_sale") == 0) {
			i++;
		} else if (strcmp(Key, "address_descriptor") == 0) {
			j = 0;
			memset(tempString, 0, sizeof(tempString));
			while (sResponse[i] != 34) {
				i++;
			}
			i++;
			while (sResponse[i] != 34) {
				tempString[j] = sResponse[i];
				i++;
				j++;
			}
			i++;
			tempString[j] = '\0';
			strcpy(sRecieptVar.sRecieptAddress, tempString);
			sprintf(sPrint, "Address : %s", sRecieptVar.sRecieptAddress);
			PrintLog(sPrint);
			i++;
		} else if (strcmp(Key, "name") == 0) {
			j = 0;
			memset(tempString, 0, sizeof(tempString));
			while (sResponse[i] != 34) {
				i++;
			}
			i++;
			while (sResponse[i] != 34) {
				tempString[j] = sResponse[i];
				i++;
				j++;
			}
			i++;
			tempString[j] = '\0';
			strcpy(sRecieptVar.sRecieptName, tempString);
			sprintf(sPrint, "Name : %s ", sRecieptVar.sRecieptName);
			PrintLog(sPrint);
			i++;
		}
		else if (strcasecmp(Key, "status") == 0) {
			j = 0;
			memset(tempString, 0, sizeof(tempString));
			while (sResponse[i] != 34) {
				i++;
			}
			i++;
			while (sResponse[i] != 34) {
				tempString[j] = sResponse[i];
				i++;
				j++;
			}
			i++;
			tempString[j] = '\0';
			strcpy(sRecieptVar.sRecieptStatus, tempString);
			sprintf(sPrint, "Status : %s ", sRecieptVar.sRecieptStatus);
			PrintLog(sPrint);
			i++;
		}
		else if (strcasecmp(Key, "currency") == 0) {
			j = 0;
			memset(tempString, 0, sizeof(tempString));
			while (sResponse[i] != 34) {
				i++;
			}
			i++;
			while (sResponse[i] != 34) {
				tempString[j] = sResponse[i];
				i++;
				j++;
			}
			i++;
			tempString[j] = '\0';
			strcpy(sRecieptVar.sRecieptCurrency, tempString);
			sprintf(sPrint, "Currency : %s ", sRecieptVar.sRecieptCurrency);
			PrintLog(sPrint);
			i++;
		}
		else if (strcasecmp(Key, "charges") == 0) {
				i++;
				iBelongToCharges=TRUE;
		}
		else if ((iBelongToCharges==TRUE) && (bChargeID==FALSE) && strcasecmp(Key, "id") == 0) {
				j = 0;
				i++;
				memset(tempString, 0, sizeof(tempString));
				while (sResponse[i] != 44) {
					tempString[j] = sResponse[i];
					i++;
					j++;
				}
				tempString[j] = '\0';
				strcpy(sRecieptVar.sRecieptChargeID, tempString);
				sprintf(sPrint, "Charge ID : %s ", sRecieptVar.sRecieptChargeID);
				PrintLog(sPrint);
				i++;
				bChargeID=TRUE;
		}
		else if ((iBelongToCharges==TRUE) && strcasecmp(Key, "card_type") == 0) {
			j = 0;
			memset(tempString, 0, sizeof(tempString));
			while (sResponse[i] != 34) {
				i++;
			}
			i++;
			while (sResponse[i] != 34) {
				tempString[j] = sResponse[i];
				i++;
				j++;
			}
			i++;
			tempString[j] = '\0';
			strcpy(sRecieptVar.sRecieptCardType, tempString);
			sprintf(sPrint, "Card Type : %s ", sRecieptVar.sRecieptCardType);
			PrintLog(sPrint);
			i++;
		}
		else if ((iBelongToCharges==TRUE) && (bObtainedChargeToken==FALSE) && strcasecmp(Key, "remote_token") == 0) {
			j = 0;
			memset(tempString, 0, sizeof(tempString));
			while (sResponse[i] != 34) {
				i++;
			}
			i++;
			while (sResponse[i] != 34) {
				tempString[j] = sResponse[i];
				i++;
				j++;
			}
			i++;
			tempString[j] = '\0';
			strcpy(sRecieptVar.sRecieptChargeRemoteToken, tempString);
			sprintf(sPrint, "Charge Remote Token : %s ", sRecieptVar.sRecieptChargeRemoteToken);
			PrintLog(sPrint);
			i++;
			bObtainedChargeToken=TRUE;
		}
		else if ((iBelongToCharges==TRUE) && strcasecmp(Key, "last4") == 0) {
			j = 0;
			memset(tempString, 0, sizeof(tempString));
			while (sResponse[i] != 34) {
				i++;
			}
			i++;
			while (sResponse[i] != 34) {
				tempString[j] = sResponse[i];
				i++;
				j++;
			}
			i++;
			tempString[j] = '\0';
			strcpy(sRecieptVar.sRecieptCardNumber, tempString);
			sprintf(sPrint, "Last 4 Digits : %s ", sRecieptVar.sRecieptCardNumber);
			PrintLog(sPrint);
			i++;
		}
		else if ((iBelongToCharges==TRUE) && strcasecmp(Key, "created_at") == 0) {
					if (sRecieptVar.sRecieptChargeCreatedat[0] == '\0') {
						j = 0;
						memset(tempString, 0, sizeof(tempString));
						while (sResponse[i] != 34) {
							i++;
						}
						i++;
						while (sResponse[i] != 34) {
							tempString[j] = sResponse[i];
							i++;
							j++;
						}

						tempString[j] = '\0';
						strcpy(sRecieptVar.sRecieptChargeCreatedat, tempString);
						sprintf(sPrint, "Charge Created at. : %s ",
								sRecieptVar.sRecieptChargeCreatedat);
						PrintLog(sPrint);
						i++;
					} else {
						while (sResponse[i] != 44) {
							i++;
						}
						i++;
					}
		}
		else {
			while (sResponse[i] != 44 && sResponse[i] != 125)
				i++;
			//i++;
		}
		//ttestall(0,100);
	}
	return TRUE;
}

void DrawPrompt(char *Title, char *Text, char *Text1, int itimeout) {
	int iRight, iBottom;
	//int Column=2;
	int x = 0, y = 0;
	int FontHeight;
	int TextLen;
	int wait;
	wait = itimeout * 100 + 1;
	SetType(0);
	GetScreenSize(&iBottom, &iRight);
	SetArea(0, 0, iRight - 3, iBottom - 3, _OFF_);
	DrawWindow(0, 0, iRight - 1, iBottom - 1, 1, (unsigned char *) Title);
	FontHeight = GetPoliceSize();
	TextLen = StringWidth(Text);
	y = iBottom / 2;
	x = iRight - strlen(Text);
	x = x / 2;
	SetArea(10, 10, iRight - 3, iBottom - 3, _OFF_);
	DrawString(Text, 7, 2 * FontHeight, _OFF_);
	DrawString(Text1, 7, 3 * FontHeight, _OFF_);
	ttestall(0, wait);
}

int LoadGlobalData(void)
{
	S_FS_FILE *hFile;
	NO_SEGMENT No;
	unsigned char FlagDownload;
	unsigned char Type[100] = { 0 };
	static int siFirstInit = FALSE;

	No = ApplicationGetCurrent(); // Return the application number
	first_init(No, &FlagDownload, Type);
	raz_init(No);

	hFile=OpenFlashFile("","MOBILEDATA.TXT",FALSE);
	if(!hFile)
		return FALSE;

	memset(&globalData,0,sizeof(_global));
	if(FS_read(&globalData,sizeof(_global),1,hFile)!=1)
	{
		FS_close(hFile);
		return FALSE;
	}
	FS_close(hFile);


	if (FlagDownload == 0xFF && siFirstInit == FALSE) /* First Execution */
	{
		siFirstInit=TRUE;
		FS_unlink("/HOST/MOBILEDATA.TXT");
		return FALSE;
	}

	strcpy(gAuthToken,globalData.szAuthToken);
	return TRUE;
}

int SaveGlobalData(void)
{
	S_FS_FILE *hFile;

	hFile=OpenFlashFile("","MOBILEDATA.TXT",TRUE);
	if(!hFile)
		return FALSE;

	strcpy(globalData.szAuthToken,gAuthToken);
	if(FS_write(&globalData,sizeof(_global),1,hFile)!=1)
	{
		FS_close(hFile);
		return FALSE;
	}

	FS_close(hFile);
	return TRUE;
}
