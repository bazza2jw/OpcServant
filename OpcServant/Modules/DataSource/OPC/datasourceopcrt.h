#ifndef DATASOURCEOPCRT_H
#define DATASOURCEOPCRT_H

#include <open62541cpp/open62541client.h>
#include <open62541cpp/monitoreditem.h>
#include <open62541cpp/clientsubscription.h>

#include <Common/Daq/rtobject.h>

namespace MRL {
/*!
     * \brief The DataSourceOpcRt class
     * OPC data sources
     */
    class DataSourceOpcRt : public RTObject {

        public:
            DataSourceOpcRt(int id);
            /*!
                \brief start
            */
            virtual void start();
            /*!
                \brief stop
            */
            virtual void stop();

            virtual bool useProcessTimer() {
                return true;   // used to drive state machines
            }
            /*!
                \brief process
            */
            virtual void process();
            /*!
                \brief onOneSecond
                \param t
            */
            virtual void onOneSecond(time_t t);
    };
}

#endif // DATASOURCEOPCRT_H
