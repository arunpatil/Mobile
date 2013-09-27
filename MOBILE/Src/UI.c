#include "SDK30.H"
#include "Display.h"
#include "ui.h"
#include <VGE_UIM.h>
#include "GL_GraphicLib.h"
#include "GL_Dialog.h"
#include "VGE_DBG_Traces.h"
#include "trace_sap.h"
#include "Parms.h"
#include "Utils.h"
#define SAP_TRACE_0  0xF000      // Initialize tracing with trace

static fs_UserInterfaceData glUInterfaceData;
static const unsigned char t_car[]           = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.";

bool fs_UIInit(void)
{

	glUInterfaceData.iTitleRow=ROW1;
	glUInterfaceData.iFirstNonTitleRow=ROW2;

	glUInterfaceData.iCommRow=ROW3;
	glUInterfaceData.iPromptRow1=ROW3;
	glUInterfaceData.iPromptRow2=ROW4;
	glUInterfaceData.iPromptRow3=ROW5;
	glUInterfaceData.iPromptRow4=ROW6;
	glUInterfaceData.iEntryRow=ROW6;
	glUInterfaceData.iLineCount=6;

return TRUE;
}

bool fs_UIClearNonTitlePrompts(void)
{
	int Row;

	for(Row=glUInterfaceData.iFirstNonTitleRow;Row<=glUInterfaceData.iLineCount;Row++)
		fs_DisplayClearRow(Row);

	return TRUE;
}

bool fs_UIDisplayCommPrompt(CONST char *Prompt)
{
	fs_DisplayAtRow(glUInterfaceData.iCommRow,(char*)Prompt,daAlignCenter);
return TRUE;
}

bool fs_UIDisplaySuccessPrompts(CONST char *Prompt1, CONST char *Prompt2,
                                CONST char *Prompt3, CONST char *Prompt4, int Timeout)
{
	fs_DisplayClearAll();
	fs_DisplayAtRowReverse(glUInterfaceData.iTitleRow,"SUCCESS!",daAlignCenter);
	fs_DisplayAtRow(glUInterfaceData.iPromptRow1,(char*)Prompt1,daAlignCenter);
	fs_DisplayAtRow(glUInterfaceData.iPromptRow2,(char*)Prompt2,daAlignCenter);
	fs_DisplayAtRow(glUInterfaceData.iPromptRow3,(char*)Prompt3,daAlignCenter);
	fs_DisplayAtRow(glUInterfaceData.iPromptRow4,(char*)Prompt4,daAlignCenter);
	ttestall(0,Timeout*100);
return TRUE;
}

bool fs_UIDisplayErrorPrompts(CONST char *Prompt1, CONST char *Prompt2,
                                CONST char *Prompt3, CONST char *Prompt4, int Timeout)
{
	fs_DisplayClearAll();
	fs_DisplayAtRowReverse(glUInterfaceData.iTitleRow,"ERROR!",daAlignCenter);
	fs_DisplayAtRow(glUInterfaceData.iPromptRow1,(char*)Prompt1,daAlignCenter);
	fs_DisplayAtRow(glUInterfaceData.iPromptRow2,(char*)Prompt2,daAlignCenter);
	fs_DisplayAtRow(glUInterfaceData.iPromptRow3,(char*)Prompt3,daAlignCenter);
	fs_DisplayAtRow(glUInterfaceData.iPromptRow4,(char*)Prompt4,daAlignCenter);
	ttestall(0,Timeout*100);

return TRUE;
}

