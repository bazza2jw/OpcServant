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
#include "bme280rt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>

MRL::StringVector MRL::BME280RT::_inputs = {
    "Temperature", "Pressure", "Humidity"
};

/*!
    \brief start
*/
void MRL::BME280RT::start()
{
    RTObject::start();
    _state = STATE_RESET;
    _i2cType = configuration().getInt("/I2CType");
    _i2cAddress = configuration().getString("/I2CAddress");
    _i2cPort = configuration().getInt("/I2CPort");
    MRL::I2cInterfaceBase::makeI2c(MRL::I2cInterfaceBase::Type(_i2cType),_i2c,_i2cAddress,_i2cPort);
}

/*!
    \brief stop
*/
void MRL::BME280RT::stop()
{
    RTObject::stop();
    _state = STATE_IDLE;
    _i2c.reset();
}

/*!
    \brief publish
*/
void MRL::BME280RT::publish()
{
    RTObject::publish();
    publishValue(temperature(),"Temperature",_ok?STATES::States::StateOk:STATES::States::StateFault);
    publishValue( pressure(),"Pressure",_ok?STATES::States::StateOk:STATES::States::StateFault);
    publishValue(humidity(),"Humidity",_ok?STATES::States::StateOk:STATES::States::StateFault);
}

/*!
    \brief measure
*/
void MRL::BME280RT::measure()
{
    RTObject::measure();
    // The BME280 is for correcting other items
    Common::runTime().setValue("Temperature",temperature());
    Common::runTime().setValue("Pressure",pressure());
    Common::runTime().setValue("Humidity",humidity());
    //
    updateObject();
}

/*!
 * \brief MRL::BME280RT::onOneSecond
 * \param t
 */
void MRL::BME280RT::onOneSecond(time_t t)
{
    if(_i2c)
    {
        RTObject::onOneSecond(t);
        if(_i2c->start(_bmeAddr))
        {
            switch(_state)
            {
            case STATE_IDLE:
                break;
            case STATE_FAULT:
                _state = STATE_RESET;
                break;
            case STATE_RESET:
            {
                uint8_t t_id = 0;
                if (_i2c->read( 0xD0, t_id)) {
                    if (t_id != 0x60) {
                        wxLogDebug("BME 280 Not Found");
                        _ok = false;
                        _state = STATE_FAULT;
                    } else {
                        initialiseBme280();
                        _state = STATE_READ_CALIB;
                        _ok = true;
                    }
                }
            }

            break;
            case STATE_READ_CALIB:
            {
                // configure the device
                if (!_i2c->write( 0xF5, (uint8_t)0x04)) {
                    _state = STATE_FAULT;
                }
                else
                {
                    _i2c->write( 0xF2, (uint8_t)5);
                    _i2c->write(0xF4, (uint8_t)0xB4);
                    readBmeCal();
                    _state = STATE_TRIGGER;
                }
            }

            break;
            case STATE_TRIGGER:
                trigger();
                _state = STATE_READ;
                break;
            case STATE_READ:
                readBme280(_temperature,_pressure,_humidity);
                _state = STATE_TRIGGER;
                break;
            default:
                _state = STATE_IDLE;
                break;
            }
            _i2c->end();
            _ok = true;
        }
        else
        {
            _ok = false;
        }
    }
}


/*!
 * \brief MRL::BME280RT::readBmeCal
 */
void MRL::BME280RT::readBmeCal() {
    if(_i2c)
    {
        uint8_t h, l;
        _i2c->read( 0x88, calib_data.dig_T1);
        _i2c->read( 0x8A,  calib_data.dig_T2);
        _i2c->read( 0x8C, calib_data.dig_T3);
        _i2c->read(0x8E, calib_data.dig_P1);
        _i2c->read(0x90,  calib_data.dig_P2);
        _i2c->read(0x92,  calib_data.dig_P3);
        _i2c->read( 0x94, calib_data.dig_P4);
        _i2c->read( 0x96,  calib_data.dig_P5);
        _i2c->read( 0x98,  calib_data.dig_P6);
        _i2c->read( 0x9A,  calib_data.dig_P7);
        _i2c->read( 0x9C,  calib_data.dig_P8);
        _i2c->read( 0x9E,  calib_data.dig_P9);
        _i2c->read(0xA1, calib_data.dig_H1);
        _i2c->read( 0xE1,  calib_data.dig_H2);
        _i2c->read(0xE3,calib_data.dig_H3);
        l = h = 0;
        _i2c->read(0xE4,  l);
        _i2c->read(0xE5,  h);
        calib_data.dig_H4 = (l << 4) | (h & 0x7);
        l = h = 0;
        _i2c->read(0xE5,  l);
        _i2c->read(0xE6,  h);
        calib_data.dig_H5 = (l & 0x7) | (h << 4);
        _i2c->read(0xE7,  calib_data.dig_H6);
    }
}

/*!
 * \brief MRL::BME280RT::initialiseBme280
 */

void MRL::BME280RT::initialiseBme280() {
    uint8_t id = 0;
    if (_i2c && _i2c->read(0xD0, id)) {
        if (id != 0x60) {
            wxLogDebug("BME 280 Not Found");
        } else {
            _i2c->write(0xE0, (uint8_t)0xB6); // soft reset
            //
            // sleep for 50ms
            wxSleep(1);
            //
            // configure the device
            if (!_i2c->write( 0xF5, (uint8_t)0x04)) {
                wxLogDebug("F5 Fail");
                return;
            }
            _i2c->write(0xF2, (uint8_t)5);
            _i2c->write(0xF4, (uint8_t)0xB4);
            //
            readBmeCal();
            wxLogDebug("BME 280 Initialise Complete");
        }
    }
}
/*!
 * \brief MRL::BME280RT::compensate_pressure
 * \param P
 * \return compensated pressure
 */
