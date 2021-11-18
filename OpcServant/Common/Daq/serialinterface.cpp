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
#include "serialinterface.h"

/*!
 * \brief SerialPacket::gotStart
 * \return true on success
 */
bool MRL::SerialInterface::gotStart()
{
    int c = getChar();
    bool ret =  ((_start == 0) && (c > ' '))|| (c == _start);
    if(ret)
    {
        _buffer.push_back(c);
    }
    return ret;
}

/*!
 * \brief SerialPacket::gotEnd
 * \return true on success
 */
bool MRL::SerialInterface::gotEnd()
{
    int c;
    bool ret = false;
    while((c = getChar()) >= 0)
    {
        ret =  (c == _end);
        if(!ret)
        {
            _buffer.push_back(c);
        }
        else
        {
            break;
        }
    }
    return ret;
}

/*!
 * \brief SerialInterfaceScript::readLine
 * \param start
 * \param end
 * \return received line of text
 */
std::string & MRL::SerialInterface::readLine()
{
    try
    {
        _to.Start(); // trigger stopwatch
        _buffer.clear();
        while(!gotStart() && !timedOut());
        if(!timedOut())
        {
            _to.Start();
            while(!gotEnd() && !timedOut());
        }
    }
    catch(...)
    {
        _buffer.clear();

    }
    return _buffer;
}

/*!
 * \brief SerialInterface::packetDrive
 * \return true on success
 */
bool MRL::SerialInterface::packetDrive()
{
    bool ret = false;
    switch(_statePacket)
    {
        case StateWaitStart:
        if(gotStart() )
        {
            _statePacket = StateWaitEnd;
        }
        else
        {
            if(timedOut())
            {
                _statePacket = StateNone;
                ret = true;
            }
        }
        break;
        case StateWaitEnd:
        if(gotEnd())
        {
            _statePacket = StateNone;
            ret = true;
        }
        else
        {
            if(timedOut())
            {
                _statePacket = StateNone;
                ret = true;
            }
        }
        break;
        default:
        break;
    }
    return ret;
}

/*!
 * \brief MRL::SerialInterface::startPacket
 * \param s
 */
void MRL::SerialInterface::startPacket(const std::string &s)
{
    clearTimeout();
    flush();
    writeString(s);
    _statePacket = StateWaitStart;
}

