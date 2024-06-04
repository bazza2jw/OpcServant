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
#include <Common/Daq/serial.hpp>
#include <Common/Daq/serialinterface.h>

namespace MRL {
    /*!
        \brief The SerialPacket class
    */
    class SerialPacket : public SerialInterface, public SerialConnect {

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
            bool openSerial(const std::string port, int baud, int data = SC_8DATA, int stopbits = SC_1STOP, int parity = SC_NOPARITY) {
                bool ret = false;
                if (!isOpen()) {
                    ret = open(port);
                    if (ret) {
                        ret = configure(baud, data, stopbits, parity);
                    }
                    setLocked(false);
                }
                else {
                    ret = true;
                }
                return ret;
            }


            /*!
                \brief getChar
                \return character or -1
            */
            int getChar() {
                int ret = -1;
                char c = 0;
                if (read(&c, 1) == 1)   ret = int(c);
                return ret;
            }


            /*!
                \brief flush
            */
            void flush() {
                clearBuffer();
                flushReceive();
                flushTransmit();
            }

            /*!
                \brief write
                \param buf
            */
            void writeString(const std::string &buf) {
                write(buf.c_str(), buf.size());
            }



            /*!
                \brief isOk
                \return true if ok
            */
            bool isOk() {
                return isOpen();
            }
            /*!
                \brief reopen
            */
            void reopen() {
                reset();
            }

            /*!
                \brief closePort
            */
            void closePort() {
                SerialConnect::close();
            }
            //
    };

    typedef std::unique_ptr<SerialPacket> SerialPacketRef;
}

#endif // SERIALPACKET_H
