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
#ifndef ICPCON_H
#define ICPCON_H
#include <Common/plugin.h>
#include <Common/objectmanager.h>
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "icpcon_global.h"

namespace MRL {
/*!
     * \brief The ICPCON class
     */
    class ICPCON : public Plugin {

        public:
            ICPCON();
            //
            // Global initialise - create the object manager
            virtual void initialise();
            //
            virtual void initialiseOpc();


            /*!
             * \brief makePacket
             * \param fmt
             * \return packet
             */
            static std::string  makePacket( int node, const char *fmt)
            {
                char b[32];
                char cmd[64];
                int l = sprintf(b,fmt,node); // get the module ID
                sprintf(cmd,"%s%02X\r",b,CS(b,l));
                return std::string(cmd);
            }
            /*!
             * \brief CS
             * \param s
             * \param n
             */
            static unsigned CS(const char *s, int n) // calculate checksum
            {
                int cs = 0;
                for(int i = 0; i < n; i++)
                {
                    cs += s[i];
                };
                return cs & 0xFF;
            }

    };




}


#endif // ICPCON_H
