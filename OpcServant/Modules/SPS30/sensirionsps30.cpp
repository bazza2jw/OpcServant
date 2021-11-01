#include "sensirionsps30.h"
#include "Web/aqdatamodel.h"


#ifndef USE_NATIVE
    static MRL::I2cInterfaceBase * i2cIf = nullptr;
#endif

/*!
 * \brief MRL::SensirionSPS30::SensirionSPS30
 * \param parent
 * \param slotId
 */
MRL::SensirionSPS30::SensirionSPS30(unsigned id)
    : MRL::RTObject(id)
{

}




/*!
 * \brief MRL::ElectochemCell::measure
 */
void MRL::SensirionSPS30::measure()
{
    MRL::RTObject::measure();
        updateValue("PM1.0",_m.mc_1p0);
        updateValue("PM2.5",_m.mc_2p5);
        updateValue("PM4.0",_m.mc_4p0);
        updateValue("PM10.0",_m.mc_10p0);
        updateValue("0.5um",_m.nc_0p5);
        updateValue("1.0um",_m.nc_1p0);
        updateValue("2.5um",_m.nc_2p5);
        updateValue("4.0um",_m.nc_4p0);
        updateValue("10.0um",_m.nc_10p0);
}
/*!
 * \brief MRL::ElectochemCell::publish
 */
void MRL::SensirionSPS30::publish()
{
    // publish each of the values
        publishValue("PM1.0",_m.mc_1p0);
        publishValue("PM2.5",_m.mc_2p5);
        publishValue("PM4.0",_m.mc_4p0);
        publishValue("PM10.0",_m.mc_10p0);
        publishValue("0.5um",_m.nc_0p5);
        publishValue("1.0um",_m.nc_1p0);
        publishValue("2.5um",_m.nc_2p5);
        publishValue("4.0um",_m.nc_4p0);
        publishValue("10.0um",_m.nc_10p0);
}
/*!
 * \brief MRL::ElectochemCell::initialiseOpc
 */
void MRL::SensirionSPS30::initialiseOpc(OpcServer &server)
{
    MRL::AQSystemObject::initialiseOpc(server);
    Open62541::UANodeTree &t = parent().tree();
    Open62541::UAPath p;
    Open62541::Variant v(double(0.0));
    p.push_back(slotId());
    t.setNodeValue(p,"PM1.0",v);
    t.setNodeValue(p,"PM2.5",v);
    t.setNodeValue(p,"PM4.0",v);
    t.setNodeValue(p,"PM10.0",v);
    t.setNodeValue(p,"0.5um",v);
    t.setNodeValue(p,"1.0um",v);
    t.setNodeValue(p,"2.5um",v);
    t.setNodeValue(p,"4.0um",v);
    t.setNodeValue(p,"10.0um",v);
}
/*!
 * \brief MRL::ElectochemCell::updateOpc
 */
void MRL::SensirionSPS30::updateOpc(OpcServer &/*server*/)
{
    if(ok())
    {
        Open62541::UANodeTree &t = parent().tree();
        Open62541::UAPath p;
        p.push_back(slotId());
        Open62541::Variant mc_1p0(_m.mc_1p0);
        t.setNodeValue(p,"PM1.0",mc_1p0);
        Open62541::Variant mc_2p5(_m.mc_2p5);
        t.setNodeValue(p,"PM2.5",mc_2p5);
        Open62541::Variant  mc_4p0(_m.mc_4p0);
        t.setNodeValue(p,"PM4.0",mc_4p0);
        Open62541::Variant mc_10p0(_m.mc_10p0);
        t.setNodeValue(p,"PM10.0",mc_10p0);
        Open62541::Variant  nc_0p5(_m.nc_0p5);
        t.setNodeValue(p,"0.5um",nc_0p5);
        Open62541::Variant nc_1p0(_m.nc_1p0);
        t.setNodeValue(p,"1.0um",nc_1p0);
        Open62541::Variant nc_2p5(_m.nc_2p5);
        t.setNodeValue(p,"2.5um",nc_2p5);
        Open62541::Variant nc_4p0(_m.nc_4p0);
        t.setNodeValue(p,"4.0um",nc_4p0);
        Open62541::Variant nc_10p0(_m.nc_10p0);
        t.setNodeValue(p,"10.0um",nc_10p0);
   }
}


/*!
  * \brief MRL::SensirionSPS30::process
  */
void MRL::SensirionSPS30::process()
{
    AQSystemObject::process();
#ifndef USE_NATIVE
    i2cIf = parent().i2cRef();
#endif
    sensirion_i2c_init(); // open I2C channel
    switch(_state)
    {
    case STATE_IDLE:
        break;
    case STATE_RESET:
        if(!sps30_probe())
        {
            if(sps30_start_measurement() == 0)
            {
                _state = STATE_WAIT;
                _waitTime = 0;
            }
        }
        else
        {
            wxLogDebug("AQSystem: Failed to initialise SPS30");
        }
        break;
    case STATE_WAIT:
    {
        _waitTime++;
        if(_waitTime > 2) _state = STATE_MEASURE;
    }
    break;
    case STATE_MEASURE:
    {
        int ret = sps30_read_measurement(&_m);
        if (ret < 0) {
            wxLogDebug("error reading measurement\n");
            _state = STATE_FAIL;

        } else {
            setOk(true);
            _state = STATE_WAIT;
            _waitTime = 0;
        }
    }
    break;
    case STATE_FAIL:
        _state = STATE_RESET;
        break;

    default:
        _state = STATE_FAIL;
        setOk(false);
        break;
    }

    sensirion_i2c_release();
}
/*!
 * \brief MRL::SensirionSPS30::start
 */