bool fs_UIGetInput(char *Title,char *Prompt1,char *Prompt2,daAlign PromptAlign,entryType entry,daAlign EntryAlign, char *DataBuffer,
                        int MinLength, int MaxLength, int Timeout)
{
	int WindowHeight,WindowWidth;
	int MaxEntryWidth=0;
    TAB_ENTRY_ALPHA       AlphanumEntry;
	TAB_ENTRY_STRING       strAlphanumEntry;
    StructListe EntryWindow;
    ENTRY_BUFFER buf_entry;
    cGuiStructEntry NumEntry;
    char MaxStringBuffer[100]={0};
    int LoopCounter=0;
    char DisplayBuffer[100]={0};
    char DottedPrompt[100]={0};
	int LineOffset=3;
	int FontHeight;
	unsigned char RetVal;
	FILE *hDisplay;
	S_MONEY devise;
	BUFFER_SAISIE saisie;
	//TAB_ENTRY_FCT table;
	TAB_ENTRY_STRING table;
	//TAB_ENTRY_AMOUNT table;
	S_MONEY tab_devise[1];
	unsigned long montant;
	char AmountEntryBuffer[100]="123456.58";
	FILE *hKeyboard=NULL;
	UIM_t_input i_pxInput;
	S_FS_PARAM_CREATE ParamCreate;

	memset(DataBuffer,0,MaxLength);

	fs_DisplayInit();
	//fs_UIInit();

	memset(&EntryWindow,0,sizeof(StructListe));
	memset(&buf_entry,0,sizeof(ENTRY_BUFFER));
	memset(DottedPrompt,0,sizeof(DottedPrompt));

	FS_mount("/HOST",&ParamCreate.AccessMode);

    for(LoopCounter=MaxLength;LoopCounter;LoopCounter--)
    {
    	if(entry==entryTypeNumeric || entry==entryTypePassword || entry==entryTypeAmount )
    		strcat(MaxStringBuffer,"0");
    	else
    		strcat(MaxStringBuffer,"W");
    	strcat(DottedPrompt,"-");
    }

    if(entry==entryTypeAmount)
    	strcat(MaxStringBuffer,".");


	GetScreenSize(&WindowHeight,&WindowWidth);

	MaxEntryWidth=StringWidth(MaxStringBuffer);

	DrawWindowFont(0,0,WindowWidth-1,WindowHeight-1,1,"TITLE",_SMALL_);


     EntryWindow.Fenetre.fontsize=_SMALL_;
     EntryWindow.Fenetre.left=0;
     EntryWindow.Fenetre.rigth=WindowWidth-1;
     EntryWindow.Fenetre.top=0;
     EntryWindow.Fenetre.bottom=WindowHeight-1;
     EntryWindow.Fenetre.nblines=5;
     EntryWindow.Fenetre.correct=_ON_;
     EntryWindow.Fenetre.border=_ON_;
     EntryWindow.Fenetre.thickness=1;
     EntryWindow.Fenetre.type=_PROPORTIONNEL_;
     EntryWindow.Fenetre.police=0;
     EntryWindow.Fenetre.popup=_NOPOPUP_;
     EntryWindow.Fenetre.current=0;
     EntryWindow.Fenetre.first=0;
     EntryWindow.Fenetre.offset=0;
     EntryWindow.Fenetre.selected=0;
     EntryWindow.Fenetre.shortcommand=_OFF_;

/*
     EntryWindow.tab[0].message="TAB0";
     EntryWindow.tab[1].message="TAB1";
     EntryWindow.tab[2].message="TAB2";
     EntryWindow.tab[3].message="TAB3";
     EntryWindow.tab[4].message="TAB4";
*/
     EntryWindow.Fenetre.titre=Title;

     //EntryWindow.tab[0]                 = (byte*)"TAB0";                     // Text on first line
     EntryWindow.tab[1]                 =*DataBuffer?(byte*)DataBuffer:(byte*)DottedPrompt;                    // Initialise previous input with current font on second line
     //EntryWindow.tab[2]                 = (byte*)"TAB2";                    // Initialise previous input with current font on second line


/*
	fs_DisplayClearAll();
	sprintf(DisplayBuffer,"Height=%d",WindowHeight);
	fs_DisplayAtRow(ROW2,DisplayBuffer,daAlignCenter);
	sprintf(DisplayBuffer,"Width=%d",WindowWidth);
	fs_DisplayAtRow(ROW3,DisplayBuffer,daAlignCenter);
	sprintf(DisplayBuffer,"Entry Width=%d",MaxEntryWidth);
	fs_DisplayAtRow(ROW4,DisplayBuffer,daAlignCenter);
	ttestall(0,1*100);
*/

	if(entry==entryTypePassword)
		AlphanumEntry.mask     = 1;
	else
		AlphanumEntry.mask     = 0;

	AlphanumEntry.mask     = MASK_ANNULATION;

      AlphanumEntry.time_out = Timeout;
      FontHeight=GetPoliceSize();
/*
      gotoxy(0,0);
      clrscr();
      printf("FontHeight=\n%d",FontHeight);
      ttestall(0,2*200);
*/

      AlphanumEntry.line     = glUInterfaceData.iEntryRow*(FontHeight+LineOffset)-((glUInterfaceData.iEntryRow>(glUInterfaceData.iPromptRow2+1)) && (EntryWindow.Fenetre.border==_ON_)?EntryWindow.Fenetre.thickness:0);//glUInterfaceData.iEntryRow;
      LOG_PRINTF("Entryline=%d",AlphanumEntry.line);

      if(EntryAlign==daAlignRight)
    	  AlphanumEntry.column= WindowWidth-MaxEntryWidth-((EntryWindow.Fenetre.border==_ON_)?EntryWindow.Fenetre.thickness+2:0);
      else if(EntryAlign==daAlignCenter)
    	  AlphanumEntry.column= (WindowWidth-MaxEntryWidth)/2;
      else
    	  AlphanumEntry.column=2+((EntryWindow.Fenetre.border==_ON_)?EntryWindow.Fenetre.thickness:0);

      if(entry==entryTypePassword)
    	  AlphanumEntry.echo     = ECHO_SPECIAL;
      else
    	  AlphanumEntry.echo     = ECHO_NORMAL;
      AlphanumEntry.nb_max   = MaxLength ;
      AlphanumEntry.nb_min   = MinLength ;
      AlphanumEntry.tab_caracteres =(byte*) t_car;


      strAlphanumEntry.line     = glUInterfaceData.iEntryRow*(FontHeight+LineOffset)-((glUInterfaceData.iEntryRow>(glUInterfaceData.iPromptRow2+1)) && (EntryWindow.Fenetre.border==_ON_)?EntryWindow.Fenetre.thickness:0);//glUInterfaceData.iEntryRow;


       if(EntryAlign==daAlignRight)
     	  strAlphanumEntry.column= WindowWidth-MaxEntryWidth-((EntryWindow.Fenetre.border==_ON_)?EntryWindow.Fenetre.thickness+2:0);
       else if(EntryAlign==daAlignCenter)
     	  strAlphanumEntry.column= (WindowWidth-MaxEntryWidth)/2;
       else
     	  strAlphanumEntry.column=2+((EntryWindow.Fenetre.border==_ON_)?EntryWindow.Fenetre.thickness:0);

       if(entry==entryTypePassword)
     	  strAlphanumEntry.echo     = ECHO_SPECIAL;
       else
     	  strAlphanumEntry.echo     = ECHO_NORMAL;
       strAlphanumEntry.nb_max   = MaxLength ;
       strAlphanumEntry.nb_min   = MinLength ;
       strAlphanumEntry.mask     = 0;
       strAlphanumEntry.time_out = Timeout;

      devise.posdec=2;
      strcpy(devise.code,"824");
      strcpy(devise.nom,"USD");

      tab_devise[0].posdec=2;
      strncpy(tab_devise[0].code,"824",3);
      strncpy(tab_devise[0].nom,"USD",3);

      table.mask=(word)(MASK_F|MASK_NUM|MASK_CORR|MASK_POINT|MASK_VAL);
      //table.mask=0x0000;
      table.time_out=60;

      saisie.nombre=6;
      strcpy(saisie.donnees,"123456");





/*
       AlphanumEntry.mask     = 0;
       AlphanumEntry.time_out = 30;
       AlphanumEntry.line     = 1;
       AlphanumEntry.column   = 0;
       AlphanumEntry.echo     = ECHO_NORMAL;
       AlphanumEntry.nb_max   = 6 ;
       AlphanumEntry.nb_min   = 6 ;
       AlphanumEntry.tab_caracteres = t_car;
*/
      //fs_UIClearNonTitlePrompts();

      memset(&NumEntry,0,sizeof(cGuiStructEntry));
      NumEntry.echo='M';
      NumEntry.nb_char_max=10;
      NumEntry.nb_char_min=5;
      //G_Init(&EntryWindow,1,1);

      if(AlphanumEntry.column>127)
    	  AlphanumEntry.column=2;

      //glUInterfaceData.iEntryRow*(FontHeight+LineOffset)+1;
      //wG_Saisie_Alphanumerique( &EntryWindow,&AlphanumEntry);
     while(1)
     {

    	 //Alphanumerical_Entry(&AlphanumEntry);

    	 switch(entry)
    	 {
    	 	 case entryTypeAlphaNumeric:
    	 		 G_Alphanumerical_Entry(&EntryWindow,&AlphanumEntry);
    	 		 break;
    	 	 case entryTypeNumeric:
    	 	 case entryTypePassword:
    	 		 G_Numerical_Entry(&EntryWindow,&AlphanumEntry);
    	 		 break;

    	 	 case entryTypeAmount:
    	 		i_pxInput.cEcho=ECHO_NORMAL;
    	 		i_pxInput.cInputType=NUMERICAL;
    	 		i_pxInput.iColomne=15;
    	 		i_pxInput.iLine=10;
    	 		i_pxInput.iMaxInput=10;
    	 		i_pxInput.iMinInput=5;
    	 		i_pxInput.iTimeOut=Timeout;
    	 		//SetType(_NORMALE_);
    	 		//G_Init(&EntryWindow,1,1);
    	 		//hKeyboard=fopen("KEYBOARD","r");
    	 		//G_Amount_Entry(&devise,&saisie,AmountEntryBuffer,AmountEntryBuffer,&table,tab_devise,1, E_KEYBOARD,ANGLAIS);
    	 		//Stop_Entry();
    	 		//G_Amount_Entry(&devise,&saisie, DataBuffer,"LINE1",&table, tab_devise,1,ENTRY,0);
    	 		//G_Fct_Entry(&EntryWindow,&strAlphanumEntry);
    	 		//Saisie_Seuil(&table);
    	 		//Saisie_Montant(&table);
    	 		//GetAmount( 1, "INR",0,&montant,"LINE1", &table);
    	 		table.echo=ECHO_NORMAL;
    	 		table.nb_min=0;
    	 		table.nb_max=10;
    	 		table.column=10;
    	 		table.line=10;
    	 		EntryWindow.Fenetre.offset=2;
    	 		//G_Extended_entry(&EntryWindow,&table);
    	 		memset(AmountEntryBuffer,0,sizeof(AmountEntryBuffer));
    	 		fs_UIGetCustomInput(Title,Prompt1,Prompt2,PromptAlign,"********.**",EntryAlign,eDRightToLeft,'_',MinLength,AmountEntryBuffer,Timeout);
    	 		return TRUE;
    	 		 break;

    	 	 default:
    	 		 break;
    	 }
    	 //G_Init(&EntryWindow,glUInterfaceData.iEntryRow*(FontHeight+LineOffset)+1,AlphanumEntry.column);

    	 if(Prompt1 && *Prompt1)
         fs_DisplayAtRowOverWrite(glUInterfaceData.iPromptRow1,Prompt1,PromptAlign);
    	 if(Prompt2 && *Prompt2)
         fs_DisplayAtRowOverWrite(glUInterfaceData.iPromptRow2,Prompt2,PromptAlign);
    	 //InitSysSaisie();

    	 //cGUI_Alphanum_Entry(&EntryWindow,&AlphanumEntry,&NumEntry);
         //CGUI_Display("HELLO",NULL,!WAITING);
    	 //G_Extended_entry((StructListe *)&EntryWindow,&AlphanumEntry);

    	 //G_Init(&EntryWindow,1,1);



      //fs_DisplayClearRow(glUInterfaceData.iEntryRow);


      //wG_Init(&EntryWindow,AlphanumEntry.line*(FontHeight+LineOffset)+1,AlphanumEntry.column);
      //Alphanumerical_Entry (&AlphanumEntry);
      //gotoxy(AlphanumEntry.line*(FontHeight+LineOffset)+1,AlphanumEntry.column);


      	  RetVal=ttestall(ENTRY,Timeout*100);
/*
      	  gotoxy(0,0);
    	  clrscr();
    	  printf("ttestall=%d",RetVal);
    	  ttestall(0,1*100);
*/
/*
      	  fs_DisplayAtRow(ROW3,"ENTRY FINISHED",daAlignCenter);
      	  ttestall(0,3*100);
*/
    	  RetVal=Get_Entry(&buf_entry);

    	  if(hKeyboard)
    		  fclose(hKeyboard);

/*
    	  gotoxy(0,0);
    	  clrscr();
    	  printf("Entry=%d",RetVal);
    	  ttestall(0,1*100);
*/
    	  switch(RetVal)
    	  {
    	  	  case CR_ENTRY_OK:
    	  	  case CR_ENTRY_VALIDATED:

    	  		  strncpy(DataBuffer,buf_entry.d_entry,buf_entry.d_len);
    	  		  DataBuffer[buf_entry.d_len]='\0';
    	  		  return TRUE;

    	  	  case  CR_ENTRY_CANCEL:
    	  	  case CR_ENTRY_TIME_OUT:
    	  		  return FALSE;


    	  	  default:
    	  		  strncat(DataBuffer,buf_entry.d_entry,buf_entry.d_len);
    	  		  DataBuffer[buf_entry.d_len]='\0';
    	  		  break;
    	  }
      }


return FALSE;
}


