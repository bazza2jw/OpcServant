#include "nodeflowwebconfiguration.h"

/*!
 * \brief MRL::NodeFlowWebConfiguration::setup
 */
void MRL::NodeFlowWebConfiguration::setup()
{
    setWindowTitle("LWS Apex Configuration");
    auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setHorizontalSpacing(10);
    //
    int row = 0;
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Flow File")), row, 0);
    _flowName = layout->addWidget(std::make_unique<Wt::WComboBox>(),row++,1);
    _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>(_TR("Enabled")),row,0);
    //
    // Get the flow list
    //
    MRL::StringList l;
    if(MRL::getFileList(l,Common::baseDir() + "/sets", "*.nfl" ) > 0)
    {
        for(auto i = l.begin(); i != l.end(); i++) _flowName->addItem(*i);
    }

    //
    Json::Value v;
    if (MRL::Common::configuration().getData(_path, v)) {
        std::string s = _configuration.getString("Flow");
        int n = _flowName->findText(s);
        _flowName->setCurrentIndex((n >= 0)?n:0);
        _enabled->setChecked(_configuration.getBool("Enabled"));
    }
    // set up
    WebDialogBase::setup();
}
/*!
 * \brief MRL::NodeFlowWebConfiguration::onOk
 */
void MRL::NodeFlowWebConfiguration::onOk()
{
    _configuration.setBool("Enabled",_enabled->isChecked());
    _configuration.setString("Flow",_flowName->currentText().toUTF8());
    //
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path, v);
}
