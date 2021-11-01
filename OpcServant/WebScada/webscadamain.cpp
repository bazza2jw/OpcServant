#include "webscadamain.h"
#include "wscadamapwindow.h"
/*!
 * \brief MRL::WebScadaMain::WebScadaMain
 */
MRL::WebScadaMain::WebScadaMain()
{
    auto layout = std::make_unique<Wt::WVBoxLayout>();
    //
    _navigationBar =  layout->addWidget(std::make_unique<Wt::WNavigationBar>());
    _navigationBar->setTitle("Web SCADA");
    //
    layout->addWidget(std::make_unique<Wt::WLabel>("Above"));
    layout->addWidget(std::make_unique<WScadaMapWindow>(),1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Below"));
    // Add menu
    auto m = std::make_unique<Wt::WMenu>();
    //
    // Add menu items and connect signals
    //
    _logout = m->addItem("Logout");
    _navigationBar->addMenu(std::move(m));
    _logout->triggered().connect(this, &MRL::WebScadaMain::onLogout);
    setLayout(std::move(layout));
}


/*!
    \brief MRL::WebScadaMain::onLogout
*/
void MRL::WebScadaMain::onLogout() {
    Wt::StandardButton result = Wt::WMessageBox::show("Confirm", "Logout?",  Wt::StandardButton::Ok | Wt::StandardButton::Cancel);
    if (result == Wt::StandardButton::Ok) {
        // Do logout
    }
}
