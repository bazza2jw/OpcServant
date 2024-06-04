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
    _hasStart =  ((_start == 0) && (c > ' '))|| (c == _start);
    if(_hasStart)
    {
        _buffer.push_back(c);
    }
    return _hasStart;
}

/*!
 * \brief SerialPacket::gotEnd
 * \return true on success
 */
bool MRL::SerialInterface::gotEnd()
{
    int c;
    while((c = getChar()) >= 0)
    {
        _hasEnd =  (c == _end);
        _buffer.push_back(c);
        if(_hasEnd)
        {
            break;
        }
    }
    return _hasEnd;
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
                _hasTimedOut = true;
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
                _hasTimedOut = true;
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
    _hasStart = false;
    _hasEnd = false;
    _hasTimedOut = false;

}

/*!
 * \brief MRL::SerialInterface::startPacket
 * \param s
 * \param l
 */
void MRL::SerialInterface::startPacket(const char * s, size_t l)
{
    clearTimeout();
    flush();
    std::string b(s,l);
    writeString(b);
    _statePacket = StateWaitStart;
    _hasStart = false;
    _hasEnd = false;
    _hasTimedOut = false;
}
