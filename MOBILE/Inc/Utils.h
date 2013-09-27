#ifndef __UTILS_H
#define __UTILS_H

#include "sdk30.h"

#ifndef FS_DATE_TIME_LEN_MAX
#define FS_DATE_TIME_LEN_MAX 16 /* DDMMYYHHMMSSwyyy 	w=weekday, yyy=day of the year */
#endif

#define FS_OFFSET_1_HOUR 3600
#define FS_OFFSET_24_HOURS 24*3600

bool fs_UtilTimerSleep(long milliSeconds);
bool fs_ClockGetString(char *buffer);
bool fs_ConvertTODStringToInts(char *DateTimeBuffer, int *Year, int *Month, int *Day, int *Hour, int *Minute, int *Second, int *Week, int *DayOfYear);
bool fs_GetNextDateTime(long Offset,char *ReferenceDateTime,char *PostedDateTime);
int fs_GetDayOfYear(char *DDMMYY);
bool fs_GetTimeDifference(char *DateTime,char *ReferenceDateTime,long *Days,long *Hours,long *Minutes);
bool fs_CheckTimeElapsed(char *ReferenceDateTimeBuffer);

bool isValuePresentInList(int *List,int Value);


#endif
