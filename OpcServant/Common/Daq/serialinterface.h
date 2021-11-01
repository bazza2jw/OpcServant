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
#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H
#include <Common/common.h>
#include <wx/stopwatch.h>

namespace MRL {

    class SerialInterface {
            std::string _buffer;
            int _timeOut = 100; // 100 ms
            int _start = 0; // start of line character - 0 = any
            int _end = '\r'; // end of line
            bool _locked = false; // has this interface been claimed?
            wxStopWatch _to; // timeout timer

            int _statePacket = 0;
            enum {
                StateNone = 0,
                StateWaitStart,
                StateWaitEnd
            };

        public:

            SerialInterface() {}
            /*!
                \brief locked
                \return
            */
            bool locked() const {
                return _locked;
            }

            bool setLocked(bool f = true) {
                return (_locked = f);
            }

            /*!
                \brief release
            */
            void release() {
                _locked = false;
            }
            /*!
                \brief claim
                \return
            */
            bool claim() {
                if (!locked()) {
                    return setLocked();
                }
                return false;
            }

            /*!
                \brief setStart
                \param c
            */
            void setStart(int c) {
                _start = c;
            }
            /*!
                \brief setEnd
                \param c
            */
            void setEnd(int c) {
                _end = c;
            }
            /*!
                \brief getChar
                \return
            */
            virtual int getChar() {
                return  -1;
            }

            /*!
                \brief gotStart
                \return
            */
            bool gotStart();
            /*!
                \brief gotEnd
                \return
            */
            bool gotEnd();

            bool timedOut() {
                return _to.Time() > _timeOut;
            }

            /*!
                \brief flush
            */
            virtual void flush() {
            }

            /*!
                \brief write
                \param buf
            */
            virtual void writeString(const std::string &) {   }

            /*!
                \brief readLine
                \return
            */
            std::string &readLine();
            /*!
                \brief doPacket
                \param request
                \return
            */
            std::string  doPacket(const std::string &request) {
                // discrete packet transaction
                startPacket(request);
                return readLine();
            }

            /*!
                \brief clearTimeout
            */
            void clearTimeout() {
                _to.Start();
            }

            /*!
                \brief buffer
                \return
            */
            std::string &buffer() {
                return _buffer;
            }
            void clearBuffer() {
                _buffer.clear();
            }

            /*!
                \brief setTimeout
                \param t
            */
            void setTimeout(int t) {
                _timeOut = t;
            }

            /*!
                \brief isOk
                \return
            */
            virtual bool isOk() {
                return false;
            }
            /*!
                \brief reopen
            */
            virtual void reopen() {
            }

            /*!
                \brief closePort
            */
            virtual void closePort() {}

            /*!
                \brief packetDrive
                \return
            */
            bool packetDrive();

            /*!
                \brief startPacket
                \param s
            */
            void startPacket(const std::string &s);
    };
}
#endif // SERIALINTERFACE_H