unsigned char test_Alphanumerical_Entry(ENTRY_BUFFER *buf_entry)
{
  unsigned char     cr;
  int               prec;
  TAB_ENTRY_ALPHA       AlphanumEntry;
  FILE             *display;

  prec=StateHeader(0);
  display = fopen("DISPLAY","w");
  printf("ENTER VALUE\n");
  fclose(display);

  AlphanumEntry.mask     = 0;
  AlphanumEntry.time_out = 30;
  AlphanumEntry.line     = 1;
  AlphanumEntry.column   = 0;
  AlphanumEntry.echo     = ECHO_NORMAL;
  AlphanumEntry.nb_max   = 6 ;
  AlphanumEntry.nb_min   = 6 ;
  //AlphanumEntry.tab_caracteres = t_car;
  Alphanumerical_Entry (&AlphanumEntry);

  // Wait for the entry, the timeout from ttestall MUST BE 0. The timeout used is coming from Alphanumerical_Entry().
        // If ttestall exits on another event, the function Stop_Entry() must be used to stop the entry.
        // Example:
        //    if( ttestall(ENTRY | MSG,0) == MSG)
//    {
//      Stop_Entry();
//      return(0);
//    }
  ttestall(ENTRY,0);

  cr=Get_Entry(buf_entry);
  StateHeader(prec);
  return(cr);
}



