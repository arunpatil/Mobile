#ifndef __COMMON_H_
#define __COMMON_H_

#define FS_MAX_COMMON_BUFFER 1000

typedef enum
{
ttSale,
ttRefund,
ttCashWithDrawal,
ttCashDeposit,
ttCashCollection,
ttCardActivate,
ttPreLoadPurchase,
ttPreLoadActivate,
ttCardTransfer,
ttBillPayment,
ttUtilityPayment,
ttBalanceEnquiry,
ttRateEnquiry,
ttCurrencyExchange,
ttMiniStatement,
ttPinExchange

}_TransactionType;


typedef struct
{
	/* Transaction Related Information */
	_TransactionType ttTransactionType;


	/* Common Buffers */
	char strCommonBuffer[FS_MAX_COMMON_BUFFER+1];
	int iCommonBufferLength;

}_glCommonData;

extern _glCommonData glCommonData;

/* Function Prototypes */
bool clearCommonDataAll(void);
bool clearCommonDataBuffer(void);
bool appendDataToCommonBuffer(char *data,int length);
bool readDataFromCommonBuffer(char *data,int *length);
bool popDataFromCommonBuffer(char *data,int length);


#endif
