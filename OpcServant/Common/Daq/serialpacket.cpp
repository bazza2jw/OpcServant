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
#include "Daq/serialpacket.h"

LibSerial::BaudRate MRL::fromIntBaudRate(int baud)
{
    LibSerial::BaudRate b = LibSerial::BaudRate::BAUD_DEFAULT;
    switch(baud)
    {
    case 0:
        b = LibSerial::BaudRate(B0);
        break;
    case 50:
        b = LibSerial::BaudRate(B50);
        break;
    //
    case 75:
        b = LibSerial::BaudRate(B75);
        break;
    //
    case 110:
        b = LibSerial::BaudRate(B50);
        break;
    //
    case 300:
        b = LibSerial::BaudRate(B300);
        break;
    //
    case 600:
        b = LibSerial::BaudRate(B600);
        break;
    //
    case 1200:
        b = LibSerial::BaudRate(B1200);
        break;
    //
    case 2400:
        b = LibSerial::BaudRate(B2400);
        break;
    //
    case 4800:
        b = LibSerial::BaudRate(B4800);
        break;
    //
    case 9600:
        b = LibSerial::BaudRate(B9600);
        break;
    //
    case 19200:
        b = LibSerial::BaudRate(B19200);
        break;
    //
    case 38400:
        b = LibSerial::BaudRate(B38400);
        break;
    //
    case 57600:
        b = LibSerial::BaudRate(B57600);
        break;
    //
    case 115200:
        b = LibSerial::BaudRate(B115200);
        break;
    //
    default:
        break;
    };
    return b;
}
