#ifndef HARNESSTESTDIALOG_H
#define HARNESSTESTDIALOG_H
#include "HarnessUI.h"
#include "Common/Daq/minp2pserial.h"

class HarnessTestDialog : public HarnessTestDialogBase
{
    std::string _port;
public:
    HarnessTestDialog(wxWindow* parent);
    virtual ~HarnessTestDialog();
protected:
    virtual void onConnect(wxCommandEvent& event);
    virtual void onTick(wxTimerEvent& event);
    virtual void onSend(wxCommandEvent& event);
    virtual void onCancel(wxCommandEvent& event);
    virtual void onOk(wxCommandEvent& event);
    /*!
     * \brief handleDataFrame
     * \param data
     * \param len
     */
    void handleDataFrame(uint8_t const *data, uint8_t len);
    /*!
     * \brief handleNotifyFrame
     * \param data
     * \param len
     */
    void handleNotifyFrame(uint8_t const *data, uint8_t len);
};
#endif // HARNESSTESTDIALOG_H
