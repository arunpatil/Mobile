#ifndef __CARD_H_
#define __CARD_H_


#define FS_CR_MAX_TRACK_DATA_LENGTH  100
#define FS_CR_VERSION_NUMBER_SZ      12  /* Size to create version string buffer */

typedef struct { /* Structure used to store card track data */
  bool IsTrack1Present;
  bool IsTrack2Present;
  bool IsTrack3Present;
  char Track1Data[FS_CR_MAX_TRACK_DATA_LENGTH+20+1]; /* Addition of 20 bytes for special */
                                                     /*   California DL */
  char Track2Data[FS_CR_MAX_TRACK_DATA_LENGTH+1];
  char Track3Data[FS_CR_MAX_TRACK_DATA_LENGTH+4+1];  /* ISO 4904 allows 104 characters */
} fs_CardTrackData;

typedef enum { /* PrtDoc result codes */
   crrcNone,               /* No result code has been generated */
   crrcErrCRAccess,        /* Error gaining access to card reader */
   crrcErrCRRelease,       /* Error releasing access to card reader */
   crrcErrCRInit,          /* Error initializing the card reader */
   crrcErrCRRead,          /* Error reading data to card reader */
   crrcErrCRReadBadFormat, /* Card data read but contained errors */
   crrcErrCRReadNoData,    /* No card data available to read */
   crrcErrCRFlush,         /* Unable to flush card reader */
   crrcBadParam,           /* An invalid parameter was passed to a CardReader function */
   crrcPlaceHolder
} ECardReaderResultCode;

/**************************************************************************************************
* --------------------- P U B L I C   F U N C T I O N   P R O T O T Y P E S --------------------- *
**************************************************************************************************/

bool fs_CardReaderInit(void);
bool fs_CardReaderClose(void);
bool fs_CardReaderIsDataWaiting(bool *DataWaiting);
bool fs_CardReaderGetTrackData(fs_CardTrackData *Data);
bool fs_CardReaderFlush(void);
bool fs_CardReaderGetLastError(ECardReaderResultCode *Result, char *Text, int BuffSize);


#endif
