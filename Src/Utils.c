#include "SDK30.H"
#include "Display.h"
#include "ui.h"
#include "VGE_UIM.h"
#include "GL_GraphicLib.h"
#include "GL_Dialog.h"
#include "VGE_DBG_Traces.h"
#include "trace_sap.h"
#include "Parms.h"
#include "Utils.h"
#include "time.h"
#include "rtctime.h"



/**
 * bool fs_TimerSleep(long MS)
 * Inserts delay of specifies milliseconds
 * @param milliSeconds: Number of milliseconds to delay
 * @return :  Returns TRUE always
 */

bool fs_UtilTimerSleep(long milliSeconds)
{
	/* If arguement to ttestall are 0,0 then application will block forever */
	if(milliSeconds>10)
	{
		ttestall(0,milliSeconds/10);
	}
	return TRUE;
}

/**
 * bool fs_ClockGetString(char *buffer)
 * This function gets the current date time in DDMMYYHHMMSSwyyy format
 * @param buffer: The address where the string will be stored.
 * @return Returns TRUE if successful else FALSE
 */

bool fs_ClockGetString(char *buffer)
{
	struct tm tmLocalTime;
	DATE dtDate;

	/* Read date from terminal */
	if(read_date(&dtDate)!=0)
		return FALSE;

	/* Get the integer equivalents in tm structure */
	memset(&tmLocalTime,0,sizeof(tmLocalTime));
	if(!fs_ConvertTODStringToInts((char *)&dtDate,&tmLocalTime.tm_year,&tmLocalTime.tm_mon,&tmLocalTime.tm_mday,
			&tmLocalTime.tm_hour,&tmLocalTime.tm_min,&tmLocalTime.tm_sec,NULL,NULL))
		return FALSE;

	tmLocalTime.tm_isdst=0;	/* No information */
	tmLocalTime.tm_mon-=1;	/* Month Adjust From 0-11 */

	/* To get weekday & day of year */
	mktime(&tmLocalTime);

	/* Print to the buffer */
	sprintf(buffer,"%02d%02d%02d%02d%02d%02d%d%03d",
		tmLocalTime.tm_mday,tmLocalTime.tm_mon+1,tmLocalTime.tm_year%100,
		tmLocalTime.tm_hour,tmLocalTime.tm_min,tmLocalTime.tm_sec,tmLocalTime.tm_wday,
		tmLocalTime.tm_yday	);

	LOG_PRINTF("fs_ClockGetString()|time=%s",buffer);
	return TRUE;
}

/**
 * bool fs_ConvertTODStringToInts(char *DateTimeBuffer, int *Year, int *Month, int *Day, int *Hour, int *Minute, int *Second, int *Week, int *DayOfYear)
 * Used to convert date time string buffer to integer variables
 * @param DateTimeBuffer: Buffer containing date time in DDMMYYHHMMSSwyyy
 * @param Year : Integer Pointer to Year Variable 0-99
 * @param Month : Integer Pointer to Month Variable 1-12
 * @param Day : Integer Pointer to Day Variable 01-31
 * @param Hour : Integer Pointer to Hour Variable 00-23
 * @param Minute : Integer Pointer to Minute Variable 00-59
 * @param Second : Integer Pointer to Second Variable 00-59
 * @param Week : Integer Pointer to Weekday 0-6
 * @param DayOfYear : Integer Pointer to Day of the year 0-365
 * @return : Returns TRUE if successfully converted else FALSE
 */

bool fs_ConvertTODStringToInts(char *DateTimeBuffer, int *Year, int *Month, int *Day, int *Hour, int *Minute, int *Second, int *Week, int *DayOfYear)
{
	char strBuffer[2+1]={0};
	char *pDateTimeBuffer;

	pDateTimeBuffer=DateTimeBuffer;

	/* Convert available data to integer format */
	/* Day */
	memset(strBuffer,0,sizeof(strBuffer));
	strncpy(strBuffer,pDateTimeBuffer,2);
	if(Day)
		*Day=atoi(strBuffer);
	pDateTimeBuffer+=2;

	/* Month */
	memset(strBuffer,0,sizeof(strBuffer));
	strncpy(strBuffer,pDateTimeBuffer,2);
	if(Month)
		*Month=atoi(strBuffer);
	pDateTimeBuffer+=2;

	/* Year */
	memset(strBuffer,0,sizeof(strBuffer));
	strncpy(strBuffer,pDateTimeBuffer,2);
	if(Year)
		*Year=atoi(strBuffer);
	pDateTimeBuffer+=2;

	/* Hour */
	memset(strBuffer,0,sizeof(strBuffer));
	strncpy(strBuffer,pDateTimeBuffer,2);
	if(Hour)
		*Hour=atoi(strBuffer);
	pDateTimeBuffer+=2;

	/* Minute */
	memset(strBuffer,0,sizeof(strBuffer));
	strncpy(strBuffer,pDateTimeBuffer,2);
	if(Minute)
		*Minute=atoi(strBuffer);
	pDateTimeBuffer+=2;

	/* Second */
	memset(strBuffer,0,sizeof(strBuffer));
	strncpy(strBuffer,pDateTimeBuffer,2);
	if(Second)
		*Second=atoi(strBuffer);
	pDateTimeBuffer+=2;

	/* Weekday */
	memset(strBuffer,0,sizeof(strBuffer));
	strncpy(strBuffer,pDateTimeBuffer,1);
	if(Week)
		*Week=atoi(strBuffer);
	pDateTimeBuffer+=1;

	/* day of the year */
	memset(strBuffer,0,sizeof(strBuffer));
	strncpy(strBuffer,pDateTimeBuffer,3);
	if(DayOfYear)
		*DayOfYear=atoi(strBuffer);

return TRUE;
}

