#include "webscadastatus.h"

MRL::StandardItemModelMap MRL::WebScadaStatus::_model; // map of all status table models
/*!
 * \brief MRL::WebScadaStatus::WebScadaStatus
 */
MRL::WebScadaStatus::WebScadaStatus()
{
    // construct the table view
    auto layout = std::make_unique<Wt::WVBoxLayout>();
    //
    _navigationBar =  layout->addWidget(std::make_unique<Wt::WNavigationBar>());
    _navigationBar->setTitle("Web SCADA");

}
