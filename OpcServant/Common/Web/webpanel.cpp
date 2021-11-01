#include "webpanel.h"

/*!
 * \brief MRL::WebPanel::WebPanel
 * This is a generic table panel display - shows published values listed in the object's inputs
 */
MRL::WebPanel::WebPanel(unsigned id, int tickTime) :_id(id)
{
    auto layout = std::make_unique<Wt::WFitLayout>();
    auto boxLayout = std::make_unique<Wt::WVBoxLayout>();
    _table = boxLayout->addWidget(std::make_unique<Wt::WTable>());
    _timer = addChild(std::make_unique<Wt::WTimer>());
    _timer->timeout().connect(this, &MRL::WebPanel::timerHandle);
    _timer->setInterval(std::chrono::seconds(tickTime));
    _timer->start();

    //
    Wt::WLength tableHeight(10,Wt::LengthUnit::ViewportWidth );
    Wt::WLength tableWidth(50,Wt::LengthUnit::ViewportWidth );
    //
    Wt::WCssDecorationStyle s;
    Wt::WFont f;
    f.setSize(Wt::FontSize::XXLarge);
    s.setFont(f);
    //
    _table->setDecorationStyle(s);
    _table->setHeaderCount(1);
    _table->setWidth(Wt::WLength("100%"));
    _table->addStyleClass("table form-inline");
    //
    // Now build the table for the associated object
    // input name and value
    setupTable();
    //
    _table->toggleStyleClass("table-bordered", true);
    _table->toggleStyleClass("table-condensed", true);
    //
    _statusLine = boxLayout->addWidget(std::make_unique<Wt::WText>("Status"));
    layout->addItem(std::move(boxLayout));
    setLayout(std::move(layout));
    resize(tableWidth,tableHeight);
    //
}

/*!
 * \brief MRL::WebPanel::setupTable
 */
void MRL::WebPanel::setupTable()
{
    MRL::RtObjectRef &r = object(); // get the object
    if(r)
    {
        _table->elementAt(0, 0)->addNew<Wt::WText>(_TR("Item"));
        _table->elementAt(0, 1)->addNew<Wt::WText>(_TR("Value"));
        MRL::StringVector &l = r->inputs(); // get the list of inputs
        for(size_t i = 0; i < l.size(); i++)
        {
            _table->elementAt(i+1, 0)->addNew<Wt::WText>(l[i]);
            _table->elementAt(i+1, 1)->addNew<Wt::WText>(Wt::WString("0.0"));
        }
    }
}

/*!
 * \brief MRL::WebPanel::updateTable
 */
void MRL::WebPanel::updateTable()
{
    MRL::RtObjectRef &r = object(); // get the object
    if(r)
    {
        MRL::StringVector &l = r->inputs(); // get the list of inputs
        for(size_t i = 0; i < l.size(); i++)
        {
            char b[32];
            sprintf(b,"%8.1f",r->getInput<double>(l[i]));
            Wt::WText * w = dynamic_cast<Wt::WText *>(_table->elementAt(i+1,1)->widget(0));
            w->setText(b);
        }
    }
}


/*!
 * \brief MRL::WebPanel::timerHandle
 */
void MRL::WebPanel::timerHandle()
{
    // update the table from the published values
    updateTable();
    wxString t = wxDateTime::Now().FormatISOCombined(' ');
    _statusLine->setText(t.ToStdString());
}



/*!
 * \brief MRL::ControlSetupWeb::SetpointsLimits::SetpointsLimits
 * \param id
 * \param c
 */
MRL::ScaleOffsetWebPanel::ScaleOffsetWebPanel( int numberRows)
    : _numberRows(numberRows),_labels(numberRows),_scales(numberRows),_offsets(numberRows)
{
    auto layout = std::make_unique<Wt::WFitLayout>();
    _table = layout->addWidget(std::make_unique<Wt::WTable>());
    Wt::WLength w("8em");
    //
    //
    _table->insertColumn(0);
    _table->insertColumn(1);
    _table->insertColumn(2);
    //
    _table->columnAt(0)->setWidth(w);
    _table->columnAt(1)->setWidth(w);
    _table->columnAt(2)->setWidth(w);
    // set up
    _table->setHeaderCount(1,Wt::Orientation::Horizontal);
    _table->addStyleClass(SCROLL_TABLE_CSS);

    _table->elementAt(0, 0)->addNew<Wt::WText>(_TR("Item"));
    _table->elementAt(0, 1)->addNew<Wt::WText>(_TR("Scale"));
    _table->elementAt(0, 2)->addNew<Wt::WText>(_TR("Offset"));
    //
    for(int i = 1; i <= _numberRows; i++)
    {
        int j = i - 1;
        _labels[j]  = _table->elementAt(i, 1)->addNew<Wt::WLineEdit>(Wt::WString(std::to_string(i)));
        _scales[j]  = _table->elementAt(i, 2)->addNew<Wt::WLineEdit>(Wt::WString("1.0"));
        _offsets[j] = _table->elementAt(i, 3)->addNew<Wt::WLineEdit>(Wt::WString("0"));
    }
    setLayout(std::move(layout));
}

