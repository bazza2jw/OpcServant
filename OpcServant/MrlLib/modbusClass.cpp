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
#include "modbusClass.h"
#include <iostream>


/**
 * @brief
 *
 * @param parent
 */
MrlModbus::Modbus::Modbus() :
     _ctx(0),
    _numberRetries(2)
{
}

//construct a TCP connection
/**
 * @brief
 *
 * @param h
 * @param port
 */
bool MrlModbus::Modbus::setTcpConnection(const std::string &h,  const std::string &port ) // TCP connection
{
    if(_ctx)
    {
       close();
       modbus_free(_ctx);
       _ctx = nullptr;
    }
    if(h.empty() || port.empty()) THROW_DEF("MrlModbus::Modbus::setTcpConnection");
    _ctx = modbus_new_tcp_pi(h.c_str(), port.c_str());
    setSlave(1); // default slave address
    return (_ctx != nullptr);
}

// construct an RTU interface on a serial port
/**
 * @brief
 *
 * @param port
 * @param baud_rate
 * @param parity
 * @param data_bit
 * @param stop_bit
 */


bool MrlModbus::Modbus::setSerialConnection(const std::string &port, int baud_rate , char parity , int data_bit , int stop_bit )
{
    if(_ctx)
    {
       close();
       modbus_free(_ctx);
       _ctx = nullptr;
    }
    if(port.empty()) THROW_DEF("MrlModbus::Modbus Invalid Port");
    _ctx = modbus_new_rtu(port.c_str(), baud_rate, parity, data_bit, stop_bit);
    return (_ctx != nullptr);
}


/**
 * @brief
 *
 * @return bool
 */
bool MrlModbus::Modbus::connectTo() // make the connection
{
    if(_ctx)
    {
        _lastError = modbus_connect(handle());
        return _lastError == 0;
    }
    return false;
}





