#ifndef DATASOURCEMQTTRT_H
#define DATASOURCEMQTTRT_H

#include <mosquittopp.h>
#include <Common/Daq/rtobject.h>
#include <Common/MQTT/mqttconnection.h>
namespace MRL {
    /*!
     * \brief The DataSourceMqttRt class
     * MQTT data source - this maps to a broker
     * Add
     */
    class DataSourceMqttRt : public RTObject {

        public:
            DataSourceMqttRt(int id);
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
#endif // DATASOURCEMQTTRT_H
