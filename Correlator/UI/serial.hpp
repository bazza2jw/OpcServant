
#ifndef serial_hpp
#define serial_hpp
#include <QString>
#include <QTime>
#include <QTimer>
#include "fifo.hpp"
#include <QByteArray>
#include "Utility.hpp"
// Description:
// This is a general Serial interface class
// Under Unix we just open a serial port put it into raw mode
// and mark it as No Delay
// Under Windows 16bit we use the OpenComm, ReadComm, WriteComm, CloseComm and DCB stuff
// Under Windows 32 bit we  use Createfile, ReadFile WriteFile  and DeviceIoControl
//
// *********************************
//
// are we Unix
#ifdef UNIX
#include <termio.h>
#endif
#ifdef _WINDOWS
#include "windows.h"
#endif
//
// we have to unify the constants used
// Windows uses one set Unix another
//
enum
{
#ifdef UNIX
	SC_50 = 1,
	SC_75,
	SC_110 ,
#endif
#ifdef _WINDOWS
	SC_110 = 1,
#endif
	SC_300,
	SC_600,
	SC_1200,
	SC_2400,
	SC_4800,
	SC_9600,
	SC_19200,
	SC_38400,
	SC_57600,
	SC_115200
};
//
// other constants set to useful values
//
#define SC_5DATA 5
#define SC_6DATA 6
#define SC_8DATA 8
#define SC_7DATA 7
#define SC_NOPARITY 'N'
#define SC_EVENPARITY 'E'
#define SC_ODDPARITY 'O'
#define SC_MARK 'M'
#define SC_SPACE 'S'
#define SC_1STOP 1
#define SC_2STOP 2
#define SC_FLOW_XON_XOFF 2
#define SC_FLOW_HARD 1
#define SC_FLOW_NONE 0
//
#ifdef _WINDOWS
#define SC_INVALID_FD (INVALID_HANDLE_VALUE)
#else
#define SC_INVALID_FD (-1)
#endif
//
// error codes
//
enum
{
	SC_SUCCESS = 0,
	SC_INVALID_BAUDRATE, // not a valid baudrate
	SC_INVALID_DATASIZE, // not a valid data size
	SC_INVALID_PARITY, // not a valid parity
	SC_INVALID_STOPBITS, // not a valid number of stop bits
	SC_CANNOT_OPEN, // cannot open the port
	SC_CANNOT_CONFIGURE, // cannot configure the port
	SC_INVALID_FLOW,  // invalid flow control option
	SC_FD_INVALID  // the FD is not valid
};
//
class SerialConnect  // we assume the user task/thread locks before use
{
#ifdef UNIX
	int fd; // serial port fd
#endif
	// Under Windows a DCB is used to hold the COM port configuratiom
#ifdef _WINDOWS
	COMMCONFIG	Win_CommConfig;
	COMMTIMEOUTS Win_CommTimeouts;
	HANDLE fd; // we have HANDLE for 32
#endif
	//
#ifdef UNIX
	// Linux termio configuration
	struct termios Config;
#endif
	//
	QString Name;
	//
	Fifo In;         // the input fifo
	//
	int _baudrate;
	int _data;
	int _stop;
	int _parity;
	int _flow;
	//
	public:
	//
	// configure from given values
	//
	int Configure(int baud, int data = SC_8DATA,
	int stop = SC_1STOP, int parity = SC_NOPARITY,
	int flow = SC_FLOW_NONE);
	void FlushTransmit(); // flush the Tx buffer
	void FlushReceive(); // flush the receive buffer
	void FlowStart(); // enable receive
	void FlowStop(); // Disable Receive
	//
	void Flush()
	{
		In.Flush();
		FlushTransmit();
		FlushReceive();
	};
	
	SerialConnect(QString s = "");
	~SerialConnect() {};
	//
	int Open(QString);  // open a connection using a mode string
	void Close(); // close a connection
	//
	int Read(void *buf, int maxlen); // read from the fifo
	int portRead(void *buf, int maxlen); // read from the serial port
	//
	int Write(const void *buf,int len);// write to a serial port
	const QString &GetName() const { return Name;}; // get the object's name
	int Write(QByteArray b)
	{
		//GDS << endl << "Serial:: Write " << (const char *)b << endl;
		return Write((void *)(b.data()),b.size());
	};
	//
	void PutChar(int c) { char b = (char)c; Write(&b);};
	int GetChar() 
	{
		if(canRead())
		{
			char c;
			Read(&c,1);
			return c;
		}
		return - 1;
	};
	int GetLine(QByteArray &b, int to, int term = '\r')
	{
		QTime t;
		t.start();
		b.clear(); // clear the buffer
		//GDS << "GetLine to = " << to << endl;
		while(t.elapsed() < to) // must complete in 1 second
		{
			int c = GetChar();
			if(c == term) 
			{
				//GDS << "Got Terminator" << endl;
				break;
			};
			if(c >= 0)
			{
				//GDS << "Got Char " << (int)c << endl; 
				b += c;
			};
		};
		return b.size();
	};
	void Hangup() // drop modem lines
	{
		if(fd != SC_INVALID_FD)
		{
#ifdef UNIX
			cfsetospeed(&Config,B0); // set the baud rate (output)
			cfsetispeed(&Config,B0); // set the baud rate (input)
			tcsetattr(fd,TCSANOW,&Config);
#endif
#ifdef _WINDOWS
			EscapeCommFunction(fd,CLRDTR);
#endif
		};
	};
	//
	bool canRead(); // checks for data
	//
	void Tick(); // periodic handler
	//
	void Reset() // reload the configuration
	{
		Configure(_baudrate, _data,_stop,_parity,_flow);
		FlowStart();
	};
	//
	static QString TranslateSerialName(const QString &s); // convert from Unix to Windows name - very platform dependent
#ifdef _WINDOWS
	HANDLE GetFd() const { return fd;}; // need this for low level debugging
#endif
	bool isOpen() const { return (fd != SC_INVALID_FD);	};
	//
	int Baudrate() const { return _baudrate;}; // baud rate index
	int Data() const { return _data;};
	int Stop() const { return _stop;};
	int Parity() const { return _parity;};
	int Flow() const { return _flow;};
};
#endif

