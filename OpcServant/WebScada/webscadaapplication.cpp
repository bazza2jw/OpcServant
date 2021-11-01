#include "webscadaapplication.h"
#include "webscadamain.h"
/*!
   * \brief MRL::WebScadaApplication::WebScadaApplication
   * \param env
   */
  MRL::WebScadaApplication::WebScadaApplication(const Wt::WEnvironment& env) : Wt::WApplication(env)
  {
      setTitle("OPC Servant User Interface");
      // Get the boot strap theme for the navigation bar
      _theme = std::make_shared<Wt::WBootstrapTheme>();
      _theme->setVersion(Wt::BootstrapVersion::v3);
      setTheme(_theme);
      //
      // Set up the model for the configuration
      //
      _topContainer = root()->addWidget(std::make_unique<MRL::WebScadaMain>());

  }
