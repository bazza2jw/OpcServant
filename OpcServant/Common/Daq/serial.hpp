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
/*!

    \author B. J. Hill
    \date __DATE__
    License:  GNU LESSER GENERAL PUBLIC LICENSE 2.1
    (c)  Micro Research Limited 2010 -
*/
#ifndef serial_hpp
#define serial_hpp
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
// Description:
// Taken from Foresite code and ported to wxWidgets
// This is a general Serial interface class
// Under Unix we just open a serial port put it into raw mode
// and mark it as No Delay
// Under Windows 16bit we use the OpenComm, ReadComm, WriteComm, CloseComm and DCB stuff
// Under Windows 32 bit we  use Createfile, ReadFile WriteFile  and DeviceIoControl
//
// *********************************
//

#include <string>
// are we Unix
#ifdef _UNIX
#include <termio.h>
#endif
#ifdef _WINDOWS
#include "windows.h"
#endif
//
// we have to unify the constants used
// Windows uses one set Unix another
//
enum {
    #ifdef _UNIX
    SC_50 = 1,
    SC_75,
    SC_110,
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
enum {
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
namespace MRL {
    /*!
        \brief The SerialConnect class
    */
    class  SerialConnect {
            #ifdef _UNIX
            int _fd = SC_INVALID_FD; // serial port fd
            #endif
            // Under Windows a DCB is used to hold the COM port configuratiom
            #ifdef _WINDOWS
            COMMCONFIG  Win_CommConfig;
            COMMTIMEOUTS Win_CommTimeouts;
            HANDLE _fd = SC_INVALID_FD; // we have HANDLE for 32
            #endif
            //
            #ifdef _UNIX
            // Linux termio configuration
            struct termios _config;
            #endif
            //
            std::string _name;
            //
            int _baudrate = 57600;
            int _data = 8;
            int _stop = 1;
            int _parity = 'N';
            int _flow = SC_FLOW_NONE;
            //
        public:
            //
            // configure from given values
            //
            bool configure(int baud, int data = SC_8DATA,
                           int stop = SC_1STOP, int parity = SC_NOPARITY,
                           int flow = SC_FLOW_NONE);
            void flushTransmit(); // flush the Tx buffer
            void flushReceive(); // flush the receive buffer
            void flowStart(); // enable receive
            void flowStop(); // Disable Receive
            int sendBreak(int duration = 0); // send a break - needed for SDI12
            //
            SerialConnect(const std::string &port) : SerialConnect() {
                open(port);
            }
            SerialConnect();
            ~SerialConnect() {
                close();
            }
            //
            bool open(const std::string &name);  // open a connection using a mode string
            bool open(); // open current port
            void close(); // close a connection
            //
            int read(void *buf, int maxlen); // read from the serial port
            //
            int write(const void *buf, int len = 1); // write to a serial port
            //
            void hangup() { // drop modem lines
                if (_fd != SC_INVALID_FD) {
                    #ifdef _UNIX
                    cfsetospeed(&_config, B0); // set the baud rate (output)
                    cfsetispeed(&_config, B0); // set the baud rate (input)
                    tcsetattr(fd(), TCSANOW, &_config);
                    #endif
                    #ifdef _WINDOWS
                    EscapeCommFunction(fd, CLRDTR);
                    #endif
                };
            }
            //
            void reset() { // reload the configuration
                close();
                open();
                configure(_baudrate, _data, _stop, _parity, _flow);
                flowStart();
            }
            //
            #ifdef _UNIX
            int fd() const {
                return _fd;   // serial port fd
            }
            #endif
            // Under Windows a DCB is used to hold the COM port configuratiom
            #ifdef _WINDOWS
            HANDLE fd() const {
                return _fd;   // serial port fd
            }
            #endif
            //
            bool isOpen() const {
                return (_fd != SC_INVALID_FD);
            }
            //
            int baudrate() const {
                return _baudrate;   // baud rate index
            }
            int data() const {
                return _data;
            }
            int stop() const {
                return _stop;
            }
            int parity() const {
                return _parity;
            }
            int flow() const {
                return _flow;
            }
            const std::string &name() {
                return _name;
            }
            //
    };
}

#endif