MRL::ScaleOffsetWebPanel::ScaleOffsetWebPanel(MRL::StringVector &l) : ScaleOffsetWebPanel(l.size())
{
    for(int i = 0; i < l.size(); i++)
    {
        map()[l[i]] = ScaleOffset(1.0,0.0); // setup the map
    }
    setLabels();
}

/*!
 * \brief MRL::ScaleOffsetWebPanel::setLabels
 * \param l
 */
void MRL::ScaleOffsetWebPanel::setLabels()
{
    if(_map.size() <= _numberRows)
    {
        int j = 0;
        for(auto i = _map.begin(); i != _map.end(); i++)
        {
            _labels[j++]->setText(i->first);
        }
    }
}
/*!
 * \brief MRL::ControlSetupWeb::SetpointsLimits::read
 * Read from configuration
 */
void MRL::ScaleOffsetWebPanel::read(VariantPropertyTree &c,MRL::PropertyPath &path)
{
    _map.read(c,path);
    int j = 0;
    for(auto i = _map.begin(); i != _map.end(); i++, j++)
    {
        ScaleOffset &o = i->second;
        _scales[j]->setText(std::to_string(o.getScale()));
        _offsets[j]->setText(std::to_string(o.getOffset()));
    }
}
/*!
 * \brief MRL::ControlSetupWeb::SetpointsLimits::write
 * Write to configuration
 */
void MRL::ScaleOffsetWebPanel::write(VariantPropertyTree &c,MRL::PropertyPath &path)
{
    int j = 0;
    for(auto i = _map.begin(); i != _map.end(); i++, j++)
    {
        ScaleOffset &o = i->second;
        o.setScale(stringToNumber<double>(_scales[j]->text().toUTF8()));
        o.setOffset(stringToNumber<double>(_offsets[j]->text().toUTF8()));
    }
    _map.write(c,path);
}


/*!
 * \brief MRL::ConfigurationWebPanel::ConfigurationWebPanel
 * \param c
 */
MRL::ConfigurationWebPanel::ConfigurationWebPanel()
{

    auto layout = std::make_unique<Wt::WGridLayout>(); // a bag grid layout
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setHorizontalSpacing(10);
    //
    int row = 0;
    layout->addWidget(std::make_unique<Wt::WLabel>("Measure Interval(s)"), row, 0);
    _measureInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), row++, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Publish Interval(s)"), row, 0);
    _publishInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), row++, 1);
    _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>(_TR("Enabled")),row++,0);
    _enableTabView = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enable Tab View"),row++,0);
    setLayout(std::move(layout));

}

/*!
 * \brief MRL::ConfigurationWebPanel::read
 */
void MRL::ConfigurationWebPanel::read(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    if(c.existsub(p,"MeasureInterval"))
    {
        _measureInterval->setValue(c.getValue<int>("MeasureInterval"));
        _publishInterval->setValue(c.getValue<int>("PublishInterval"));
        _enabled->setChecked(c.getValue<bool>("Enabled"));
        _enableTabView->setChecked(c.getValue<bool>("EnableTabView"));
    }
    else
    {
        _measureInterval->setValue(15);
        _publishInterval->setValue(60);
        _enabled->setChecked(true);
        _enableTabView->setChecked(false);
    }
}

/*!
 * \brief MRL::ConfigurationWebPanel::write
 */
void MRL::ConfigurationWebPanel::write(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    c.setValue(p,"MeasureInterval",_measureInterval->value());
    c.setValue(p,"PublishInterval",_publishInterval->value());
    c.setValue(p,"Enabled",_enabled->isChecked());
    c.setValue(p,"EnableTabView",_enabled->isChecked());

}

/*!
 * \brief MRL::ConfigureI2cWebPanel::ConfigureI2cWebPanel
 * \param c
 */

