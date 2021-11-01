#ifndef SENSIRIONSPS30_H
#define SENSIRIONSPS30_H
#include <Common/Daq/rtobject.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#include <Common/Daq/rtobject.h>
#include <open62541cpp/servernodetree.h>
#include <wx/stopwatch.h>
#include "sps30/sps30.h"
//
// This class uses the I2C but has its own interface functions
//
namespace MRL {
class SensirionSPS30 : public MRL::RTObject
{
    enum
    {
       STATE_IDLE = 0,
       STATE_RESET,
       STATE_WAIT,
       STATE_MEASURE,
       STATE_FAIL
    };
    int _state = 0;
    int _waitTime = 0;
    struct sps30_measurement _m;

public:
    SensirionSPS30(unsigned id);
    virtual void process();
    virtual void start();
    virtual void stop();
    virtual int type() {
        return MODULEID_SPS30;
    }
    /*!
     * \brief slaveAddress
     * \return
     */
    int slaveAddress() {
        return SPS30_I2C_ADDRESS;
    }
    /*!
     * \brief setInputs
     * \param l
     */
    virtual void setInputs(StringVector &l)
    {
        l.push_back("pm1");
        l.push_back("pm2,5");
        l.push_back("pm4");
        l.push_back("pm10");
        l.push_back("nc0,5");
        l.push_back("nc1,0");
        l.push_back("nc2,5");
        l.push_back("nc4,5");
        l.push_back("nc10,0");
        l.push_back("typicalParticleSize");
    } // set the available inputs


    virtual void measure();
    virtual void publish();
    virtual void initialiseOpc(OpcServer &/*server*/);
    virtual void updateOpc(OpcServer &/*server*/);
    virtual void constructModel(AQDataModel &); // create the web data model(s) for this object type
    virtual void updateModel(AQDataModel &); // update the web data models
    void setValue(Wt::WStandardItem *f, double v);
};
}
#endif // SENSIRIONSPS30_H