int ShowEntry (const char *pcTitle, const char *pcText, char *pcInput, byte ucMinDigit, byte ucMaxDigit,
		       byte ucEntryMode)
{
	// Local variables
    // ***************
	StructList xMenu;                                                 // List definition
	int iMaxX, iMaxY;
	TAB_ENTRY_ALPHA xEntry;                                           // Entry definition
	ENTRY_BUFFER xBuf;                                                // Buffer entry
	byte ucInput;
	int iRet;

	// Entry management
	// ****************
	if (ucEntryMode == 3)                                             // Mode password, no initial input
		*pcInput=0;

	GetScreenSize(&iMaxY, &iMaxX);                                    // Get screen size
	memset(&xMenu, 0, sizeof(xMenu));                                 // Must clear the structure before initialisation
	xMenu.MyWindow.left          = 0;                                 // Windows
	xMenu.MyWindow.top           = 0;                                 //   position
	xMenu.MyWindow.rigth         = iMaxX-1;                           //     in
	xMenu.MyWindow.bottom        = iMaxY-1;                           //       pixels
	if (iMaxY == 128)
		xMenu.MyWindow.nblines   = 10;                                // Large screen
	else
		xMenu.MyWindow.nblines   = 5;                                 // Normal screen
	xMenu.MyWindow.fontsize		 = _MEDIUM_;
	xMenu.MyWindow.type			 = _PROPORTIONNEL_;                   // Font type
	xMenu.MyWindow.font			 = 0;                                 // Should be zero
	xMenu.MyWindow.correct		 = _ON_;                              // Process correction key
	xMenu.MyWindow.offset		 = 0;                                 // Table index for selection
    xMenu.MyWindow.shortcommand	 = _OFF_;                             // Shortcut
	xMenu.MyWindow.selected      = _OFF_;                             // Display a predefined item
	xMenu.MyWindow.thickness     = 1;                                 // Border thickness of the window entry
	xMenu.MyWindow.border		 = _ON_;                              // Display window entry border
	xMenu.MyWindow.popup		 = _NOPOPUP_;                         // Save an restore screen
	xMenu.MyWindow.first		 = 0;                                 // Pre select item if selected is _OFF_
	xMenu.MyWindow.current		 = 0;                                 // Pre select item if selected is _ON_
	xMenu.MyWindow.time_out		 = 60;                                // Time-out in seconde
	xMenu.MyWindow.title         = (byte*)pcTitle;                    // Window title with current font (no title is NULL)
	xMenu.tab[0]                 = (byte*)pcText;                     // Text on first line
	xMenu.tab[1]                 = (byte*)pcInput;                    // Initialise previous input with current font on second line

	// Entry Graphic Library definition
	// ================================
    memset (&xEntry, 0, sizeof(TAB_ENTRY_ALPHA));                     // Must clear the structure before initialisation
	xEntry.mask = 0x2000;                                             // Mask key
	xEntry.time_out = MINUTE;                                         // Timeout entry
	xEntry.line = 25;                                                 // Line where displaying
	xEntry.column = 5;                                                // Column where displaying
    if (ucEntryMode == 3)                                             // Mode password
    	xEntry.echo = ECHO_SPECIAL;                                   // - Echo on '*'
    else
    	xEntry.echo = ECHO_NORMAL;                                    // - Echo on characters
	xEntry.nb_max = ucMaxDigit;                                       // Max entry digit
	xEntry.nb_min = ucMinDigit;                                       // Min entry digit

	// Entry process
	// =============
    memset(&xBuf, 0, sizeof(ENTRY_BUFFER));
    switch (ucEntryMode)
    {
    case 0:                                                           // *** Extended Entry (GSM Style) ***
    	G_Extended_entry((void*)&xMenu, (TAB_ENTRY_STRING *)&xEntry);
    	break;
    case 1:                                                           // *** Alpha-Numerical Entry ***
    	G_Alphanumerical_Entry((void*)&xMenu, &xEntry);
    	break;
    case 2: case 3:                                                   // *** Numerical/Password Entry ***
    	G_Numerical_Entry((void*)&xMenu, (TAB_ENTRY_STRING *)&xEntry);
    	break;
    default:                                                          // *** Default => Extended Entry (GSM Style) ***
    	G_Extended_entry((void*)&xMenu, (TAB_ENTRY_STRING *)&xEntry);
    	break;
    }
	ttestall(ENTRY, 0);
	ucInput = Get_Entry(&xBuf);                                       // Read the Entry result

	switch (ucInput)
	{
	case CR_ENTRY_OK:                                                 // Valid with input
		xBuf.d_entry[xBuf.d_len]=0;                                   // Don't forget end of string
		strcpy(pcInput, (char*)xBuf.d_entry);
		iRet=2;
        break;
	case CR_ENTRY_VALIDATED:                                          // Valid without input
		xBuf.d_entry[xBuf.d_len]=0;                                   // Don't forget end of string
		strcpy(pcInput, (char*)xBuf.d_entry);
		iRet=1;
		break;
	case CR_ENTRY_CANCEL :                                            // Timeout/Cancel
	case CR_ENTRY_TIME_OUT :
		iRet=0;
		break;
	default:                                                          // Entry error
		iRet=-1;
		break;
	}

	return iRet;
}