MRL::ConfigureI2cWebPanel::ConfigureI2cWebPanel()
{
    //
    std::unique_ptr<Wt::WGridLayout> grid = std::make_unique<Wt::WGridLayout>();
    Wt::WLength w(20,Wt::LengthUnit::FontEm); // combo box size
    Wt::WLength sw(10,Wt::LengthUnit::FontEm); // spin box size
    //
    int row = 0;
    //
    grid->addWidget(std::make_unique<Wt::WLabel>("I2C Type"),row,0);
    _i2cType = grid->addWidget(std::make_unique<Wt::WComboBox>(),row++,1);
    _i2cType->setMaximumSize(w,_i2cType->maximumHeight());
    _i2cType->addItem(_TR("Local"));
    _i2cType->addItem(_TR("Remote"));

    //
    grid->addWidget(std::make_unique<Wt::WLabel>("I2C Address"),row,0);
    _i2cAddress = grid->addWidget(std::make_unique<Wt::WLineEdit>(),row++,1);
    _i2cDeviceList = grid->addWidget(std::make_unique<Wt::WComboBox>(),row++,1);
    onI2cTypeSet();
    //
    grid->addWidget(std::make_unique<Wt::WLabel>("I2C Port"),row,0);
    _i2cPort = grid->addWidget(std::make_unique<Wt::WSpinBox>(),row++,1);
    _i2cPort->setMaximumSize(sw,_i2cPort->maximumHeight());
    _i2cPort->setMaximum(32000);
    _i2cPort->setMinimum(2000);
    _i2cPort->setValue(5000);
    //
    setLayout(std::move(grid));
}
/*!
 * \brief MRL::ConfigureI2cWebPanel::read
 */
void MRL::ConfigureI2cWebPanel::read(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    // now load the fields
    // get the configuration
    if(c.existsub(p,"I2CType"))
    {
        _i2cType->setCurrentIndex(c.getValue<int>(p,"I2CType"));
        _i2cAddress->setText(c.getValue<std::string>(p,"I2CAddress"));
        _i2cPort->setValue(c.getValue<int>(p,"I2CPort"));
    }
    else
    {
        _i2cType->setCurrentIndex(0);
        _i2cAddress->setText("i2c-1");
        _i2cPort->setValue(5000);
    }
}
/*!
 * \brief MRL::ConfigureI2cWebPanel::write
 */
void MRL::ConfigureI2cWebPanel::write(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    c.setValue(p,"I2CType",_i2cType->currentIndex());
    c.setValue(p,"I2CAddress",_i2cAddress->text().toUTF8());
    c.setValue(p,"I2CPort",_i2cPort->value());
}
/*!
 * \brief MRL::ConfigureI2cWebPanel::onI2cTypeSet
 */
void MRL::ConfigureI2cWebPanel::onI2cTypeSet()
{
    // Local
    _i2cDeviceList->clear();
    // look for i2c
    wxDir d("/dev");
    if(d.IsOpened())
    {
        wxString f;
        if(d.GetFirst (&f, "*i2c*", wxDIR_FILES ))
        {
            do
            {
                std::string s = f.ToStdString();
                _i2cDeviceList->addItem(s);
            } while(d.GetNext(&f));
        }
    }

}

/*!
 * \brief MRL::ConfigurationWebDialog::setup
 */
void MRL::ConfigurationWebDialog::setup()
{
    auto layout = contents()->setLayout(std::make_unique<Wt::WVBoxLayout>());
    // create the pages on collapsable panels
    // create a list of panels
    _panelList =  layout->addWidget(std::make_unique<PanelList>());
    addPanels();
    read(_configuration,_dataPath);
    WebDialogBase::setup();
}

/*!
 * \brief MRL::ConfigurationWebDialog::onOk
 */
void MRL::ConfigurationWebDialog::onOk()
{
    PropertyPath p;
    write(_configuration,_dataPath);
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
}

/*!
 * \brief MRL::ConfigureSerialWebPanel::ConfigureSerialWebPanel
 */
MRL::ConfigureSerialWebPanel::ConfigureSerialWebPanel(bool f) : _hasNode(f)
{
    std::list<std::string> sp;
    MRL::getSerialPortList(sp);

    auto layout = std::make_unique<Wt::WGridLayout>(); // a bag grid layout
    //
    layout->addWidget(std::make_unique<Wt::WLabel>("Port"), 0, 0);
    _port = layout->addWidget(std::make_unique<Wt::WComboBox>(), 0, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Baud Rate"), 1, 0);
    _baudRate = layout->addWidget(std::make_unique<Wt::WComboBox>(), 1, 1);
    //
    if(_hasNode)
    {
        layout->addWidget(std::make_unique<Wt::WLabel>("Node Address"), 2, 0);
        _nodeAddress = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 2, 1);
        _nodeAddress->setMaximum(255);
        _nodeAddress->setMinimum(1);
        _nodeAddress->setValue(1);
    }
    //
    for(auto i = sp.begin(); i != sp.end(); i++)
    {
        _port->addItem(*i);
    }
    //
    _baudRate->addItem("9600");
    _baudRate->addItem("19200");
    _baudRate->addItem("38400");
    _baudRate->addItem("57600");
    _baudRate->addItem("115200");
    setLayout(std::move(layout));
}
/*!
 * \brief MRL::ConfigureSerialWebPanel::read
 * \param c
 */
