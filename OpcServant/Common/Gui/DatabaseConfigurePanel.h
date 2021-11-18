#ifndef DATABASECONFIGUREPANEL_H
#define DATABASECONFIGUREPANEL_H
#include "BourbonCommonUI.h"
#include <Common/common.h>

/*!
 * \brief The DatabaseConfigurePanel class
 * Configure a MySQL / MariaDB database configuration
 */
class DatabaseConfigurePanel : public DatabaseConfigurePanelBase
{
public:
    DatabaseConfigurePanel(wxWindow* parent);
    virtual ~DatabaseConfigurePanel();
    /*!
     * \brief read
     * \param c
     */
    virtual void read( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        GetHost()->SetValue(c.getValue<std::string>(p,"Host"));
        GetDatabase()->SetValue(c.getValue<std::string>(p,"Database"));
        GetPort()->SetValue(c.getValue<int>(p,"Port"));
        GetUsername()->SetValue(c.getValue<std::string>(p,"Username"));
        GetPassword()->SetValue(c.getValue<std::string>(p,"Password"));
    }
    /*!
     * \brief write
     * \param c
     */
    virtual void write( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        c.setValue(p,"Host",GetHost()->GetValue().ToStdString());
        c.setValue(p,"Database",GetDatabase()->GetValue().ToStdString());
        c.setValue(p,"Username",GetUsername()->GetValue().ToStdString());
        c.setValue(p,"Password",GetPassword()->GetValue().ToStdString());
        c.setValue(p,"Port",GetPort()->GetValue());
    }
protected:
    virtual void onTest(wxCommandEvent& event);
};
#endif // DATABASECONFIGUREPANEL_H