/**
 * bool fs_CheckTimeElapsed(char *ReferenceDateTimeBuffer)
 * This function checks whether the reference date time has been elapsed or not
 * @param ReferenceDateTimeBuffer: Reference Date Time in DDMMYYHHMMSSwyyy format
 * @return: Returns TRUE if current date time is past the reference date time
 */

bool fs_CheckTimeElapsed(char *ReferenceDateTimeBuffer)
{
	char strTimeBuffer[FS_DATE_TIME_LEN_MAX+1]={0};
	int iYear,iMonth,iDay,iHour,iMinute,iSecond;
	int iYearRef,iMonthRef,iDayRef,iHourRef,iMinuteRef,iSecondRef;
	bool bResult=FALSE;

	if(
			/* Convert to integer equivalents */
			fs_ClockGetString(strTimeBuffer) &&
			fs_ConvertTODStringToInts(strTimeBuffer,&iYear,&iMonth,&iDay,
					&iHour,&iMinute,&iSecond,NULL,NULL) &&
			fs_ConvertTODStringToInts(ReferenceDateTimeBuffer,&iYearRef,&iMonthRef,&iDayRef,
					&iHourRef,&iMinuteRef,&iSecondRef,NULL,NULL)
	  )
	{
		/* Set flag if any of the condition is true */
		bResult= (iYear>iYearRef) ||
				 ((iYear==iYearRef) && (iMonth>iMonthRef)) ||
				 ((iYear==iYearRef) && (iMonth==iMonthRef) && (iDay>iDayRef))||
				 ((iYear==iYearRef) && (iMonth==iMonthRef) && (iDay==iDayRef) && (iHour>iHourRef))||
				 ((iYear==iYearRef) && (iMonth==iMonthRef) && (iDay==iDayRef) && (iHour==iHourRef) && (iMinute>iMinuteRef))||
				 ((iYear==iYearRef) && (iMonth==iMonthRef) && (iDay==iDayRef) && (iHour==iHourRef) && (iMinute==iMinuteRef) && (iSecond>iSecondRef));
	}

return bResult;
}

/**
 * bool fs_GetNextDateTime(long Offset,char *ReferenceDateTime,char *PostedDateTime)
 * Used to get the next date time with offset in seconds
 * @param Offset: Offset in seconds
 * @param ReferenceDateTime: Reference date time
 * @param PostedDateTime: Offset date time
 * @return: Returns TRUE if successful else FALSE
 */
bool fs_GetNextDateTime(long Offset,char *ReferenceDateTime,char *PostedDateTime)
{
	struct tm tmRefTime;
	struct tm tmPostedTime;
	time_t referenceTime;
	char strBuffer[100]={0};

	memset(&tmRefTime,0,sizeof(struct tm));

	/* Convert the structure to integer equivalents */
	if(!fs_ConvertTODStringToInts(ReferenceDateTime,&tmRefTime.tm_year,&tmRefTime.tm_mon,&tmRefTime.tm_mday,&tmRefTime.tm_hour,
			&tmRefTime.tm_min,&tmRefTime.tm_sec,&tmRefTime.tm_wday,&tmRefTime.tm_yday))
		return FALSE;

	/* No information available for DST */
	tmRefTime.tm_isdst=0;
	tmRefTime.tm_mon-=1;

	/* Copy data only of the size which fits in the structure */
	referenceTime=mktime(&tmRefTime);
	referenceTime+=(time_t)Offset;
	tmPostedTime=*gmtime(&referenceTime);

	mktime(&tmPostedTime);	/* To Adjust the day of the year & weekday */

	sprintf(strBuffer,"%02d%02d%02d%02d%02d%02d%d%03d",tmPostedTime.tm_mday,
			tmPostedTime.tm_mon+1,tmPostedTime.tm_year%100,tmPostedTime.tm_hour,
			tmPostedTime.tm_min,tmPostedTime.tm_sec,tmPostedTime.tm_wday,
			tmPostedTime.tm_yday);

	/* Copy data only of the size which fits in the structure */
	memcpy(PostedDateTime,strBuffer,FS_DATE_TIME_LEN_MAX);
	return TRUE;
}


/**
 * int fs_GetDayOfYear(char *DDMMYY)
 * This function returns the day of the year
 * @param DDMMYY: String which points to the date in DDMMYY format
 * @return: Returns the day of the year of the provided date
 */