#undef LOG_PRINTF

void LOG_PRINTF(char * frmt,...)
{


	//int flag=0;
	char sTemp[100]={0};
	char traceStr[1000]={0};
	char *p;
	int i;
	long long ll;
	unsigned u;
	double f;
	char *s;
	va_list argp;
	va_start(argp, frmt);
	p=frmt;

	clrscr();

	strcpy(traceStr,"");

	for(p=frmt; *p!='\0';p++)
	{
	 if(*p!='%')
	 {

		sprintf(sTemp,"%c",*p);
		strcat(traceStr,sTemp);
	  continue;
	 }

	p++;

	switch(*p)
	{
	case 'p': u=va_arg(argp, unsigned int);
				 sprintf(sTemp,"%p",(void*)u);
			strcat(traceStr,sTemp);
			break;
	case 'c' : i=va_arg(argp,int);
				sprintf(sTemp,"%c",i);
			strcat(traceStr,sTemp);
			break;

	case 'd' : i=va_arg(argp,int);
				sprintf(sTemp,"%d",i);
			strcat(traceStr,sTemp);
			break;

	case 'o': i=va_arg(argp,unsigned int);
				 sprintf(sTemp,"%o",i);
			strcat(traceStr,sTemp);
			break;
	case 's': s=va_arg(argp,char *);
			strcat(traceStr,s);
			 break;
	case 'u': u=va_arg(argp, unsigned int);
				 sprintf(sTemp,"%u",u);
			strcat(traceStr,sTemp);
			break;
	case 'f': f=va_arg(argp, double);
				 sprintf(sTemp,"%f",f);
			strcat(traceStr,sTemp);
			break;
	case 'x': u=va_arg(argp, unsigned int);
				 sprintf(sTemp,"0x%x",u);
					strcat(traceStr,sTemp);
					break;
	case 'l':ll=va_arg(argp,long int);
				sprintf(sTemp,"%ld",(long int)ll);
				strcat(traceStr,sTemp);
				 while(*p && *p!='d')
					 p++;

	break;

	case '%': strcat(traceStr,"%");break;

	}
	}
	va_end(argp);


	//trace((trace_sap_id_t) SAP_TRACE_0,strlen(traceStr),traceStr);
	trace((trace_sap_id_t) 0,strlen(traceStr),traceStr);

}

