
#include "fifo.hpp"
// Description:
// Fifo class
//
// *********************************
//
//
// Function: Fifo
// Purpose:Constructs FIFO class
// Inputs: none
// Outputs: none
// Returns: none
//
Fifo::Fifo( unsigned size)
{
	nSize = size;
	pBuf = new char [nSize];
	nIn = 0; // insertion index
	nOut = 0; // extraction index
	nData = 0; // data in fifo
};
//
// Function:~Fifo
// Purpose:
// Inputs: none
// Outputs: none
// Returns: none
//
//
Fifo::	~Fifo()
{
	if(pBuf)
	{
		delete pBuf;
	};
};
//
// Function:Flush
// Purpose:to clear the fifo buffer
// Inputs: none
// Outputs: none
// Returns: R_SUCCESS;
//
//
void  Fifo::Flush()
{
	nIn = 0;
	nOut = 0;
	nData = 0;
};
//
// Function: Read
// Purpose:To read from a Fifo
// Inputs: destination buffer, lenght of buffer
// Outputs: data read
// Returns: R_SUCCESS on success
//
//
unsigned Fifo::Read(void *pDest, unsigned nDestLen, unsigned &nDataRead) // read from a connection
{
	unsigned status = R_SUCCESS;
	nDataRead = 0;
	*((char *)pDest) = 0;
	if(nData)
	{
		if(nData > 0)
		{
			if(nData < nDestLen)
			{
				nDestLen = nData; // clip the data length
			};
			nDataRead = nDestLen; // set the amount of data read
			char *p = (char *)pDest;
			for(unsigned i = 0; i < nDestLen; i++)
			{
				*(p++) = pBuf[nOut++];
				nData--;
				if(nOut >= nSize)
				{
					nOut = 0; // wrap around
				};
			};
		};
	};
	return status;
};
//
// Function: Write
// Purpose:Writes data to a Fifo
// Inputs: Src buffer, length of buffer
// Outputs: Data written length, either  all or nothing is written
// Returns: R_SUCCESS on success,
//
//
unsigned Fifo::Write(void *pSrc, unsigned nSrcLen, unsigned &nSrcWrite)// write to a connection
{
	unsigned status = R_SUCCESS;
	nSrcWrite = 0;
	if(nSrcLen <= Free()) // write all or nothing
	{
		nSrcWrite = nSrcLen;
		char *p = (char *)pSrc;
		for(unsigned i = 0; i < nSrcLen; i++)
		{
			pBuf[nIn++] = *(p++); // copy to fifo
			nData++;
			if(nIn >= nSize)
			{
				nIn = 0;
			};
		};
	};
	return status;
};
//*****************************************************
//
//Function: Peek
//Purpose:to read up to nData bytes from the fifo but not update the extraction
// pointer
//Inputs:pointer to buffer, number of bytes to read and refernce to number of bytes transfered
//Ouputs: number of bytes transfered
//Returns:R_SUCCESS on Success R_FAILURE otherwise
//
//*****************************************************
unsigned Fifo::Peek(char *pData, unsigned nRead, unsigned &nPeek)
{
	nPeek = 0;
	if(nRead > 0)
	{
		if(nRead > nData)
		{
			nPeek = nRead = nData;
			unsigned out = nOut;
			for(unsigned i = 0; i < nRead; i++)
			{
				pData[i] = pBuf[out++];    // copy byte across
				if(out >= nSize) out  = 0; // ring round
			};
		};
	};
	return R_SUCCESS; // cannot fail really
};
//*****************************************************
//Function: Drop
//Purpose: to drop a given number of bytes from the buffer.
//Inputs:number of bytes to drop
//Ouputs:number of bytes dropped
//Returns:R_SUCCESS on Success R_FAILURE otherwise
//*****************************************************
unsigned Fifo::Drop(unsigned nDrop, unsigned &nDropped)
{
	nDropped = 0;
	if(nSize > 0)
	{
		if(nDrop > nData) nDrop = nData;
		nOut += nDrop;
		nOut %= nSize;
		nDropped = nDrop;
	};
	return R_SUCCESS;
};

