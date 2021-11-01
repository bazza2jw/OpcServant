#ifndef WEBPANEL_H
#define WEBPANEL_H

// A set of panels to build a configuration dialog from - use the PanelList to wrap

#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WGridLayout.h>
#include <Wt/WToolBar.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTree.h>
#include <Wt/WIconPair.h>
#include <Wt/WTreeNode.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WPushButton.h>
#include <Wt/WTabWidget.h>
#include <Wt/WFitLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTextArea.h>
#include <Wt/WTimer.h>
#include <Wt/WTable.h>
#include <Wt/WPanel.h>
#include <Wt/WSpinBox.h>
#include <Wt/WCheckBox.h>
#include <MrlLib/mrllib.h>
#include <Common/Web/webdialog.h>
#include <Common/PanelList.h>
#include <Common/Daq/rtobject.h>
#include <Common/Daq/daqcommon.h>
#include <MrlLib/scaleoffset.h>

namespace MRL {
/*!
 * \brief The WebPanel class
 */
class WebPanel : public Wt::WContainerWidget
{
    Wt::WTable * _table = nullptr;
    unsigned _id = 0;
    Wt::WTimer * _timer = nullptr; // drive the updates
    Wt::WText * _statusLine = nullptr;
public:
    WebPanel(unsigned id, int tickTime = 2);
    void timerHandle();
    Wt::WTable * table() {
        return _table;
    }
    MRL::RtObjectRef & object()
    {
        return MRL::Common::daq().objects()[_id];
    }
    virtual void setupTable();
    virtual void updateTable();

};

typedef std::vector<Wt::WLineEdit *> WLineEditArray;
/*!
 * \brief The ScaleOffsetWebPanel class
 */
class ScaleOffsetWebPanel : public Wt::WContainerWidget
{
    unsigned _id = 0;
    int _numberRows = 1;
    Wt::WTable * _table = nullptr;
    WLineEditArray _labels;
    WLineEditArray _scales;
    WLineEditArray _offsets;
    ScaleOffsetMap _map;

public:
    ScaleOffsetWebPanel(int numberRows);
    ScaleOffsetWebPanel(MRL::StringVector &);
    virtual void read(VariantPropertyTree &c,MRL::PropertyPath &);
    virtual void write(VariantPropertyTree &c,MRL::PropertyPath &);