int fs_UIDisplayErrorScreen(char *Title,char *Prompt1,char *Prompt2,char *Prompt3)
{
	int Error;
	Error|=fs_DisplayClearAll();
	Error|=fs_DisplayAtRowReverse(glUInterfaceData.iTitleRow,Title,daAlignCenter) &&
	fs_DisplayAtRow(glUInterfaceData.iPromptRow1,Prompt1,daAlignCenter) &&
	fs_DisplayAtRow(glUInterfaceData.iPromptRow2,Prompt2,daAlignCenter) &&
	fs_DisplayAtRow(glUInterfaceData.iPromptRow3,Prompt3,daAlignCenter) ;

	if(!Error)
		return FALSE;

	ttestall(0,300);

return TRUE;
}

bool fs_UIBuzzerPrompt(void)
{
	if(glParameters.PromptBeep)
	{
		buzzer(10);
		ttestall(0,10);
		buzzer(10);
	}

	return TRUE;
}

bool fs_UIBeepNavigateError(void)
{
	if(glParameters.PromptBeep)
	{
		buzzer(5);
		ttestall(0,5);
		buzzer(5);
	}
	return TRUE;
}

int fs_UIGetChar(int PromptTimeout)
{
	   int nKey=0;
	   unsigned int nEvent;
	   FILE *hKeyboard;

	   hKeyboard=fopen("KEYBOARD","r");
	   //LOG_PRINTF("Entered fs_UIGetChar()");
       nEvent = ttestall(KEYBOARD, PromptTimeout * 100);
           if (nEvent == KEYBOARD)
            {
               // Keyboard event
               nKey = getchar(); // get the pressed key
            }
           else
        	   nKey=0;
          fclose(hKeyboard);
           //LOG_PRINTF("fs_UIGetChar=%d",nKey);
return nKey;
}

bool fs_UISetPromptRow(int Row)
{
	glUInterfaceData.iPromptRow1=Row++;
	glUInterfaceData.iPromptRow2=Row++;
	glUInterfaceData.iPromptRow3=Row++;
	glUInterfaceData.iPromptRow4=Row;

return TRUE;
}

bool fs_UISetEntryRow(int Row)
{
	glUInterfaceData.iEntryRow=Row;
return TRUE;
}

bool fs_UIResetParameters(void)
{

return fs_UIInit();
}


/**
 * bool fs_UIGetCustomInput(char *Title,char *Prompt1,char *Prompt2,daAlign PromptAlign,char *Pattern,daAlign EntryAlign,entryDirection EntryDir,char DefaultChar,int Min,char *DataBuffer,int Timeout)
 * @param Title: Not used so far
 * @param Prompt1: Display Prompt 1
 * @param Prompt2: Display Prompt 2
 * @param PromptAlign: Prompt Alignment
 * @param Pattern: Pattern to get input. '*' is used to denote digit at that place
 * @param EntryAlign: Entry Alignment
 * @param EntryDir: Direction of the entry. Either from left to right or vice versa
 * @param DefaultChar: Default char waiting for input
 * @param Min: Minimum length of the input
 * @param DataBuffer: Output buffer
 * @param Timeout: Idle Timeout
 * @return : returns TRUE if entry successful else FALSE
 */

bool fs_UIGetCustomInput(char *Title,char *Prompt1,char *Prompt2,daAlign PromptAlign,char *Pattern,daAlign EntryAlign,entryDirection EntryDir,char DefaultChar,int Min,char *DataBuffer,int Timeout)
{
	char strBuffer[21+1]={0};
	char strKeyBuffer[21+1]={0};
	int iKeyCount=0;
	int iKeyCountMax=0;
	int iKey;
	int iRight,iBottom;
	int iCharWidth;

	GetScreenSize(&iBottom,&iRight);
	DrawWindow(0,0,iRight-1,iBottom-1,1,(unsigned char *)Title);

	/* Get the number of occurrences of '*' in Pattern */
	iKeyCountMax=fs_UINumberOfOccurences(Pattern,'*');

	iCharWidth=GetType();	/* Get the original character width */

	do{
		LABEL_STARTOVER:
		SetType(iCharWidth);
		fs_DisplayAtRowOverWrite(glUInterfaceData.iPromptRow1,Prompt1,PromptAlign);
		fs_DisplayAtRowOverWrite(glUInterfaceData.iPromptRow2,Prompt2,PromptAlign);

		/* Set fixed width for amount */
		SetType(_FIXED_WIDTH_);
		fs_UIFormatDataWithPattern(strKeyBuffer,Pattern,strBuffer,EntryDir,DefaultChar,NULL);
		fs_DisplayAtRowOverWrite(glUInterfaceData.iEntryRow,strBuffer,EntryAlign);
		iKey=fs_UIGetChar(Timeout);

		if((iKey>='0' && iKey<='9') || (iKey==T_CORR))
		{

			switch(iKey)
			{
				case T_CORR:
					if(iKeyCount>0)
						iKeyCount--;
					strKeyBuffer[iKeyCount]='\0';
					break;

				default:
					if(iKeyCount<iKeyCountMax)
						strKeyBuffer[iKeyCount++]=iKey;
					break;
			}
		}
		/* Continue until input length is less than minimum length */
		if((iKeyCount<Min) && iKey==T_VAL)
			goto LABEL_STARTOVER;

	}while(iKey!=FALSE && iKey!=T_ANN && iKey!=T_VAL);

	SetType(iCharWidth);	/* Set the Original character width */

	if(iKey==T_VAL)
	{
		/* Copy the input data to the desired address */
		strcpy(DataBuffer,strKeyBuffer);
		return TRUE;
	}

return FALSE;
}

