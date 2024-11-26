/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef SERIALPACKET_H
#define SERIALPACKET_H
#include <libserial/SerialPort.h>
#include <Common/Daq/serialinterface.h>

namespace MRL {
/*!
    \brief The SerialPacket class
*/

LibSerial::BaudRate fromIntBaudRate(int b);

class SerialPacket : public LibSerial::SerialPort,public SerialInterface {
    std::string _port;
    LibSerial::BaudRate      _baudRate        = LibSerial::BaudRate::BAUD_DEFAULT;
    LibSerial::CharacterSize _characterSize   = LibSerial::CharacterSize::CHAR_SIZE_DEFAULT;
    LibSerial::FlowControl   _flowControlType = LibSerial::FlowControl::FLOW_CONTROL_DEFAULT;
    LibSerial::Parity        _parityType      = LibSerial::Parity::PARITY_DEFAULT;
    LibSerial::StopBits      _stopBits        = LibSerial::StopBits::STOP_BITS_DEFAULT;

public:
    /*!
     * \brief SerialPacket
     */
    SerialPacket() {}
    /*!
    */
    ~SerialPacket() {
    }



    /*!
        \brief openSerial
        \param port
        \param baud
        \param data
        \param stopbits
        \param parity
        \return true if serial port opened
    */
    bool openSerial(const std::string &port,
                    const LibSerial::BaudRate&      baudRate        = LibSerial::BaudRate::BAUD_DEFAULT,
                    const LibSerial::CharacterSize& characterSize   = LibSerial::CharacterSize::CHAR_SIZE_DEFAULT,
                    const LibSerial::FlowControl&   flowControlType = LibSerial::FlowControl::FLOW_CONTROL_DEFAULT,
                    const LibSerial::Parity&        parityType      = LibSerial::Parity::PARITY_DEFAULT,
                    const LibSerial::StopBits&      stopBits        = LibSerial::StopBits::STOP_BITS_DEFAULT)

    {
        bool ret = false;
        //
        _port =  port;
        _baudRate = baudRate;
        _characterSize = characterSize;
        _flowControlType = flowControlType;
        _parityType = parityType;
        _stopBits = stopBits;
        //
        try
        {
            if (!IsOpen()) {
                Open(_port);
                SetBaudRate(_baudRate);
                SetParity(_parityType);
                SetFlowControl(_flowControlType);
                SetCharacterSize(_characterSize);
                SetStopBits(_stopBits);
                setLocked(false);
            }
            else {
                ret = true;
            }
        }
        catch(...)
        {
            ret = false;
        }
        return ret;
    }


    /*!
        \brief getChar
        \return character or -1
    */
    int getChar() {
        int ret = -1;
        try
        {
            if(IsDataAvailable())
            {
                char c = 0;
                ReadByte(c);
                ret = int(c);
            }
        }
        catch(...)
        {
            ret = -1;
        }

        return ret;
    }


    /*!
        \brief flush
    */
    void flush() {
        clearBuffer();
        FlushInputBuffer() ;
        FlushOutputBuffer() ;
    }

    /*!
        \brief isOk
        \return true if ok
    */
    bool isOk() {
        return IsOpen();
    }
    /*!
        \brief reopen
    */
    void reopen() {
        try
        {
            if(IsOpen()) Close();
            Open(_port);
            SetBaudRate(_baudRate);
            SetParity(_parityType);
            SetFlowControl(_flowControlType);
            SetCharacterSize(_characterSize);
            SetStopBits(_stopBits);
        }
        catch(...)
        {

        }
    }

    void writeString(const std::string &s)
    {
        Write(s);
    }

};

typedef std::unique_ptr<SerialPacket> SerialPacketRef;
}

#endif // SERIALPACKET_H