    Wt::WTable * table() const {
        return _table;
    }
    WLineEditArray & scales() {
        return _scales;
    }
    WLineEditArray & offsets() {
        return  _offsets;
    }
    WLineEditArray & labels() {
        return _labels;
    }
    void setLabels();
    ScaleOffsetMap & map() { return _map;}
};

/*!
 * \brief The ConfigurationWebPanel class
 */
class ConfigurationWebPanel : public Wt::WContainerWidget
{
public:
    Wt::WSpinBox  * _publishInterval = nullptr;
    Wt::WSpinBox  * _measureInterval = nullptr;
    Wt::WCheckBox * _enabled = nullptr;
    Wt::WCheckBox * _enableTabView = nullptr;
    ConfigurationWebPanel();
    virtual void read(VariantPropertyTree &c,MRL::PropertyPath &);
    virtual void write(VariantPropertyTree &c,MRL::PropertyPath &);
};

/*!
 * \brief The ConfigureI2cWebPanel class
 */
class ConfigureI2cWebPanel : public Wt::WContainerWidget
{
    // I2C settings
    Wt::WLineEdit * _i2cAddress;
    Wt::WComboBox * _i2cDeviceList; // the I2C address
    Wt::WComboBox * _i2cType;
    Wt::WSpinBox * _i2cPort;

public:
    ConfigureI2cWebPanel();
    void read(VariantPropertyTree &c,MRL::PropertyPath &);
    void write(VariantPropertyTree &c,MRL::PropertyPath &);
    void onI2cTypeSet();
};

/*!
 * \brief The ConfigureSerialWebPanel class
 */
class ConfigureSerialWebPanel : public Wt::WContainerWidget
{
    Wt::WComboBox * _port = nullptr;
    Wt::WComboBox * _baudRate = nullptr;
    Wt::WSpinBox  * _nodeAddress = nullptr;
    bool _hasNode = false;
public:
    ConfigureSerialWebPanel(bool f = false);
    void read(VariantPropertyTree &c,MRL::PropertyPath &);
    void write(VariantPropertyTree &c,MRL::PropertyPath &);
};

/*!
 * \brief The ConfigureDatabaseWebPanel class
 */
class ConfigureDatabaseWebPanel : public Wt::WContainerWidget
{
    Wt::WLineEdit * _host = nullptr;
    Wt::WLineEdit * _database = nullptr;
    Wt::WSpinBox * _port = nullptr;
    Wt::WLineEdit * _username = nullptr;
    Wt::WLineEdit * _password = nullptr;
public:
    ConfigureDatabaseWebPanel();
    void read(VariantPropertyTree &c,MRL::PropertyPath &);
    void write(VariantPropertyTree &c,MRL::PropertyPath &);
};


class ConfigureNetworkWebPanel : public Wt::WContainerWidget
{
    Wt::WLineEdit * _address = nullptr;
    Wt::WSpinBox * _port = nullptr;
public:
    ConfigureNetworkWebPanel();
    void read(VariantPropertyTree &c,MRL::PropertyPath &);
    void write(VariantPropertyTree &c,MRL::PropertyPath &);
};



/*!
 * \brief The ConfigurationWebDialog class
 */
class ConfigurationWebDialog : public WebDialogBase
{
    PropertyPath _path; // object path
    MRL::VariantPropertyTree _configuration; // tree of configuration values
    PanelList * _panelList = nullptr;
    PropertyPath _dataPath; // usuall empty but can be a sub directory in the configuration tree

public:
    ConfigurationWebDialog(const PropertyPath &path) : _path(path)
    {
        Json::Value v;
        if(MRL::Common::configuration().getData(_path,v))
        {
            _configuration.fromJson(v); // load configuration
        }
    }

    template <typename T>
    Wt::WPanel *  addPanel(const std::string &s, std::unique_ptr<T> &w)
    {
        Wt::WPanel *p = _panelList->addWidget(_TR(s), std::move(w));
        p->setMargin(Wt::WLength::Auto, Wt::Side::Left | Wt::Side::Right);
        p->resize(800, Wt::WLength::Auto);
        p->setMargin(20, Wt::Side::Top | Wt::Side::Bottom);
        p->setCollapsible(true);
        return p;
    }

    virtual void addPanels() {} // add panels to dialog
    // the path is a sub directory in the configuration not the object path in the database
    virtual void read(MRL::VariantPropertyTree &,MRL::PropertyPath &) {} // read the panel data - called by setup
    virtual void write(MRL::VariantPropertyTree &,MRL::PropertyPath &) {} // write panel data to the configuration - called by onOK
    void setup(); // setup the dialog fields
    void onOk(); // when OK
    PropertyPath & dataPath() { return _dataPath;} // usually empty but can be a sub directory in the configuration tree
    MRL::VariantPropertyTree & configuration() {
        return  _configuration;
    }
};
/*!
 * \brief The DefaultConfigurationWebDialog class
 */
template <typename T>
class   DefaultConfigurationWebDialogBase : public ConfigurationWebDialog
{

    std::string _title;
    T * _panel = nullptr;

public:
    DefaultConfigurationWebDialogBase(const PropertyPath &path,
                                      const std::string &t = "Setup")
        :  ConfigurationWebDialog(path),_title(t) {}
    virtual void addPanels()
    {
        auto bp =  std::make_unique<T>();
        _panel = bp.get();
        addPanel(_TR(_title),bp);
    } // add panels to dialog
    virtual void read(MRL::VariantPropertyTree &c,MRL::PropertyPath &p)
    {
        _panel->read(c,p);
    } // read the panel data - called by setup
    virtual void write(MRL::VariantPropertyTree &c,MRL::PropertyPath &p)
    {
        _panel->write(c,p);
    } // write panel data to the configuration - called by onOK

