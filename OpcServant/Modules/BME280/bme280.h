#ifndef BME280_H
#define BME280_H
#include <Common/Daq/rtobject.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#include <Common/Daq/rtobject.h>
#include <open62541cpp/servernodetree.h>
#include <wx/stopwatch.h>


namespace MRL
{
/*!
 * \brief The Particles class
 */
class Bme280 : public MRL::RTObject
{
    struct bme280_calib_data {
        /**
         * @ Trim Variables
         */

        /**@{*/
        uint16_t dig_T1;
        int16_t dig_T2;
        int16_t dig_T3;
        uint16_t dig_P1;
        int16_t dig_P2;
        int16_t dig_P3;
        int16_t dig_P4;
        int16_t dig_P5;
        int16_t dig_P6;
        int16_t dig_P7;
        int16_t dig_P8;
        int16_t dig_P9;
        uint8_t dig_H1;
        int16_t dig_H2;
        uint8_t dig_H3;
        int16_t dig_H4;
        int16_t dig_H5;
        int8_t dig_H6;
        int32_t t_fine;

        /**@}*/
    };

    bme280_calib_data calib_data;
    double _temperature = 20;
    double _pressure = 100000;
    double _humidity = 100;
    bool triggered = false;
    int _bmeAddr = 0x76;
    int _state = 0;

    enum
    {
        STATE_IDLE = 0,
        STATE_FAULT,
        STATE_RESET,
        STATE_READ_CALIB,
        STATE_TRIGGER,
        STATE_READ

    };

public:
    Bme280(unsigned id);
    virtual void process();
    virtual void start();
    virtual void stop();
    virtual int type() {
        return MODULEID_BME280;
    }
    double temperature() {
        return _temperature;
    }
    double pressure() {
       return  _pressure;
    }
    double humidity() {
        return _humidity;
    }

    // the slave address
    int slaveAddress() { return _bmeAddr;}

    virtual void setInputs(StringVector &l)
    {
        l.push_back("Temperature");
        l.push_back("Pressure");
        l.push_back("Humidity");
    } // set the available inputs

    virtual void measure() ;
    virtual void publish() ;
    virtual void initialiseOpc(OpcServer &/*server*/);
    virtual void updateOpc(OpcServer &/*server*/);
    virtual void updateModel(AQDataModel &m);


private:
    double compensate_humidity(double H);
    double compensate_temperature(double T);
    double compensate_pressure(double P);
    void readBmeCal();
    void readBme280(double &t, double &p, double &h);
    void initialiseBme280();
    void trigger();
};
}
#endif // BME280_H
