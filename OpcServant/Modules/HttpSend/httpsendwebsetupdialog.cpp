#include "httpsendwebsetupdialog.h"

#include <Wt/WGridLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WCheckBox.h>
/*!
 * \brief MRL::HttpSendSetupDialog::setup
 */
void MRL::HttpSendWebSetupDialog::setup()
{
  auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
  layout->addWidget(std::make_unique<Wt::WLabel>("Host"), 0, 0);
  _host = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
  layout->addWidget(std::make_unique<Wt::WLabel>("Port"), 5, 0);
  _port = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 5, 1);
  layout->addWidget(std::make_unique<Wt::WLabel>("URL"), 0, 0);
  _url = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
  layout->addWidget(std::make_unique<Wt::WLabel>("Filter"), 4, 0);
  _filter = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 4, 1);
  _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enabled"), 6, 1);
  //
  _port->setMaximum(10000);
  _port->setMinimum(10);
  _port->setValue(8080);

  //
  Json::Value v;
  if (MRL::Common::configuration().getData(_path, v)) {
      // do not always have to convert to tree - esp if simple property list
      Json::Value &db = v["Filter"];
      Json::Value &hs = v["URL"];
      Json::Value &ei = v["Enabled"];
      //
      _url->setText(hs.asString());
      _filter->setText(db.asString());
      _enabled->setCheckState(ei.asBool()?Wt::CheckState::Checked:Wt::CheckState::Unchecked);
  }
  else
  {
      _filter->setText("*");
      _enabled->setCheckState(Wt::CheckState::Checked);
  }
  WebDialogBase::setup();
}
/*!
 * \brief MRL::HttpSendWebSetupDialog::onOk
 */
void MRL::HttpSendWebSetupDialog::onOk()
{
    Json::Value v;
    v["Filter"]   = _filter->text().toUTF8();
    v["URL"]       = _url->text().toUTF8();
    v["Enabled"]    = (_enabled->checkState() == Wt::CheckState::Checked);
    MRL::Common::configuration().updateData(_path, v);
}
