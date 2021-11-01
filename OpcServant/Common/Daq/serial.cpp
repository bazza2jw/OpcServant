/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided with open62541.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "serial.hpp"
#ifdef _UNIX
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#endif
/*!

    \author B. J. Hill
    \date __DATE__
    License:  GNU LESSER GENERAL PUBLIC LICENSE 2.1
    (c)  Micro Research Limited 2010 -
    $Id: modbusmsg.hpp,v 1.1.1.1 2013/12/24 18:07:04 barry Exp $
*/
//
// Function: SerialConnect
// Purpose:To construct a SerialConnect object
// Inputs: name of port to open
// Outputs: none
// Returns: none
//
//
MRL::SerialConnect::SerialConnect()
{
#ifdef _UNIX
    // initialise the config structure
    memset(&_config,0,sizeof(_config));
#endif
#ifdef _WINDOWS
	memset(&Win_CommConfig,0,sizeof(Win_CommConfig));
	Win_CommConfig.dwSize = sizeof(Win_CommConfig); // SET THE STRUCTURE LENGTH
#endif

}
//
// baud rate constant translation tables
#if 0
#ifdef _WINDOWS
static int TransBaud[] =
{
	9600,
	CBR_110,
	CBR_300,
	CBR_600,
	CBR_1200,
	CBR_2400,
	CBR_4800,
	CBR_9600,
	CBR_19200,
	CBR_38400,
	CBR_56000,
	CBR_128000
};
#endif
#ifdef _UNIX
static int TransBaud[] =
{
	B0,
	B50,
	B75,
	B110 ,
	B300,
	B600,
	B1200,
	B2400,
	B4800,
	B9600,
	B19200,
	B38400,
	B57600,
	B115200
};
#endif
#endif
//
// Function: configure
// Purpose:To configure a serial port
// Inputs: baudrate, data size, parity , stop bits and flow control
// Outputs: none
// Returns: R_SUCCESS if completed successfully
//
//
bool MRL::SerialConnect::configure(int baud, int data, int stop, int parity, int flow)
{
    bool status = false;
    //wxLogDebug("Enter Configure fd = %d",fd());
    if(fd() != SC_INVALID_FD)
	{
		int ok = 1; // if ok is zeo we have an error
		// Validate baudrate
		switch(baud)
		{
#ifdef _UNIX
			case 0: // hangup modem lines
			case 50:
			case 75:
#endif
			case 110:
			case 300:
			case 600:
			case 1200:
			case 2400:
			case 4800:
			case 9600:
#ifdef _WINDOWS
			case 14400:
			case 28800:
#endif
			case 19200:
			case 38400:
			case 57600:
			case 115200:
			break;
			default:
			ok = 0;
			break;
		};
		if(ok)
		{
			switch(data)
			{
				case SC_5DATA:
				case SC_6DATA:
				case SC_8DATA:
				case SC_7DATA:
				break;
				default:
				ok = 0;
				break;
			};
		};
		if(ok)
		{
			switch(parity)
			{
				case SC_NOPARITY:
				case SC_EVENPARITY:
				case SC_ODDPARITY:
				break;
				default:
				ok = 0;
				break;
			};
		};
		if(ok)
		{
			switch(stop)
			{
				case SC_1STOP:
				case SC_2STOP:
				break;
				default:
				ok = 0;
				break;
			};
		};
		if(ok)
		{
			switch(flow)
			{
				case SC_FLOW_XON_XOFF:
				case SC_FLOW_HARD:
				case SC_FLOW_NONE:
				break;
				default:
				ok = 0;
				break;
			};
		};
		if(ok)
		{
			_baudrate = baud;
			_data = data;
			_stop = stop;
			_parity = parity;
			_flow = flow;
#ifdef _WINDOWS
			//
			// Set the data bits
			//
			Win_CommConfig.dcb.ByteSize=data;
			//
			// Set the stop bits
			//
			switch(stop)
			{
				case 1:
				Win_CommConfig.dcb.StopBits=ONESTOPBIT;
				break;
				case 2:
				Win_CommConfig.dcb.StopBits=TWOSTOPBITS;
				break;
				default:
				break;
			};
			//
			//
			switch(parity)
			{
				case SC_NOPARITY:
				Win_CommConfig.dcb.Parity= NOPARITY;
				Win_CommConfig.dcb.fParity=FALSE;
				break;
				case SC_EVENPARITY:
				Win_CommConfig.dcb.fParity=TRUE;
				Win_CommConfig.dcb.Parity= EVENPARITY;
				break;
				case SC_ODDPARITY:
				Win_CommConfig.dcb.fParity=TRUE;
				Win_CommConfig.dcb.Parity= ODDPARITY;
				break;
				default:
				break;
			};
			//
			//
			//
			switch (baud)
			{
				/*50 baud*/
				case 50:
				Win_CommConfig.dcb.BaudRate=CBR_110;
				break;
				/*75 baud*/
				case 75:
				Win_CommConfig.dcb.BaudRate=CBR_110;
				break;
				/*110 baud*/
				case 110:
				Win_CommConfig.dcb.BaudRate=CBR_110;
				break;
				/*134.5 baud*/
				case 134:
				Win_CommConfig.dcb.BaudRate=CBR_110;
				break;
				/*150 baud*/
				case 150:
				Win_CommConfig.dcb.BaudRate=CBR_110;
				break;
				/*200 baud*/
				case 200:
				Win_CommConfig.dcb.BaudRate=CBR_110;
				break;
				/*300 baud*/
				case 300:
				Win_CommConfig.dcb.BaudRate=CBR_300;
				break;
				/*600 baud*/
				case 600:
				Win_CommConfig.dcb.BaudRate=CBR_600;
				break;
				/*1200 baud*/
				case 1200:
				Win_CommConfig.dcb.BaudRate=CBR_1200;
				break;
				/*1800 baud*/
				case 1800:
				Win_CommConfig.dcb.BaudRate=CBR_1200;
				break;
				/*2400 baud*/
				case 2400:
				Win_CommConfig.dcb.BaudRate=CBR_2400;
				break;
				/*4800 baud*/
				case 4800:
				Win_CommConfig.dcb.BaudRate=CBR_4800;
				break;
				/*9600 baud*/
				case 9600:
				Win_CommConfig.dcb.BaudRate=CBR_9600;
				break;
				/*14400 baud*/
				case 14400:
				Win_CommConfig.dcb.BaudRate=CBR_14400;
				break;
				/*19200 baud*/
				case 19200:
				Win_CommConfig.dcb.BaudRate=CBR_19200;
				break;
				/*38400 baud*/
				case 38400:
				Win_CommConfig.dcb.BaudRate=CBR_38400;
				break;
				/*56000 baud*/
				case 56000:
				Win_CommConfig.dcb.BaudRate=CBR_56000;
				break;
				/*57600 baud*/
				case 57600:
				Win_CommConfig.dcb.BaudRate=CBR_57600;
				break;
				/*76800 baud*/
				case 76800:
				Win_CommConfig.dcb.BaudRate=CBR_57600;
				break;
				/*115200 baud*/
				case 115200:
				Win_CommConfig.dcb.BaudRate=CBR_115200;
				break;
				/*128000 baud*/
				case 128000:
				Win_CommConfig.dcb.BaudRate=CBR_128000;
				break;
				/*256000 baud*/
				case 256000:
				Win_CommConfig.dcb.BaudRate=CBR_256000;
				break;
			}
			//
			// set the flow control
			//
			switch(flow)
			{
				case SC_FLOW_XON_XOFF:
				Win_CommConfig.dcb.fDtrControl  =  DTR_CONTROL_DISABLE;
				Win_CommConfig.dcb.fRtsControl = RTS_CONTROL_DISABLE;
				Win_CommConfig.dcb.fOutxCtsFlow = Win_CommConfig.dcb.fOutxDsrFlow = FALSE;
				Win_CommConfig.dcb.fOutX = TRUE;
				Win_CommConfig.dcb.fInX  = TRUE;
				break;
				case SC_FLOW_HARD:
				Win_CommConfig.dcb.fDtrControl  =  DTR_CONTROL_ENABLE;
				Win_CommConfig.dcb.fRtsControl = RTS_CONTROL_ENABLE;
				Win_CommConfig.dcb.fOutxCtsFlow = Win_CommConfig.dcb.fOutxDsrFlow = TRUE;
				Win_CommConfig.dcb.fOutX = FALSE;
				Win_CommConfig.dcb.fInX  = FALSE;
				break;
				case SC_FLOW_NONE:
				Win_CommConfig.dcb.fDtrControl  =  DTR_CONTROL_DISABLE;
				Win_CommConfig.dcb.fRtsControl = RTS_CONTROL_DISABLE;
				Win_CommConfig.dcb.fOutxCtsFlow = Win_CommConfig.dcb.fOutxDsrFlow = FALSE;
				Win_CommConfig.dcb.fOutX = FALSE;
				Win_CommConfig.dcb.fInX  = FALSE;
				break;
				default:
				break;
			};
			// set the configuration
            status =  SetCommConfig(fd(), &Win_CommConfig, sizeof(COMMCONFIG));
#endif
			//
			//
			// ******************************
			//
#ifdef _UNIX
			//
			// start by clearing c_cflag
			//
            _config.c_cflag = CLOCAL | CREAD ;
            _config.c_iflag = IGNBRK;
            _config.c_oflag = 0; // no processing
            _config.c_lflag = 0; // default line status
            _config.c_line = N_TTY;
			//
			switch(data)
			{
				case SC_5DATA:
                _config.c_cflag |= CS5;
				break;
				case SC_6DATA:
                _config.c_cflag |= CS6;
				break;
				case SC_8DATA:
                _config.c_cflag |= CS8;
				break;
				case SC_7DATA:
                _config.c_cflag |= CS7;
				break;
				default:
				break;
			};
			switch(parity)
			{
				case SC_NOPARITY:
				break;
				case SC_EVENPARITY:
                _config.c_cflag |= PARENB;
				break;
				case SC_ODDPARITY:
                _config.c_cflag |= (PARENB | PARODD);
				break;
				default:
				break;
			};
			switch(stop)
			{
				case SC_1STOP:
				break;
				case SC_2STOP:
                _config.c_cflag |= CSTOPB;
				break;
				default:
				break;
			};
			switch(flow)
			{
				case SC_FLOW_XON_XOFF:
                _config.c_iflag |= IXON | IXOFF;
				break;
				case SC_FLOW_HARD:
                _config.c_cflag |= CRTSCTS;
				break;
				case SC_FLOW_NONE:
				break;
				default:
				break;
			};
			//
			// configure the port
			//
			int b = B9600;
			switch(baud)
			{
				case 0:
				b = B0;
				break;
				case 50:
				b = B50;
				break;
				//
				case 75:
				b = B75;
				break;
				//
				case 110:
				b = B50;
				break;
				//
				case 300:
				b = B300;
				break;
				//
				case 600:
				b = B600;
				break;
				//
				case 1200:
				b = B1200;
				break;
				//
				case 2400:
				b = B2400;
				break;
				//
				case 4800:
				b = B4800;
				break;
				//
				case 9600:
				b = B9600;
				break;
				//
				case 19200:
				b = B19200;
				break;
				//
				case 38400:
				b = B38400;
				break;
				//
				case 57600:
				b = B57600;
				break;
				//
				case 115200:
				b = B115200;
				break;
				//
				default:
				b = B9600;
				break;
			};
			//
            cfsetospeed(&_config,b); // set the baud rate (output)
			//
            cfsetispeed(&_config,b); // set the baud rate (input)
			//
            if(!tcsetattr(fd(),TCSANOW,&_config))
			{
                tcflow(fd(),TCOON); // enable output
                flowStart();
				status =true;
            }
            else
            {
                wxLogDebug("tcsetattr fails %s",strerror(errno));
            };
#endif
		};
	};
    //wxLogDebug("Serial::Configure status %s",status?"True":"False");
	return status;
}
//
// Function:FlushTransmit
// Purpose:To flush the transmit buffer
// Inputs: none
// Outputs: none
// Returns: none
//
//
void MRL::SerialConnect::flushTransmit()
{
    if(fd() != SC_INVALID_FD)
	{
#ifdef _WINDOWS
        PurgeComm(fd(),PURGE_TXCLEAR);
#endif
#ifdef _UNIX
        tcflush(fd(),TCOFLUSH);
#endif
	};
}
//
// Function: FlushReceive
// Purpose:To flush the receive buffer
// Inputs: none
// Outputs: none
// Returns: R_SUCCESS
//
//
void MRL::SerialConnect::flushReceive()
{
    if(fd() != SC_INVALID_FD)
	{
#ifdef _WINDOWS
        PurgeComm(fd(),PURGE_RXCLEAR);
#endif
#ifdef _UNIX
        tcflush(fd(),TCIFLUSH);
#endif
	};
}
//
// Function:Flow Stop
// Purpose:
// Inputs
// Outputs
// Returns
//
//
void  MRL::SerialConnect::flowStop()
{
    if(fd() != SC_INVALID_FD)
	{
#ifdef _WINDOWS
		EscapeCommFunction(fd,CLRDTR);
		EscapeCommFunction(fd,CLRRTS);
#endif
#ifdef _UNIX
        tcflow(fd(),TCION); // enable receive
#endif
	};
}
//
// Function: FlowStart
// Purpose:
// Inputs: none
// Outputs: none
// Returns: none
//
//
void MRL::SerialConnect::flowStart() // disable transmit and receive
{
    if(fd() != SC_INVALID_FD)
	{
#ifdef _WINDOWS
		EscapeCommFunction(fd,SETDTR);
		EscapeCommFunction(fd,SETRTS);
#endif
#ifdef _UNIX
        tcflow(fd(),TCIOFF);
#endif
	};
}
//
// Function: Open
// Purpose:To open a serial port
// Inputs: port name
// Outputs: none
// Returns: R_SUCCESS on success
//
bool MRL::SerialConnect::open(const std::string &name)  // open a connection using a mode string
{
    _name = name;
    return open();
}
/*!
 * \brief MRL::SerialConnect::open
 * \return
 */
