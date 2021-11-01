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
#ifndef DAQCOMMON_H
#define DAQCOMMON_H
#include <Common/Daq/rtobject.h>
#include <Common/messages.h>
#include <Common/Daq/serialpacket.h>

namespace MRL
{

    typedef std::map<unsigned, RtObjectRef  >RtObjectMap;
    typedef std::map<std::string,SerialPacketRef> SerialPacketMap;
    typedef std::map<unsigned, wxWindow *> RtWindowMap; // open views - views must delete entry on close
    typedef std::map<unsigned, wxFrame *> RtFrameWindowMap; // open views - views must delete entry on close


    /*!
     * \brief The DaqCommon class
     * common data for the DAQ thread
     */
    class DaqCommon
    {
        RtObjectMap _objects;
        SerialPacketMap _serials;
        RtFrameWindowMap _topWindows;
        RtWindowMap _tabWindows;



    public:
        /*!
         * \brief DaqCommon
         */
        DaqCommon()
        {
            std::string d = Common::baseDir() + "/data";

        }

        /*!
         * \brief objects
         * \return
         */
        RtObjectMap & objects() { return _objects;}

        /*!
         * \brief serials
         * \return
         */
        SerialPacketMap &serials() { return _serials;}

        /*!
         * \brief serial
         * \param s
         * \return
         */
        SerialPacketRef &serial(const std::string &s)
        {
            if(serials().find(s) == _serials.end())
            {
                SerialPacketRef r(new SerialPacket());
                serials()[s] = std::move(r);
            }
            return serials()[s];
        }

        /*!
         * \brief windows
         * \return  the object-window map
         */
        RtFrameWindowMap & topWindows()
        {
            return _topWindows;
        }

        /*!
         * \brief tabWindows
         * \return
         */
        RtWindowMap & tabWindows()
        {
            return _tabWindows;
        }
   };
}
#endif // DAQCOMMON_H
