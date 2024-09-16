#include "HarnessTestDialog.h"
#include "buiapp.h"
#include <Common/Daq/serial.hpp>

// the
enum
{
    CHAN_GUI = 1, // messages for the GUI object - handled by AerisPanel
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
    int l = 6 + s.size();
    uint8_t b[256];
    MRL::MinP2PSerial::P2PMSG msg;
    msg._op = 1;
    msg._len = s.size();
    msg._id = idCount++;
    memcpy(&msg._payload[0],s.c_str(),s.size());
    MRL::MinP2PSerial::packP2PMsg(msg,b);
    //
    MRL::MinP2PSerial *p = MRL::MinP2PSerial::find(_port);
    if(p)
    {
        p->send_frame(CHAN_DATA,b,l);
    }

}
void HarnessTestDialog::onTick(wxTimerEvent& event)
{
    // drive the MinP2P layer
     MRL::MinP2PSerial::pollAll();

}
void HarnessTestDialog::onConnect(wxCommandEvent& event)
{
    _port = MRL::GetChoice(GetPort());
    MRL::MinP2PSerial::addConnection(_port,false);
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
        p->addCallback(CHAN_DATA,g);
        p->addCallback(CHAN_NOTIFY, f);
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
    MRL::MinP2PSerial::P2PMSG msg;
    MRL::MinP2PSerial::unpackP2PMsg(msg,data);
    GetResponseText()->SetValue(wxString::Format("%02X %08X %02X",msg._op,msg._id,msg._len));
}
/*!
 * \brief handleNotifyFrame
 * \param data
 * \param len
 */
void  HarnessTestDialog::handleNotifyFrame(uint8_t const *data, uint8_t len)
{
    MRL::MinP2PSerial::P2PMSG msg;
    MRL::MinP2PSerial::unpackP2PMsg(msg,data);
    GetNotifyText()->SetValue(wxString::Format("%02X %08X %02X",msg._op,msg._id,msg._len));
}