bool MRL::SerialConnect::open()  // open a connection using a mode string
{
        bool status = true;
#ifdef _WINDOWS
        _fd=CreateFileA((const char *)name, GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		DOUT("CreateFileA  fd " << fd << " Last Error " << GetLastError());
        if (_fd !=INVALID_HANDLE_VALUE)
		{
            unsigned long confSize = sizeofint(Win_CommConfig);
			/*configure port settings*/
			GetCommConfig(fd, &Win_CommConfig, &confSize);
			GetCommState(fd, &(Win_CommConfig.dcb));
			Win_CommConfig.dcb.fBinary=TRUE;
			Win_CommConfig.dcb.fInX=FALSE;
			Win_CommConfig.dcb.fOutX=FALSE;
			Win_CommConfig.dcb.fAbortOnError=FALSE;
			Win_CommConfig.dcb.fNull=FALSE;
			//SetupComm(fd,8000,8000); // make sure we have big buffers for comms
			//
			COMMTIMEOUTS ct;
			memset(&ct,0,sizeof(ct)); // non-blocking i/o
			//
			ct.ReadIntervalTimeout = MAXDWORD;
			SetCommTimeouts(fd,&ct);
			//
            SetupComm(fd(),8000,8000); // make sure we have big buffers for comms
			//
            if(!configure(9600))
			{
                close();
				status = false;
			};
		}
		else
		{
			status = false;
		};
#endif
#ifdef _UNIX
        std::string port = std::string("/dev/") + _name;
        _fd = ::open(port.c_str(),O_RDWR | O_NDELAY);
        if(fd() > 0)
		{
            tcgetattr(fd(),&_config); // get the current attributes
            if(!configure(_baudrate,_data,_stop,_parity,_flow))
			{
                wxLogDebug("Serial::open Failed to default configure %d %s",errno,strerror(errno));
                close();
				status = false;
			};
		}
		else
		{
            _fd = SC_INVALID_FD;
			status = false;
            wxLogDebug("Open Serial Fails %d %s",errno,strerror(errno));

		};
#endif

	return status;
}
//
// Function:Close
// Purpose:To close a serial port
// Inputs: none
// Outputs: none
// Returns: Connection::Close
//
//
void MRL::SerialConnect::close() // close a connection
{
    if(fd() != SC_INVALID_FD)
	{
#ifdef _WINDOWS
        CloseHandle(fd()); // Win 32
#endif
#ifdef _UNIX
        ::close(fd());
#endif
	};
    _fd = SC_INVALID_FD;
}


//
// Function: portRead
// Purpose:To read from the serial port
// Inputs: dest buffer, buffer length
// Outputs: none
// Returns: numebr of bytes read
//
//
int MRL::SerialConnect::read(void *pDest, int nDestLen)
{
	int nDataRead = 0;
    if(fd() != SC_INVALID_FD)
	{
#ifdef _WINDOWS
		// clear any pending errors
		// Win 32 is similar to the Connection::Read
		//
		COMSTAT Win_ComStat;
		DWORD Win_BytesRead=0;
		DWORD Win_ErrorMask=0;
		ClearCommError(fd, &Win_ErrorMask, &Win_ComStat);
        if (Win_ComStat.cbInQue && (!ReadFile(fd(), (void*)pDest, (DWORD)nDestLen, &Win_BytesRead, NULL) || Win_BytesRead==0))
		{
			nDataRead =  -1;
		}
		else
		{
			nDataRead =  ((int)Win_BytesRead);
		}
		//
#endif
		//
		//
#ifdef _UNIX
        int n = ::read(fd(),pDest,nDestLen); // the only error we should get is no data
		nDataRead = (n >= 0)?n:0;
#endif
	};
	return nDataRead;
}
//
// Function: Write
// Purpose:To write to the serial port
// Inputs: source data, data lenth
// Outputs: none
// Returns: numebr of bytes written
//
//
int  MRL::SerialConnect::write(const void *pSrc,int nSrcLen)
{
	int nSrcWrite = 0;
    if(fd() != SC_INVALID_FD)
	{
#ifdef _WINDOWS
		//
		// if you find an application locks here check the flow control
		//
		DWORD n = 0;
        if(WriteFile(fd(),pSrc,nSrcLen,&n,NULL))
		{
			nSrcWrite = (int)n;
		};
#endif
#ifdef _UNIX
        int n = ::write(fd(),pSrc,nSrcLen);
		nSrcWrite = (n >= 0)?n:0;
#endif
	};
	return nSrcWrite;
}

int MRL::SerialConnect::sendBreak(int duration)
{
  #ifdef _UNIX
    // the parameter is equal with linux
    return tcsendbreak(fd(),duration);
  #endif

#ifdef _WINDOWS
    return 0;
#endif

}
