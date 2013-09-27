#include "SDK30.H"
#include "Display.h"
#include "GL_GraphicLib.h"
#include "GL_Dialog.h"


int fs_DisplayInit(void)
{
	CreateGraphics(_SMALL_);
	StateHeader(0);
	SetFont(_SMALL_);

	return TRUE;
}

int fs_DisplayAtRow(int Row,char *Text,daAlign Alignment)
{
	int Column=0;
	int FontHeight;
	int WindowHeight,WindowWidth;
	int TextLen;
	int LineOffset=3;
	int iRight,iBottom;

	GetScreenSize(&iBottom,&iRight);
	DrawWindow(0,0,iRight-1,iBottom-1,1,"MOBILE");
	FontHeight=GetPoliceSize();
	GetScreenSize(&WindowHeight,&WindowWidth);
	TextLen=StringWidth(Text);

	switch(Alignment)
	{
		case daAlignCenter:
			if(WindowWidth>TextLen)
				Column=(WindowWidth-TextLen)/2;
			break;
		case daAlignRight:
			if(WindowWidth>TextLen)
				Column=(WindowWidth-TextLen);
			break;

		default:
			break;
	}
	SetArea(0,Row*(FontHeight+LineOffset),127,((Row+1)*(FontHeight+LineOffset)),_OFF_);
	DrawString(Text,Column,Row*(FontHeight+LineOffset)+1,_OFF_);
return 1;
}


int fs_DisplayAtRowOverWrite(int Row,char *Text,daAlign Alignment)
{
	int Column=2;
	int FontHeight;
	int WindowHeight,WindowWidth;
	int TextLen;
	int LineOffset=3;

	FontHeight=GetPoliceSize();
	GetScreenSize(&WindowHeight,&WindowWidth);
	TextLen=StringWidth(Text);

	switch(Alignment)
	{
		case daAlignCenter:
			if(WindowWidth>TextLen)
				Column=(WindowWidth-TextLen)/2;
			break;
		case daAlignRight:
			if(WindowWidth>TextLen)
				Column=(WindowWidth-TextLen)-2;
			break;

		default:
			break;
	}
	SetArea(2,Row*(FontHeight+LineOffset)-1,127-2,((Row+1)*(FontHeight+LineOffset))-((Row==ROW6)?4:2),_OFF_);
	DrawString(Text,Column,Row*(FontHeight+LineOffset),_OFF_);
return 1;
}

int fs_DisplayAtRowOverWriteReverse(int Row,char *Text,daAlign Alignment)
{
	int Column=2;
	int FontHeight;
	int WindowHeight,WindowWidth;
	int TextLen;
	int LineOffset=3;

	FontHeight=GetPoliceSize();
	GetScreenSize(&WindowHeight,&WindowWidth);
	TextLen=StringWidth(Text);

	switch(Alignment)
	{
		case daAlignCenter:
			if(WindowWidth>TextLen)
				Column=(WindowWidth-TextLen)/2;
			break;
		case daAlignRight:
			if(WindowWidth>TextLen)
				Column=(WindowWidth-TextLen)-2;
			break;

		default:
			break;
	}
	SetArea(2,Row*(FontHeight+LineOffset)-1,127-2,((Row+1)*(FontHeight+LineOffset))-((Row==ROW6)?4:2),_OFF_);	/* Clear Area */
	SetArea(Column?Column-2:Column,Row*(FontHeight+LineOffset)-1,Column+TextLen+2,((Row+1)*(FontHeight+LineOffset))-((Row==ROW6)?4:2),_ON_);	/* Set Reverse */
	DrawString(Text,Column,Row*(FontHeight+LineOffset),_ON_);
return 1;
}

int fs_DisplayAtRowReverse(int Row,char *Text,daAlign Alignment)
{
	int Column=0;
	int FontHeight;
	int WindowHeight,WindowWidth;
	int TextLen;
	int LineOffset=3;

	FontHeight=GetPoliceSize();
	GetScreenSize(&WindowHeight,&WindowWidth);
	TextLen=StringWidth(Text);

	switch(Alignment)
	{
		case daAlignCenter:
			Column=(WindowWidth-TextLen)/2;
			break;
		case daAlignRight:
			Column=(WindowWidth-TextLen);
			break;

		default:
			break;
	}
	SetArea(0,Row*(FontHeight+LineOffset),127,((Row+1)*(FontHeight+LineOffset)),_OFF_);	/* Clear Area */
	SetArea(0,Row*(FontHeight+LineOffset),127,((Row)*(FontHeight+LineOffset)+FontHeight),_ON_);	/* Fill Reverse */
	DrawString(Text,Column,Row*(FontHeight+LineOffset)+1,_ON_);
return 1;
}

int fs_DisplayClearRow(int Row)
{
	int FontHeight;
	int WindowHeight,WindowWidth;
	int LineOffset=3;

	FontHeight=GetPoliceSize();
	GetScreenSize(&WindowHeight,&WindowWidth);

	SetArea(0,Row*(FontHeight+LineOffset),127,((Row+1)*(FontHeight+LineOffset)),_OFF_);	/* Clear Area */
return TRUE;
}

int fs_DisplayClearAll(void)
{
	int WindowHeight,WindowWidth;

	GetScreenSize(&WindowHeight,&WindowWidth);
	SetArea(0,0,WindowWidth,WindowHeight,_OFF_);	/* Clear Area */

return TRUE;
}

/** bool fs_DisplayClearWindowContents(void)
 *  This function is used to clear the current window contents except window title
 * @return : Returns TRUE always
 */

bool fs_DisplayClearWindowContents(void)
{
	int iRowNumber;

	for(iRowNumber=ROW2;iRowNumber<=ROW6;iRowNumber++)
	fs_DisplayAtRowOverWrite(iRowNumber," ",daAlignCenter);
return TRUE;
}
