#include "SDK30.H"
#include "Menu.h"
#include "Display.h"
#include "fsparms.h"
#include "parms.h"
#include "ui.h"
#include "Printer.h"
#include "Utils.h"
#include "Sale.h"
#include "Common.h"


int doSale(int iVar)
{
	char strKeyBuffer[21+1];
	char strBuffer[10+1]={0};
	static double fAmount=3.65;
	int ipSeparatorList[4]={11,8,5,0};	/* List must be NULL terminated */

	fs_UIInit();
	fs_DisplayInit();
	clearCommonDataAll();

	fs_UIGetCustomAmountEntry("Zippcard","Enter Amount:","",daAlignCenter,fs_ParmGetSelectedItem(&glParameters.slCurrecyList),2,daAlignRight,MIN_AMOUNT_LEN,MAX_AMOUNT_LEN,&fAmount,ipSeparatorList,glParameters.PromptTimeout,strKeyBuffer);

	return TRUE;
}
