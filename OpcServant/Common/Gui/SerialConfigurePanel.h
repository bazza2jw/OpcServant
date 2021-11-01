#ifndef SERIALCONFIGUREPANEL_H
#define SERIALCONFIGUREPANEL_H
#include "BourbonCommonUI.h"
#include <Common/common.h>
/*!
 * \brief The SerialConfigurePanel class
 */
class SerialConfigurePanel : public SerialConfigurePanelBase
{
public:
    SerialConfigurePanel(wxWindow* parent);
    virtual ~SerialConfigurePanel(){}
    /*!
     * \brief read
     * \param c
     */
    virtual void read( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        MRL::SetChoice(GetDevice(), c.getValue<std::string>(p,"Device"));
        MRL::SetChoice(GetBaudrate(),c.getValue<std::string>(p,"BaudRate"));
        GetNodeAddress()->SetValue(c.getValue<int>(p,"NodeAddress"));
    }
    /*!
     * \brief write
     * \param c
     */
    virtual void write( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        c.setValue(p, "Device", MRL::GetChoice(GetDevice()));
        c.setValue(p, "BaudRate", MRL::GetChoice(GetBaudrate()));
        c.setValue(p, "NodeAdress", GetNodeAddress()->GetValue());
    }
    /*!
     * \brief showNodeAddress
     */
    void showNodeAddress()
    {
        GetNodeAddress()->Show();
        m_nodeLabel->Show();

    }
};
#endif // SERIALCONFIGUREPANEL_H