void MRL::ConfigureSerialWebPanel::read(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    if(c.existsub(p,"NodeAddress"))
    {
        if(_nodeAddress) _nodeAddress->setValue(c.getValue<int>(p,"NodeAddress"));
        int j = _baudRate->findText(c.getValue<std::string>(p,"BaudRate"));
        _baudRate->setCurrentIndex(j);
        j = _port->findText(c.getValue<std::string>(p,"Device"));
        _port->setCurrentIndex(j);
    }
    else
    {
        _baudRate->setCurrentIndex(0);
        _port->setCurrentIndex(0);
    }
}
/*!
 * \brief MRL::ConfigureSerialWebPanel::write
 * \param c
 */
void MRL::ConfigureSerialWebPanel::write(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    c.setValue(p,"Device", _port->currentText().toUTF8());
    c.setValue(p,"BaudRate",_baudRate->currentText().toUTF8());
    if(_nodeAddress) c.setValue(p,"NodeAddress",_nodeAddress->value());
}

/*!
 * \brief MRL::ConfigureDatabaseWebPanel::ConfigureDatabaseWebPanel
 */
MRL::ConfigureDatabaseWebPanel::ConfigureDatabaseWebPanel()
{
    auto layout = std::make_unique<Wt::WGridLayout>(); // a bag grid layout
    layout->addWidget(std::make_unique<Wt::WLabel>("Host"), 0, 0);
    _host = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Database"), 1, 0);
    _database = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 1, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Port (0 = default)"), 2, 0);
    _port = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 2, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Username"), 3, 0);
    _username = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 3, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Password"), 4, 0);
    _password = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 4, 1);
    //
    _port->setMaximum(30000);
    _port->setMinimum(0);
    _port->setValue(0);
    //
    setLayout(std::move(layout));
}
/*!
 * \brief MRL::ConfigureDatabaseWebPanel::read
 * \param c
 */
void MRL::ConfigureDatabaseWebPanel::read(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    if(c.existsub(p,"Host"))
    {
        _host->setText(c.getValue<std::string>(p,"Host"));
        _database->setText(c.getValue<std::string>(p,"Database"));
        _port->setValue(c.getValue<int>("Port"));
        _username->setText(c.getValue<std::string>(p,"Username"));
        _password->setText(c.getValue<std::string>(p,"Password"));
    }
    else
    {
        _host->setText("localhost");
        _database->setText("DATA");
        _port->setValue(0);
        _username->setText("user");
        _password->setText("password");
    }
}
/*!
 * \brief MRL::ConfigureDatabaseWebPanel::write
 * \param c
 */
void MRL::ConfigureDatabaseWebPanel::write(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    c.setValue(p,"Host",_host->text().toUTF8());
    c.setValue(p,"Database",_database->text().toUTF8());
    c.setValue(p,"Port",_port->value());
    c.setValue(p,"Username",_username->text().toUTF8());
    c.setValue(p,"Password",_password->text().toUTF8());
}

/*!
 * \brief MRL::ConfigureNetworkWebPanel::ConfigureNetworkWebPanel
 */
MRL::ConfigureNetworkWebPanel::ConfigureNetworkWebPanel()
{
    auto layout = std::make_unique<Wt::WGridLayout>(); // a bag grid layout
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Host")), 0, 0);
    _address = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Port")), 2, 0);
    _port = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 2, 1);
    //
    _port->setMaximum(30000);
    _port->setMinimum(0);
    _port->setValue(0);
    //
    setLayout(std::move(layout));

}
/*!
 * \brief MRL::ConfigureNetworkWebPanel::read
 * \param c
 */
void MRL::ConfigureNetworkWebPanel::read(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    _address->setText(c.getValue<std::string>(p,"Address"));
    _port->setValue(c.getValue<int>(p,"Port"));
}
/*!
 * \brief MRL::ConfigureNetworkWebPanel::write
 * \param c
 */
void MRL::ConfigureNetworkWebPanel::write(VariantPropertyTree &c,MRL::PropertyPath &p)
{
    c.setValue(p,"Address",_address->text().toUTF8());
    c.setValue(p,"Port",_port->value());
}


