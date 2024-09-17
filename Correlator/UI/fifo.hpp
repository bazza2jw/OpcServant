#ifndef FIFO_HPP
#define FIFO_HPP

#define R_SUCCESS (0)
//
#define FIFO_SIZE 10000
class  Fifo
{
	unsigned nSize; // size of Fifo
	unsigned nIn; // insertion pointer
	unsigned nOut; // extractoion pointer
	char *pBuf; // buffer
	unsigned nData; // number of data bytes in buffer
	public:
	Fifo(unsigned size = FIFO_SIZE);
	virtual	~Fifo();
	unsigned Read(void *pDest, unsigned nDestLen, unsigned &nDataRead); // read from a connection
	unsigned Write(void *pSrc, unsigned nSrcLen, unsigned &nSrcWrite); // write to a conenction
	void Flush(); // flush the fifo
	unsigned Peek(char *, unsigned nRead, unsigned &nPeek); // peek the next few bytes
	unsigned Drop(unsigned nDrop , unsigned &nDropped); // drop a number of bytes from the buffer
	unsigned Free() { return  (nSize - nData); }; // free space
	unsigned Count() const { return nData;}; // return the number of bytes in the buffer
	const char *Buffer() { return pBuf;}; // return the buffer
	void Put(char c) {unsigned n ; Write(&c,1,n);};
	int  Get()
	{
		if(Count() > 0)
		{
			unsigned n;
			char c;
			Read(&c,1,n);
			return (c & 0xFF);
		}
		return - 1;
	};
};
#endif
