#include "bme280.h"

/*!
 * \brief MRL::Bme280
 * \param parent
 * \param slotId
 */
MRL::Bme280::Bme280(unsigned id)
    : MRL::RTObject(id)
{

}
/*!0xD0
 * \brief MRL::Bme280::process
 */
void MRL::Bme280::process()
{

    if(i2c().start(_bmeAddr))
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
            if (parent().i2c().read( 0xD0, t_id)) {
                if (t_id != 0x60) {
                    wxLogDebug("BME 280 Not Found");
                    _state = STATE_FAULT;
                } else {
                    initialiseBme280();
                    _state = STATE_READ_CALIB;
                }
            }
        }

        break;
        case STATE_READ_CALIB:
        {
            // configure the device
            if (!parent().i2c().write( 0xF5, (uint8_t)0x04)) {
                _state = STATE_FAULT;
                setOk(false);
            }
            else
            {
                parent().i2c().write( 0xF2, (uint8_t)5);
                parent().i2c().write(0xF4, (uint8_t)0xB4);
                readBmeCal();
                _state = STATE_TRIGGER;
                setOk(true);
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
        parent().i2c().end();
    }
}
/*!
 * \brief MRL::Bme280::start
 */
void MRL::Bme280::start()
{
    AQSystemObject::start();
    _state = STATE_RESET;
}
/*!
 * \brief MRL::Bme280::stop
 */
void MRL::Bme280::stop()
{
    AQSystemObject::stop();
    _state = STATE_IDLE;
}

/*!
 * \brief MRL::Bme280::readBmeCal
 */
void MRL::Bme280::readBmeCal() {
    uint8_t h, l;
    parent().i2c().read( 0x88, calib_data.dig_T1);
    parent().i2c().read( 0x8A,  calib_data.dig_T2);
    parent().i2c().read( 0x8C, calib_data.dig_T3);
    parent().i2c().read(0x8E, calib_data.dig_P1);
    parent().i2c().read(0x90,  calib_data.dig_P2);
    parent().i2c().read(0x92,  calib_data.dig_P3);
    parent().i2c().read( 0x94, calib_data.dig_P4);
    parent().i2c().read( 0x96,  calib_data.dig_P5);
    parent().i2c().read( 0x98,  calib_data.dig_P6);
    parent().i2c().read( 0x9A,  calib_data.dig_P7);
    parent().i2c().read( 0x9C,  calib_data.dig_P8);
    parent().i2c().read( 0x9E,  calib_data.dig_P9);
    parent().i2c().read(0xA1, calib_data.dig_H1);
    parent().i2c().read( 0xE1,  calib_data.dig_H2);
    parent().i2c().read(0xE3,calib_data.dig_H3);
    l = h = 0;
    parent().i2c().read(0xE4,  l);
    parent().i2c().read(0xE5,  h);
    calib_data.dig_H4 = (l << 4) | (h & 0x7);
    l = h = 0;
    parent().i2c().read(0xE5,  l);
    parent().i2c().read(0xE6,  h);
    calib_data.dig_H5 = (l & 0x7) | (h << 4);
    parent().i2c().read(0xE7,  calib_data.dig_H6);
}

/*!
 * \brief MRL::Bme280::initialiseBme280
 */

void MRL::Bme280::initialiseBme280() {
    uint8_t id = 0;
    if (parent().i2c().read(0xD0, id)) {
        if (id != 0x60) {
            wxLogDebug("BME 280 Not Found");
        } else {
            parent().i2c().write(0xE0, (uint8_t)0xB6); // soft reset
            //
            // sleep for 50ms
            wxSleep(1);
            //
            // configure the device
            if (!parent().i2c().write( 0xF5, (uint8_t)0x04)) {
                wxLogDebug("F5 Fail");
                return;
            }
            parent().i2c().write(0xF2, (uint8_t)5);
            parent().i2c().write(0xF4, (uint8_t)0xB4);
            //
            readBmeCal();
            wxLogDebug("BME 280 Initialise Complete");
        }
    }
}
/*!
 * \brief MRL::Bme280::compensate_pressure
 * \param P
 * \return compensated pressure
 */
double MRL::Bme280::compensate_pressure(double P) {
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
 * \brief MRL::Bme280::compensate_temperature
 * \param T
 * \return
 */
double MRL::Bme280::compensate_temperature(double T) {
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
 * \brief MRL::Bme280::compensate_humidity
 * \param H
 * \return
 */
double MRL::Bme280::compensate_humidity(double H) {
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
 * \brief MRL::Bme280::trigger
 */
void MRL::Bme280::trigger()
{
    parent().i2c().write( 0xF4, (uint8_t)0xB7); // trigger a single measure
}

/*!
 * \brief MRL::Bme280::readBme280
 * \param t
 * \param p
 * \param h
 */
void MRL::Bme280::readBme280(double &t, double &p, double &h) {

    double H, T, P;
    bool f = true;
    union {
        long l;
        uint8_t b[4];
    } d;
    d.l = 0;
    f &= parent().i2c().read( 0xFE, d.b[0]);
    f &= parent().i2c().read( 0xFD, d.b[1]);
    H = d.l;
    d.l = 0;
    f &= parent().i2c().read(0xFC, d.b[0]);
    f &= parent().i2c().read( 0xFB, d.b[1]);
    f &= parent().i2c().read(0xFA, d.b[2]);
    d.l >>= 4;
    T = d.l;
    d.l = 0;
    f &= parent().i2c().read(0xF9, d.b[0]);
    f &= parent().i2c().read(0xF8, d.b[1]);
    f &= parent().i2c().read(0xF7, d.b[2]);
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
    setOk(f);
}

/*!
 * \brief MRL::Bme280::measure
 */
void MRL::Bme280::measure()
{
    // generate the measure record
    MRL::AQSystemObject::measure();
    if(ok())
    {
        updateValue("Temperature",temperature());
        updateValue("Pressure",pressure());
        updateValue("Humidity",humidity());
        //
    }
}
/*!
 * \brief MRL::Bme280::publish
 */
void MRL::Bme280::publish()
{
    if(ok())
    {
        publishValue("Temperature",temperature());
        publishValue("Pressure", pressure());
        publishValue("Humidity",humidity());
    }
}
/*!
 * \brief MRL::Bme280::initialiseOpc
 */
void MRL::Bme280::initialiseOpc(OpcServer &server)
{
    // add nodes to the tree - the instance creates the nodes for T/P/RH
    // can only have one BME280 present
    MRL::AQSystemObject::initialiseOpc(server);
    Open62541::UANodeTree &t = parent().tree();
    Open62541::UAPath p;
    p.push_back(slotId());
    Open62541::Variant v(double(0.0));
    t.setNodeValue(p,"Temperature",v);
    t.setNodeValue(p,"Pressure",v);
    t.setNodeValue(p,"Humidity",v);
}
/*!
 * \brief MRL::Bme280::updateOpc
 */
void MRL::Bme280::updateOpc(OpcServer &server)
{
    MRL::RTObject::updateOpc(server);
        // update the nodes
        // get the tree
        Open62541::UANodeTree &t = parent().tree();
        // get the node ids
        //
        Open62541::Variant vt(_temperature);
        Open62541::Variant vp(_pressure);
        Open62541::Variant vh(_humidity);
        //
        Open62541::UAPath p;
        //
        t.setNodeValue(p,"Temperature",vt);
        t.setNodeValue(p,"Pressure",vp);
        t.setNodeValue(p,"Humidity",vh);
}



