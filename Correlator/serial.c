#include "global.h"
//
// interrupt driven RX
//
#define BUFF_SIZE 64
typedef struct 
{
	unsigned char data[BUFF_SIZE]; // the buffer
	volatile unsigned in;
	volatile unsigned out;
	volatile unsigned count;
} BUFFER;
//
static BUFFER Rx;
static BUFFER Tx;
//
#if 0
void __attribute__((__interrupt__)) _U2RXInterrupt(void)
{
	while(U2STAbits.URXDA) // there is a FIFO
	{
		unsigned c = U2RXREG;
		Rx.data[Rx.in++] = (unsigned char )c;		
		Rx.count++;
		Rx.in &= (BUFF_SIZE - 1);
	};
	_U2RXIF = 0;
	//printf("Rx");
};
//
//
//
void __attribute__((__interrupt__)) _U2TXInterrupt(void)
{
	while(!U2STAbits.UTXBF)
	{
		if(Tx.count > 0) // anything left to send
		{
			U2TXREG = Tx.data[Tx.out++];
			Tx.out %= BUFF_SIZE;
			Tx.count--;
		};
		if(Tx.count == 0) // nothing to send so disable IRQ 
		{
			_U2TXIE = 0; // disable IRQ		
			return; // do not clear flag
		};
	};
	_U2TXIF = 0;
};
#endif

inline void ClearUartIrq()
{
	_U2TXIE = _U2RXIE = 0;	
};

inline void SetUartIrq()
{
	_U2RXIE = 1;
	if(Tx.count) _U2TXIE = 1;	
};
//
// Flush / reset the buffers
//
void FlushUart(void)
{
	ClearUartIrq();
	Rx.in = Rx.out = Rx.count = 0;
	Tx.in = Tx.out = Tx.count = 0;	
	SetUartIrq();
};

int TxCanSend()
{
	return BUFF_SIZE - Tx.count;	
};
// send a character
int TxChar(unsigned char c)
{
	int res = 0;	
	_U2TXIE = 0;
	if(Tx.count < BUFF_SIZE)
	{
		if((Tx.count == 0) && !U2STAbits.UTXBF)
		{
			U2TXREG = c; // send this char now
		}
		else
		{
			Tx.data[Tx.in++] = c;
			Tx.count++;
			Tx.in %= BUFF_SIZE; // ring buffer
		};
		res = 1;
	};
	_U2TXIE = 1; // must enable IRQ on exit
	return res;	
};

// send a buffer
int TxBuffer(unsigned char *b, unsigned len)
{
	int i;
	for(i = 0; i < len ; i++,b++)
	{
		if(!TxChar(*b))break;
	};
	return i; // return number of chars sent
};

// send a null terminated string
int TxLine(unsigned char *b)
{
	for(;*b;b++)
	{
		while(!TxCanSend())
		{
			asm("CLRWDT");	
		};
		TxChar(*b);	
	};
	//printf("count %d in %d out %d\r\n",Tx.count,Tx.in,Tx.out);
};
//
// Test for a character
//
int RxCharReady()
{
	int res;
	_U2RXIE = 0;
	res = Rx.count;
	_U2RXIE = 1;
	return res;	// returns number of chars in Rx Buffer
};

//
// rx a char or return -1
//
int RxChar()
{
	int res = -1;
	_U2RXIE = 0;
	 if(Rx.count)
	 {
		res = Rx.data[Rx.out++];
		Rx.count--;
		Rx.out %= BUFF_SIZE;	 
	 };
	_U2RXIE = 1;
	return res;	
};

// receive a buffer
int RxBuffer(unsigned char *b, unsigned len)
{
	int i;
	memset(b,0,len);
	for(i = 0; i < len; i++,b++)
	{
		int c = RxChar();
		if(c >= 0) 
		{
			*b = (unsigned char)c;	
		}
		else
		{
			break;
		};	
	};
	return i; // return length of data received
};
//
// receive a line up to a CR of LF
int RxLine(unsigned char *b, int l)
{
	int i;
	memset(b,0,l); // clear the buffer
	for(i = 0; i < l; i++, b++)
	{
		while(!RxCharReady());
		int c = RxChar();
		if(c < ' ') break;
		*b = (unsigned char)c;
	};	
	return i;
};

// initialise serial ports
void InitSerial()
{
	//
	// The intention is to use U1 for comms to and from a host and U2 for debug / maintainance - we will see
	// however the cheap ethernet interface means this is probably  no longer needed - except if we use USB
	// in which case we can run at 115k or more
	//
	// set both to 56k
	// 
	//
	_U1RXIE = 0;
	_U1RXIF = 0;
	U1MODE = 0x8000; // 8 bits no parity 16 clocks per bit
	U2MODE = 0x8000; //
	//
	U2STA = 0x400;
	U1STA = 0x400; // enable transmit - non - irq driven
	//
	// set the baud rate to 56k
	U1BRG = 43 ; //Fcy / (4 * BAUD_RATE) - 1; 
	U2BRG = 43 ; //Fcy / (4 * BAUD_RATE) - 1;
	//
	FlushUart();
	_U2RXIF = 0;
	_U2RXIE = 0;
};

// although the peripheral lib
// is the same 
// get a char on UART1
unsigned char GetChar1()
{
	while(!U1STAbits.URXDA);
	return U1RXREG;
};

// Send a char on UART1
void SendChar1(unsigned char c)
{
	while(!U1STAbits.TRMT); // wait for tx buffer to empty
	U1TXREG = c; 
};

// send a string
void SendString1(const unsigned char *s)
{
	for(;*s;s++)SendChar1(*s);
};

// get the string
int GetString1(unsigned char *s, int l)
{
	int i;
 	memset(s,0,l);
	for(i = 0; i < l;i++,s++)	
	{
		unsigned char c = GetChar1();
		if(c == 0x0d) break;
		*s = c;
	}; 
	return i;
};

// get a char on UART2
unsigned char GetChar2()
{
	while(!U2STAbits.URXDA);
	return U2RXREG;
};

int CharReady2()
{
	return U2STAbits.URXDA;		
};
// Send a char on UART2
void SendChar2(unsigned char c)
{
	while(!U2STAbits.TRMT); // wait for tx buffer to empty
	U2TXREG = c; 
};

// send a string
void SendString2(const unsigned char *s)
{
	for(;*s;s++)SendChar2(*s);
};

// get the string
int GetString2(unsigned char *s, int l)
{
	int i;
 	memset(s,0,l);
	for( i = 0; i < l;i++,s++)	
	{
		unsigned char c = GetChar2();
		if(c == 0x0d) break;
		*s = c;
	}; 
	return i;
};


