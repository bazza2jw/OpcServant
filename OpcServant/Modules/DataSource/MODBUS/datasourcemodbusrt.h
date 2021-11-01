#ifndef DATASOURCEMODBUSRT_H
#define DATASOURCEMODBUSRT_H


class DataSourceModbusRt
{
public:
    DataSourceModbusRt();
};

#include <MrlLib/modbusClass.h>
#include <Common/Daq/rtobject.h>


namespace MRL {

    class DataSourceModbusRt : public RTObject {

        public:
            DataSourceModbusRt(int id);
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

#endif // DATASOURCEMODBUSRT_H