/**
 * bool fs_UIGetCustomAmountEntry(char *Title,char *Prompt1,char *Prompt2,daAlign PromptAlign,char *Currency,int Decimal,daAlign EntryAlign,int Min,int Max,double *Amount,int *ipSeparatorList,int Timeout)
 * It performs the amount entry function with customized prompts & format
 * @param Title: Title of the window
 * @param Prompt1: Display Prompt 1
 * @param Prompt2: Display Prompt 2
 * @param PromptAlign: Alignment of the DDisplay Prompts
 * @param Currency: Currency in string format
 * @param Decimal: Number of decimal points
 * @param EntryAlign: Alignment of Entry
 * @param Min: Minimum digits to be entered
 * @param Max: Max entry digits allowed
 * @param Amount: Pointer to store the entered amount
 * @param ipSeparatorList: Separators required to be placed in amount entry. Can be NULL if not required.
 * 		  The list should be an integer array with separator location.
 * 		  Must be NULL terminated at the end
 * @param Timeout: Entry timeout
 * @return: Returns TRUE if entry validated, FALSE otherwise
 */
bool fs_UIGetCustomAmountEntry(char *Title,char *Prompt1,char *Prompt2,daAlign PromptAlign,char *Currency,int Decimal,daAlign EntryAlign,int Min,int Max,double *Amount,int *ipSeparatorList,int Timeout,char *strKeyBuffer)
{
	char strBuffer[21+1]={0};
	//char strKeyBuffer[21+1]={0};
	char strPromptBuffer[21+1]={0};
	int iKeyCount=0;
	int iKeyCountMax=0;
	int iKey;
	int iCharWidth;
	char Pattern[21+1]={0};
	int iLocalVar,iLocalVarAux;
	int iRight,iBottom;
	long long lAmount=0;
	double fAmount=0;
	entryDirection EntryDir=eDRightToLeft;
	bool bRedrawWindow=FALSE;

	/* Draw window with title */
	GetScreenSize(&iBottom,&iRight);
	DrawWindow(0,0,iRight-1,iBottom-1,1,(unsigned char *)Title);

	fAmount=*Amount;

	/* Develop the pattern based on amount entry parameters */
	for(iLocalVar=0;iLocalVar<(Max-Decimal);iLocalVar++)
		strcat(Pattern,"*");
	if(Decimal)
	{
		strcat(Pattern,".");	/* Decimal Point */
		for(iLocalVar=0;iLocalVar<Decimal;iLocalVar++)
		{
			fAmount*=10.0;
			strcat(Pattern,"*");
		}
	}
	/*
	strcat(Pattern," ");
	strcat(Pattern,Currency);
	*/

	LOG_PRINTF("Pattern=%s",Pattern);
	LOG_PRINTF("sizeof(long)=%d",sizeof(long));
	LOG_PRINTF("sizeof(long long)=%d",sizeof(long long));
	lAmount=(long long)fAmount;

	/* Get the number of occurrences of '*' in Pattern */
	iKeyCountMax=fs_UINumberOfOccurences(Pattern,'*');

	iCharWidth=GetType();	/* Get the original character width */


	do{
		LABEL_STARTOVER:
		//SetType(iCharWidth);
		SetType(_NORMALE_);
		fs_DisplayAtRowOverWrite(glUInterfaceData.iPromptRow1,Prompt1,PromptAlign);
		fs_DisplayAtRowOverWrite(glUInterfaceData.iPromptRow2,Prompt2,PromptAlign);

		fs_UISetEntryRow(glUInterfaceData.iPromptRow2+1);
		fs_DisplayAtRowOverWriteReverse(glUInterfaceData.iPromptRow4,Currency,daAlignRight);
		//sprintf(strPromptBuffer,"%0*lld",Decimal+1,lAmount);	/* To display Amount in the form of 0.01 */
		memset(strPromptBuffer,0,sizeof(strPromptBuffer));
		for(iLocalVar=0,iLocalVarAux=0;((iLocalVar<iKeyCount) || (iLocalVar<(Decimal+1)));iLocalVar++)
			strPromptBuffer[iLocalVar]=((iLocalVar+iKeyCount)<(Decimal+1))?'0':strKeyBuffer[iLocalVarAux++];
		memset(strBuffer,0,sizeof(strBuffer));
		fs_UIFormatDataWithPattern(strPromptBuffer,Pattern,strBuffer,EntryDir,' ',ipSeparatorList);
		fs_DisplayAtRowOverWrite(glUInterfaceData.iEntryRow,strBuffer,EntryAlign);
		if(StringWidth(strBuffer)>=iRight)
			bRedrawWindow=TRUE;
		iKey=fs_UIGetChar(Timeout);

		if((iKey>='0' && iKey<='9') || (iKey==T_CORR))
		{

			switch(iKey)
			{
				case T_CORR:
					if(iKeyCount>0)
						iKeyCount--;
					strKeyBuffer[iKeyCount]='\0';
					break;

				default:
					/* Check if 0 is pressed before any other from 1-9 */
					if((iKeyCount==0) && iKey=='0')
						break;
					/* Store only if less than max entry digits */
					if(iKeyCount<iKeyCountMax)
						strKeyBuffer[iKeyCount++]=iKey;
					break;
			}
			fs_UtilTimerSleep(10);
			lAmount=(long long)atoll(strKeyBuffer);
			fs_UtilTimerSleep(10);
		}

		/* Redraw window if it gets corrupted due to over length */
		if(bRedrawWindow==TRUE)
		{
			DrawWindow(0,0,iRight-1,iBottom-1,1,(unsigned char *)Title);
			bRedrawWindow=FALSE;
		}


		/* If user enters green key without entering any key then accept default */
		if((iKeyCount==0) && (iKey==T_VAL) && (lAmount>0))
			return TRUE;
		/* Continue until input value is zero */
		else if((iKey==T_VAL) && Min && (lAmount==0))
			goto LABEL_STARTOVER;
		/* Continue until input length is less than minimum length */
		else if((iKeyCount<Min) && iKey==T_VAL)
			goto LABEL_STARTOVER;


	}while(iKey!=FALSE && iKey!=T_ANN && iKey!=T_VAL);

	SetType(iCharWidth);	/* Set the Original character width */

	if(iKey==T_VAL)
	{
		/* Re convert to the double with specified decimal points */
		fAmount=(double)atof(strKeyBuffer);
		for(iLocalVar=0;iLocalVar<Decimal;iLocalVar++)
			fAmount/=10.0;
		*Amount=fAmount;
		return TRUE;
	}

return FALSE;
}