double MRL::BME280RT::compensate_pressure(double P) {
    double var1;
    double var2;
    double var3;
    double pressure;
    double pressure_min = 30000.0;
    double pressure_max = 110000.0;

    var1 = ((double) calib_data.t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double) calib_data.dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double) calib_data.dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double) calib_data.dig_P4) * 65536.0);
    var3 = ((double) calib_data.dig_P3) * var1 * var1 / 524288.0;
    var1 = (var3 + ((double) calib_data.dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double) calib_data.dig_P1);

    /* avoid exception caused by division by zero */
    if (var1) {
        pressure = 1048576.0 - P;
        pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
        var1 = ((double) calib_data.dig_P9) * pressure * pressure / 2147483648.0;
        var2 = pressure * ((double) calib_data.dig_P8) / 32768.0;
        pressure = pressure + (var1 + var2 + ((double) calib_data.dig_P7)) / 16.0;
        if (pressure < pressure_min) {
            pressure = pressure_min;
        } else if (pressure > pressure_max) {
            pressure = pressure_max;
        }
    } else { /* Invalid case */
        pressure = pressure_min;
    }

    return pressure;
}

/*!
 * \brief MRL::BME280RT::compensate_temperature
 * \param T
 * \return
 */
double MRL::BME280RT::compensate_temperature(double T) {
    double var1;
    double var2;
    double temperature;
    double temperature_min = -40;
    double temperature_max = 85;

    var1 = T / 16384.0 - ((double) calib_data.dig_T1) / 1024.0;
    var1 = var1 * ((double) calib_data.dig_T2);
    var2 = (T / 131072.0 - ((double) calib_data.dig_T1) / 8192.0);
    var2 = (var2 * var2) * ((double) calib_data.dig_T3);
    calib_data.t_fine = (int32_t) (var1 + var2);
    temperature = (var1 + var2) / 5120.0;
    if (temperature < temperature_min) {
        temperature = temperature_min;
    } else if (temperature > temperature_max) {
        temperature = temperature_max;
    }

    return temperature;
}

/*!
 * \brief MRL::BME280RT::compensate_humidity
 * \param H
 * \return
 */
double MRL::BME280RT::compensate_humidity(double H) {
    double humidity;
    double humidity_min = 0.0;
    double humidity_max = 100.0;
    double var1;
    double var2;
    double var3;
    double var4;
    double var5;
    double var6;

    var1 = ((double) calib_data.t_fine) - 76800.0;
    var2 = (((double) calib_data.dig_H4) * 64.0 + (((double) calib_data.dig_H5) / 16384.0) * var1);
    var3 = H - var2;
    var4 = ((double) calib_data.dig_H2) / 65536.0;
    var5 = (1.0 + (((double) calib_data.dig_H3) / 67108864.0) * var1);
    var6 = 1.0 + (((double) calib_data.dig_H6) / 67108864.0) * var1 * var5;
    var6 = var3 * var4 * (var5 * var6);
    humidity = var6 * (1.0 - ((double) calib_data.dig_H1) * var6 / 524288.0);
    if (humidity > humidity_max) {
        humidity = humidity_max;
    } else if (humidity < humidity_min) {
        humidity = humidity_min;
    }

    return humidity;
}

/*!
 * \brief MRL::BME280RT::trigger
 */
void MRL::BME280RT::trigger()
{
    _i2c->write( 0xF4, (uint8_t)0xB7); // trigger a single measure
}

/*!
 * \brief MRL::BME280RT::readBme280
 * \param t
 * \param p
 * \param h
 */
void MRL::BME280RT::readBme280(double &t, double &p, double &h) {

    double H, T, P;
    bool f = true;
    union {
        long l;
        uint8_t b[4];
    } d;
    d.l = 0;
    f &= _i2c->read( 0xFE, d.b[0]);
    f &= _i2c->read( 0xFD, d.b[1]);
    H = d.l;
    d.l = 0;
    f &= _i2c->read(0xFC, d.b[0]);
    f &= _i2c->read( 0xFB, d.b[1]);
    f &= _i2c->read(0xFA, d.b[2]);
    d.l >>= 4;
    T = d.l;
    d.l = 0;
    f &= _i2c->read(0xF9, d.b[0]);
    f &= _i2c->read(0xF8, d.b[1]);
    f &= _i2c->read(0xF7, d.b[2]);
    d.l >>= 4;
    P = d.l;
    //
    T = compensate_temperature(T);
    P = compensate_pressure(P);
    H = compensate_humidity(H);
    //
    t = T;
    p = P;
    h = H;
}


/*!
 * \brief MRL::BME280RT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::BME280RT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/)
{
    Open62541::Variant t(temperature());
    // find the child node called Value
    server.writeValue(_temperatureNode,t);
    Open62541::Variant p(pressure());
    // find the child node called Value
    server.writeValue(_pressureNode,p);
    Open62541::Variant h(humidity());
    // find the child node called Value
    server.writeValue(_humidityNode,h);
}


/*!
 * \brief MRL::BME280RT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::BME280RT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
    server.getChild(node,"Temperature",_temperatureNode);
    server.getChild(node,"Pressure",_pressureNode);
    server.getChild(node,"Humidity",_humidityNode);
}


/*!
 * \brief publishHtmlPage
 */
void MRL::BME280RT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
    //
}

/*!
 * \brief measureHtmlPage
 */
void MRL::BME280RT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
}
