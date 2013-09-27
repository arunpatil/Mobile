#include "SDK30.H"
#include "Menu.h"
#include "Display.h"
#include "fsparms.h"
#include "parms.h"
#include "ui.h"
#include "Printer.h"
#include "Utils.h"
#include "Common.h"

_glCommonData glCommonData;


/**
 * bool clearCommonDataAll(void)
 * This function clears all the common data stored in glCommonData variable
 * @return: Returns TRUE always
 */
bool clearCommonDataAll(void)
{
	memset(&glCommonData,0,sizeof(_glCommonData));
	return TRUE;
}


/**
 * bool clearCommonDataBuffer(void)
 * This function clears common buffer in glCommonData variable
 * @return: Returns TRUE always
 */
bool clearCommonDataBuffer(void)
{
	memset(&glCommonData.strCommonBuffer,0,FS_MAX_COMMON_BUFFER);
	return TRUE;
}


/**
 * bool appendToCommonBuffer(char *data,int length)
 * This function appends the data in the buffer to the common buffer
 * @param data: Data bytes to store
 * @param length : data length of the data buffer
 * @return: Returns TRUE if successfully written all the data bytes else FALSE
 */
bool appendDataToCommonBuffer(char *data,int length)
{
	int iCharCount=0;

	/* Read every byte from the data & store in the common buffer */
	for(iCharCount=0;(glCommonData.iCommonBufferLength<FS_MAX_COMMON_BUFFER) && (iCharCount<length);iCharCount++,glCommonData.iCommonBufferLength++)
		glCommonData.strCommonBuffer[glCommonData.iCommonBufferLength]=data[iCharCount];

if(iCharCount==length)
	return TRUE;

	/* If failed to write all the bytes then restore the original count */
	glCommonData.iCommonBufferLength-=iCharCount;

return FALSE;
}

/**
 * bool readDataFromCommonBuffer(char *data,int *length)
 * It is used to read data from the common buffer
 * @param data: Address to store the data
 * @param length: Integer pointer to store the length of the read data. Can be NULL if not required.
 * @return: Returns TRUE if all the data in the common buffer is read. FALSE otherwise.
 */
bool readDataFromCommonBuffer(char *data,int *length)
{
	int iCharCount=0;

	if(!data)
		return FALSE;

	/* Read every byte from the common buffer */
	for(iCharCount=0;(iCharCount<glCommonData.iCommonBufferLength);iCharCount++)
			data[iCharCount]=glCommonData.strCommonBuffer[iCharCount];

	/* Return TRUE if all the bytes are read successfully */
if(iCharCount==glCommonData.iCommonBufferLength)
{
	/* Set the length variable */
	if(length)
		*length=iCharCount;
	return TRUE;
}

	/* Set the length variable to zero*/
	if(length)
		*length=0;

return FALSE;
}

/**
 * bool popDataFromCommonBuffer(char *data,int length)
 * Pops the data from the common buffer. The bytes read from the buffer get erased & so is the buffer count
 * @param data: Address of the data buffer
 * @param length: Length of the data to pop
 * @return: Returns TRUE if & only if the length of data bytes read are equal to length supplied. FALSE otherwise.
 */
bool popDataFromCommonBuffer(char *data,int length)
{
	int iCharCount=0;
	int iOffset;


	memset(data,0,length);

	/* Read required bytes from the common buffer */
	for(iCharCount=0;(iCharCount<glCommonData.iCommonBufferLength) && (iCharCount<length);iCharCount++)
	{
		/* If address to store the data has been supplied then only write the data else purge */
			if(data)
			data[iCharCount]=glCommonData.strCommonBuffer[iCharCount];
	}

	/* Return TRUE if all the bytes are read successfully */
	if(iCharCount==length)
	{
		/* Set the offset to copy from */
		iOffset=length;
		length=glCommonData.iCommonBufferLength-length;
		for(iCharCount=0;length;length--,iCharCount++)
			glCommonData.strCommonBuffer[iCharCount]=glCommonData.strCommonBuffer[iCharCount+iOffset];

		if(iCharCount==(glCommonData.iCommonBufferLength-iOffset))
		{
			glCommonData.iCommonBufferLength-=iOffset;
			return TRUE;
		}
		else
			return FALSE;
	}

	/* Set the length variable to zero*/
	if(length)
		length=0;

return FALSE;
}