    // this may be shadowed - I know
    T * panel() { return _panel;}
    void setPanel(T * p) { _panel = p;}


};


typedef DefaultConfigurationWebDialogBase<ConfigurationWebPanel> DefaultConfigurationWebDialog;

/*!
 * \brief The DefaultConfigurationWeb2Dialog class
 * Base configuration plus one other panel
 */

template <typename T>
class   DefaultConfigurationWeb2Dialog : public DefaultConfigurationWebDialog
{
    std::string _title;
    T * _panel = nullptr;

public:
    DefaultConfigurationWeb2Dialog(const PropertyPath &path, const std::string &t = "Data")
        :  DefaultConfigurationWebDialog(path,"Setup"),_title(t) {}
    virtual void addPanels()
    {
        DefaultConfigurationWebDialog::addPanels();
        auto bp =  std::make_unique<T>();
        _panel = bp.get();
        addPanel(_TR(_title),bp);
    } // add panels to dialog
    virtual void read(MRL::VariantPropertyTree &c,MRL::PropertyPath &p)
    {
        DefaultConfigurationWebDialog::read(c,p);
        _panel->read(c,p);
    } // read the panel data - called by setup
    virtual void write(MRL::VariantPropertyTree &c,MRL::PropertyPath &p)
    {
        DefaultConfigurationWebDialog::write(c,p);
        _panel->write(c,p);
    } // write panel data to the configuration - called by onOK

    T * panel() { return _panel;}
    void setPanel(T * p) { _panel = p;}

};

//
// Some standard configuration dialogs
//
class ConfigurationI2CWebDialog : public  DefaultConfigurationWeb2Dialog<ConfigureI2cWebPanel>
{
public:
    ConfigurationI2CWebDialog(const PropertyPath &path)
        : DefaultConfigurationWeb2Dialog<ConfigureI2cWebPanel>(path,"I2C")
    {

    }
};
class ConfigurationSerialWebDialog : public DefaultConfigurationWeb2Dialog<ConfigureSerialWebPanel>
{
public:
    ConfigurationSerialWebDialog(const PropertyPath &path)
        : DefaultConfigurationWeb2Dialog<ConfigureSerialWebPanel>(path,"Serial")
    {

    }
};
class ConfigurationDatabaseWebDialog : public DefaultConfigurationWeb2Dialog<ConfigureDatabaseWebPanel >
{
public:
    ConfigurationDatabaseWebDialog(const PropertyPath &path)
        : DefaultConfigurationWeb2Dialog<ConfigureDatabaseWebPanel >(path,"Database")
    {

    }
};

class ConfigurationNetworkWebDialog : public DefaultConfigurationWeb2Dialog<ConfigureNetworkWebPanel>
{
public:
    ConfigurationNetworkWebDialog(const PropertyPath &path) :
        DefaultConfigurationWeb2Dialog<ConfigureNetworkWebPanel>(path,"Network") {}

};

class ConfigureSerialNodeWebDialog : public DefaultConfigurationWeb2Dialog<ConfigureSerialWebPanel>
{
    ConfigureSerialWebPanel * _panel = nullptr;

public:
    ConfigureSerialNodeWebDialog(const PropertyPath &path)
        : DefaultConfigurationWeb2Dialog<ConfigureSerialWebPanel>(path)
    {}

    virtual void addPanels()
    {
        DefaultConfigurationWeb2Dialog::addPanels();
        auto bp =  std::make_unique<ConfigureSerialWebPanel>(true);
        _panel = bp.get();
        addPanel(_TR("Serial Node"),bp);
    } // add panels to dialog

    virtual void read(MRL::VariantPropertyTree &c,MRL::PropertyPath &p)
    {
        DefaultConfigurationWeb2Dialog::read(c,p);
        _panel->read(c,p);
    } // read the panel data - called by setup
    virtual void write(MRL::VariantPropertyTree &c,MRL::PropertyPath &p)
    {
        DefaultConfigurationWeb2Dialog::write(c,p);
        _panel->write(c,p);
    } // write panel data to the configuration - called by onOK

};



}
#endif // WEBPANEL_H
