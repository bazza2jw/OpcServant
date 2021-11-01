#ifndef NETWORKCONFIGUREPANEL_H
#define NETWORKCONFIGUREPANEL_H
#include "BourbonCommonUI.h"
#include <Common/common.h>
/*!
 * \brief The NetworkConfigurePanel class
 */
class NetworkConfigurePanel : public NetworkConfigurePanelBase
{
public:
    NetworkConfigurePanel(wxWindow* parent);
    virtual ~NetworkConfigurePanel();
    /*!
     * \brief read
     * \param c
     */
    virtual void read( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        GetAddress()->SetValue(c.getValue<std::string>(p,"Address"));
        GetPort()->SetValue(c.getValue<int>(p, "Port"));

    }
    /*!
     * \brief write
     * \param c
     */
    virtual void write( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
       c.setValue(p, "Address",GetAddress()->GetValue().ToStdString());
       c.setValue(p, "Port",GetPort()->GetValue());
    }
protected:
    virtual void onTest(wxCommandEvent& event);
};
#endif // NETWORKCONFIGUREPANEL_H
