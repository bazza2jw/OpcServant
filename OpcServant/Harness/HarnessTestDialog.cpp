#include "HarnessTestDialog.h"
#include "buiapp.h"
#include <Common/Daq/serial.hpp>

// the
enum
{
    CHAN_NONE = 0,
    CHAN_DATA,  // messages for the data object
    CHAN_NOTIFY, // notifications - route to data object for handling
    CHAN_TRACE // trace channel
};




HarnessTestDialog::HarnessTestDialog(wxWindow* parent)
    : HarnessTestDialogBase(parent)
{
    wxArrayString sp;
    MRL::getSerialPortList(sp);
    GetPort()->Set(sp);
}

HarnessTestDialog::~HarnessTestDialog()
{
}

void HarnessTestDialog::onCancel(wxCommandEvent& event)
{
    BuiApp::GetInstance()->Exit();
}
void HarnessTestDialog::onOk(wxCommandEvent& event)
{
    BuiApp::GetInstance()->Exit();
}
void HarnessTestDialog::onSend(wxCommandEvent& event)
{
    static uint32_t idCount = 5;
    std::string s = GetSendText()->GetValue().ToStdString();
    //
//    MRL::MinP2PSerial *p = MRL::MinP2PSerial::find(_port);
//    if(p)
//    {
//        p->send(CHAN_DATA,b,l,1);
//    }

}
void HarnessTestDialog::onTick(wxTimerEvent& event)
{
    // drive the MinP2P layer
     MRL::MinP2PSerial::pollAll();

}
void HarnessTestDialog::onConnect(wxCommandEvent& event)
{
    _port = MRL::GetChoice(GetPort());
    MRL::MinP2PSerial::addConnection(_port);
    MRL::MinP2PSerial *p = MRL::MinP2PSerial::find(_port);
    if(p)
    {
        p->configure(115200);
        // resync the other side
        p->transport_reset(true);
        // define functors
        auto f = [this](uint8_t const *data, uint8_t len) {
            this->handleNotifyFrame(data,len);};
        auto g = [this](uint8_t const *data, uint8_t len) {
            this->handleDataFrame(data,len);
        };
        p->addSession(1);
        p->removeSession(1);
        MRL::Session *s = p->findSession(1);
        if(s)
        {
            s->addCallback(CHAN_DATA,g);
            s->addCallback(CHAN_NOTIFY, f);
        }
    }
    else
    {
        std::cerr << "NULL Object" << std::endl;
    }
}

/*!
 * \brief handleDataFrame
 * \param data
 * \param len
 */
void  HarnessTestDialog::handleDataFrame(uint8_t const *data, uint8_t len)
{
    //GetResponseText()->SetValue(wxString::Format("%02X %08X %02X",msg._op,msg._id,msg._len));
}
/*!
 * \brief handleNotifyFrame
 * \param data
 * \param len
 */
void  HarnessTestDialog::handleNotifyFrame(uint8_t const *data, uint8_t len)
{
   // GetNotifyText()->SetValue(wxString::Format("%02X %08X %02X",msg._op,msg._id,msg._len));
}