/**
 * bool fs_FormatDataWithPattern(char *Data,char *Pattern,char *FormattedData)
 * This function is used to format the data as per the pattern provided.
 * '*' in pattern indicates digit
 * @param Data: Data buffer
 * @param Pattern: Pattern
 * @param FormattedData: Pointer to store the formatted data
 * @param DefaultChar: Default value in data if data length is not sufficient
 * @param entryDirection: Direction of entry. Either from left to right or from right to left
 * @return: Returns TRUE if the complete pattern is formatted else FALSE
 */
bool fs_UIFormatDataWithPattern(char *Data,char *Pattern,char *FormattedData,entryDirection EntryDir, char DefaultChar, int *SeparatorList)
{

	int iCharData=0;
	int iCharPattern=0;
	int iCharFormatted=0;
	int iDigitCountPattern,iDigitCountData,iDigitCountFormatted=0;
	int iSeparatorsAdded=0;
	int iSeparatorAddedAtCount=0;


	/* Get the number of digits in pattern & data*/
	iDigitCountPattern=fs_UINumberOfOccurences(Pattern,'*');
	iDigitCountData=strlen(Data);

	/* Format the the data as per the pattern until any of the 'Data' or 'Pattern' is empty */
	while(Pattern && FormattedData && Pattern[iCharPattern])
	{
		/* Check if entry direction is from right to left */
		if((iSeparatorAddedAtCount!=iDigitCountFormatted) && EntryDir==eDRightToLeft && SeparatorList && Pattern[iCharPattern]=='*' &&
				(iDigitCountFormatted>iDigitCountPattern-iDigitCountData) && isValuePresentInList(SeparatorList,iDigitCountPattern-iDigitCountFormatted))
		{
			FormattedData[iCharFormatted++]=',';	/* Add separators at position defined by SeparatorList */
			iSeparatorsAdded++;
			iCharPattern--;	/* Compensation to increment after if else conditions */
			iSeparatorAddedAtCount=iDigitCountFormatted;
		}
		else if(EntryDir==eDRightToLeft && Pattern[iCharPattern]=='*' &&
			(iDigitCountFormatted<(iDigitCountPattern-iDigitCountData)))
		{
			FormattedData[iCharFormatted++]=DefaultChar;
			iDigitCountFormatted++;
		}
		else if(Pattern[iCharPattern]=='*' && Data &&  Data[iCharData])
		{
			FormattedData[iCharFormatted++]=Data[iCharData++];
			iDigitCountFormatted++;
		}
		else if(Pattern[iCharPattern]=='*')
		{
			FormattedData[iCharFormatted++]=DefaultChar;
			iDigitCountFormatted++;
		}
		else
			FormattedData[iCharFormatted++]=Pattern[iCharPattern];
		iCharPattern++;

		LOG_PRINTF("FormattedData=%s",FormattedData);
	}

	/* The parsing ended before complete formatting. i.e. Data bytes insufficient */
	if(Pattern[iCharPattern])
	return FALSE;

return TRUE;
}

/**
 * int fs_UINumberOfOccurences(char *Buffer,char Character)
 * Returns the number of occurrences of the 'Character' in 'Buffer'
 * @param Buffer: Buffer string
 * @param Character: Character to search for
 * @return: Returns the number of occurrences
 */

int fs_UINumberOfOccurences(char *Buffer,char Character)
{
	char *pDataPointer;
	int iCount=0;

	/* Count number of occurrences of '*' */
	pDataPointer=Buffer;
	while((pDataPointer=strchr(pDataPointer,Character)))
	{
		/* Start searching from next byte */
		pDataPointer++;
		iCount++;
	}

return iCount;
}