int fs_GetDayOfYear(char *DDMMYY)
{
	int iYear,iDay,iMonth;
	char strBuffer[2+1]={0};
	int iDayOfYear=0;

	/* Convert string to integer equivalents */
	memset(strBuffer,0,sizeof(strBuffer));
	sprintf(strBuffer,"%2.2s",DDMMYY);
	iDay=atoi(strBuffer);

	memset(strBuffer,0,sizeof(strBuffer));
	sprintf(strBuffer,"%2.2s",DDMMYY+2);
	iMonth=atoi(strBuffer);

	memset(strBuffer,0,sizeof(strBuffer));
	sprintf(strBuffer,"%2.2s",DDMMYY+4);
	iYear=atoi(strBuffer);

	while(iMonth--)	/* Get the total for the previous months. Don't consider the current month */
	{
		if(	iMonth==1 ||
			iMonth==3 ||
			iMonth==5 ||
			iMonth==7 ||
			iMonth==8 ||
			iMonth==10 ||
			iMonth==12
		)
			iDayOfYear+=31;
		else if(
			iMonth==4 ||
			iMonth==6 ||
			iMonth==9 ||
			iMonth==11
		)
			iDayOfYear+=30;
		else if(iMonth==2 && ((iYear%4)==0))
			iDayOfYear+=29;
		else if(iMonth==2)
			iDayOfYear+=28;
	}

	/* Now add the days of the current month */
	iDayOfYear+=iDay;

return iDayOfYear;

}

/**
 * bool fs_GetTimeDifference(char *DateTime,char *ReferenceDateTime,char *Days,char *Hours,char *Minutes)
 * This function returns the time difference in days, hours & minutes
 * @param DateTime: Future date time
 * @param ReferenceDateTime: Reference date time. Can be NULL. If NULL then
 * 		  current date time will be taken as a reference.
 * @param Days: pointer to the days variable. Can be NULL
 * @param Hours:pointer to the Hours variable. Can be NULL
 * @param Minutes:pointer to the Minutes variable.
 * @return: Returns TRUE if successful else FALSE
 */


bool fs_GetTimeDifference(char *DateTime,char *ReferenceDateTime,long *Days,long *Hours,long *Minutes)
{
	struct tm tmTime;
	struct tm tmReferenceTime;
	time_t tTime;
	time_t tReferenceTime;
	char strBuffer[FS_DATE_TIME_LEN_MAX+1]={0};
	bool bClockAsReference=FALSE;

	/* Get current time as reference if no reference date provided */
	if(!ReferenceDateTime)
	{
		if(!fs_ClockGetString(strBuffer))
			return FALSE;
		bClockAsReference=TRUE;
	}
	else {
		strcpy(strBuffer,ReferenceDateTime);
	}

	memset(&tmTime,0,sizeof(struct tm));
	memset(&tmReferenceTime,0,sizeof(struct tm));
	if(	fs_ConvertTODStringToInts(DateTime,&tmTime.tm_year,&tmTime.tm_mon,&tmTime.tm_mday,
			&tmTime.tm_hour,&tmTime.tm_min,&tmTime.tm_sec,&tmTime.tm_wday,&tmTime.tm_yday) &&
		fs_ConvertTODStringToInts(strBuffer,&tmReferenceTime.tm_year,&tmReferenceTime.tm_mon,&tmReferenceTime.tm_mday,
						&tmReferenceTime.tm_hour,&tmReferenceTime.tm_min,&tmReferenceTime.tm_sec,&tmReferenceTime.tm_wday,&tmReferenceTime.tm_yday) )
	{

		/* Adjust Month between 0-11 only */
		tmTime.tm_mon-=1;
		tmReferenceTime.tm_mon-=1;

		tTime=mktime(&tmTime);
		tReferenceTime=mktime(&tmReferenceTime);

		/* check for errors in library functions */
		if(tTime==-1 || tReferenceTime==-1)
			return FALSE;

		*Minutes=(long)((difftime(tTime,tReferenceTime))/60)+1;	/* In case of seconds */

		/* Split into hours or days if required */
		if(Hours)
		{
			*Hours=*Minutes/60;
			*Minutes=*Minutes%60;
			if(Days)
			{
				*Days=*Hours/24;
				*Hours=*Hours%24;
			}
		}
		return TRUE;
	}

return FALSE;
}

/**
 * bool isValuePresentInList(int *List,int Value)
 * This function returns whether the 'Value' is present in the integer array.
 * The value should be non zero & the array list should be null terminated
 * @param List: Integer array
 * @param Value: Value to be found out
 * @return: TRUE if value present in the array else FALSE
 */
bool isValuePresentInList(int *List,int Value)
{
	int iLocalVar;

	for(iLocalVar=0;List && List[iLocalVar];iLocalVar++)
	{
		/* If array value matches the specified value then return TRUE */
		if(List[iLocalVar]==Value)
			return TRUE;
	}
return FALSE;
}