void MRL::SensirionSPS30::start()
{
    AQSystemObject::start();
    _state = STATE_RESET;
}
/*!
 * \brief MRL::SensirionSPS30::stop
 */
void MRL::SensirionSPS30::stop()
{
    AQSystemObject::stop();
    _state = STATE_IDLE;
}

/*!
 * \brief MRL::SensirionSPS30::constructModel
 */
void MRL::SensirionSPS30::constructModel(AQDataModel &m) // create the web data model(s) for this object type
{
    m.pages()[dbId()] = std::make_shared<Wt::WStandardItemModel>(9,2);
    std::shared_ptr<Wt::WStandardItemModel> &p = m.pages()[dbId()];
    p->setHeaderData(1,Wt::Orientation::Horizontal,"Value",Wt::ItemDataRole::Display);
    //
    p->setItem(0,0,std::make_unique<Wt::WStandardItem>("PM1.0"));
    p->setItem(1,0,std::make_unique<Wt::WStandardItem>("PM2.5"));
    p->setItem(2,0,std::make_unique<Wt::WStandardItem>("PM4.0"));
    p->setItem(3,0,std::make_unique<Wt::WStandardItem>("PM10.0"));
    p->setItem(4,0,std::make_unique<Wt::WStandardItem>("0.5um"));
    p->setItem(5,0,std::make_unique<Wt::WStandardItem>("1.0um"));
    p->setItem(6,0,std::make_unique<Wt::WStandardItem>("2.5um"));
    p->setItem(7,0,std::make_unique<Wt::WStandardItem>("4.0um"));
    p->setItem(8,0,std::make_unique<Wt::WStandardItem>("10.0um"));
    //
    for(int i = 0; i < 9; i++)
        p->setItem(i,1,std::make_unique<Wt::WStandardItem>("Waiting"));
    //
}

/*!
 * \brief MRL::SensirionSPS30::setValue
 * \param r
 * \param v
 */
void MRL::SensirionSPS30::setValue(Wt::WStandardItem *f , double v)
{
    if(f)
    {
        char b[32];
        sprintf(b,"%8.1f",v);
        f->setText(b);
    }
}

/*!
 * \brief MRL::SensirionSPS30::updateModel
 */
void MRL::SensirionSPS30::updateModel(AQDataModel &m) // update the web data models
{
    std::shared_ptr<Wt::WStandardItemModel> &p = m.pages()[dbId()];
    setValue(p->item(0,1),_m.mc_1p0);
    setValue(p->item(1,1),_m.mc_2p5);
    setValue(p->item(2,1),_m.mc_4p0);
    setValue(p->item(3,1),_m.mc_10p0);
    setValue(p->item(4,1),_m.nc_0p5);
    setValue(p->item(5,1),_m.nc_1p0);
    setValue(p->item(6,1),_m.nc_2p5);
    setValue(p->item(7,1),_m.nc_4p0);
    setValue(p->item(8,1),_m.nc_10p0);
}

//
//
//
extern "C"
{
#ifndef USE_NATIVE
    #define I2C_WRITE_FAILED -1
    #define I2C_READ_FAILED -1

    /**
     * Initialize all hard- and software components that are needed for the I2C
     * communication.
     */
    void sensirion_i2c_init() {
        i2cIf->start(0x69);
        return; /* no error handling */
    }

    /**
     * Release all resources initialized by sensirion_i2c_init().
     */
    void sensirion_i2c_release(void) {
        if(i2cIf)
        {
            i2cIf->end();
        }
        i2cIf = nullptr;
    }

    /**
     * Execute one read transaction on the I2C bus, reading a given number of bytes.
     * If the device does not acknowledge the read command, an error shall be
     * returned.
     *
     * @param address 7-bit I2C address to read from
     * @param data    pointer to the buffer where the data is to be stored
     * @param count   number of bytes to read from I2C and store in the buffer
     * @returns 0 on success, error code otherwise
     */
    int8_t sensirion_i2c_read(uint8_t /*address*/, uint8_t *data, uint16_t count) {
        if(i2cIf)
        {
            if(!i2cIf->blockRead(data, count))
            {
                return I2C_READ_FAILED;
            }
        }
        return 0;
    }

    /**
     * Execute one write transaction on the I2C bus, sending a given number of
     * bytes. The bytes in the supplied buffer must be sent to the given address. If
     * the slave device does not acknowledge any of the bytes, an error shall be
     * returned.
     *
     * @param address 7-bit I2C address to write to
     * @param data    pointer to the buffer containing the data to write
     * @param count   number of bytes to read from the buffer and send over I2C
     * @returns 0 on success, error code otherwise
     */
    int8_t sensirion_i2c_write(uint8_t /*address*/, const uint8_t *data, uint16_t count) {
        if(i2cIf)
        {
            if(!i2cIf->blockWrite(data,count))
            {
                return I2C_WRITE_FAILED;
            }
        }
        return 0;
    }


    /**
     * Sleep for a given number of microseconds. The function should delay the
     * execution for at least the given time, but may also sleep longer.
     *
     * @param useconds the sleep time in microseconds
     */
    void sensirion_sleep_usec(uint32_t useconds) {
        usleep(useconds);
    }
#endif
}





