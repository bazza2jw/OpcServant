#include "osclientapp.h"
#include <Wt/WException.h>
#include <Common/common.h>
#ifdef USE_GUI
#include "OpcClientMainFrame.h"
#endif
#include <MrlLib/states.h>

/*!
 * \brief OnInit
 * \return
 */
bool MRL::OsClientApp::OnInit()
{
    try
    {
        // Building without GUI means smaller executable and smaller wx libraries
#ifdef USE_GUI
        wxApp::OnInit(); // parse the command line
        wxInitAllImageHandlers();
        wxXmlResource::Get()->InitAllHandlers();
#else
        wxAppConsole::OnInit(); // parse the command line
#endif


        wxLogDebug("OnInit()");
        STATES::States::initialiseStates();
        //
        //if(Common::initialise())
        // At this point we have the boot strap config in
        {
            PropertyPath p;
            p.push_back("System");
            // What sub systems do we enable
            //_enableGui |= stringToBool(SETTINGS().getValue<std::string>(p,"EnableGui"));
            //_enableWeb |= stringToBool(SETTINGS().getValue<std::string>(p,"EnableWeb"));
            //
            // Load plugin modules
//            std::string pluginDir(Common::baseDir());
//            pluginDir += "/";
//            pluginDir += PLUGIN_DIR;
            //
    #ifdef USE_GUI
            if(_enableGui)
            {
                TRC("Starting GUI")
                // GUI is not needed (wanted) if running as a service
                // Create GUI framework
                auto w = new OpcClientMainFrame(nullptr);
                // Show it
                w->Show();
                //
                // Link navigation pane to model
                //MRL::Common::display().setStore(w->GetNavigation());
                //
                // start the GUI Interface
                //
            }
    #endif
            //
//            if(_enableWeb )
//            {
//                TRC("Starting Web")
//                // start the web interface
//                // Now start the web thread
//                static const char * av[] = {"OpcClient","--docroot=.","--http-address=0.0.0.0", "--http-port=8083"};
//                //
//                std::string av0 = wxAppConsole::argv[0].ToStdString();
//                _webThread = std::make_unique<Wt::WServer>(av0);
//                _webThread->setServerConfiguration(4,(char **)(av), WTHTTP_CONFIGURATION);
//                _webThread->addEntryPoint(Wt::EntryPointType::Application,
//                [](const Wt::WEnvironment &env) {
//                      auto app = std::make_unique<MRL::BuiWebApplication>(env);
//                      app->setTwoPhaseRenderingThreshold(0);
//                      app->setTitle("Opc Client");
//                      app->useStyleSheet("styles.css");
//                      app->messageResourceBundle().use(Wt::WApplication::appRoot() + "about");
//                      app->refresh();
//                      return std::move(app);
//                  });
//                // Set up the model for web interfaces
//                MRL::Plugin::initialiseAllWeb();
//                // initialise the model for the Wt model and link to the configuration and runtime
//                _webThread->start(); // start the server thread
//            }
////
//            wxThread::Sleep(100); // give everything a chance to get going
//            MRL::Common::display().syncWithDatabase(MRL::Common::configuration()); // initial set up of models from configuration
        }
//        else
//        {
#ifdef USE_GUI
            //auto w = new SystemPropertiesDialog(nullptr);
            //w->Show();
#else
            wxLogDebug("Failed to initialise Common");
#endif
       // }
    }
    catch(Wt::WException &e)
    {
        wxLogDebug("Web Exception %s",e.what());
    }
    catch(std::exception &e)
    {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
    //
    return true;
}

/*!
 * \brief OnInitCmdLine
 * \param parser
 */
void MRL::OsClientApp::OnInitCmdLine (wxCmdLineParser &parser)
{
#ifdef USE_GUI
    wxApp::OnInitCmdLine(parser);
#else
    wxAppConsole::OnInitCmdLine(parser);
#endif
    //
    parser.AddSwitch("web");
    parser.AddSwitch("gui");
}

/*!
 * \brief OnCmdLineParsed
 * \param parser
 * \return
 */
bool MRL::OsClientApp::OnCmdLineParsed (wxCmdLineParser &parser)
{
    // Override flags
    _enableWeb |= parser.Found("web");
    _enableGui |= parser.Found("gui");
    return true;
}

/*!
 * \brief OnExit
 * \return
 */
int  MRL::OsClientApp::OnExit()
{
    wxLogDebug("OnExit()");
    try
    {
        // Shutdown Web
//        if(_webThread && _webThread->isRunning())
//        {
//            _webThread->stop();
//            while(_webThread->isRunning())
//            {
//                wxThread::Sleep(100);
//            }
//        }



     // give a chance for all threads to finish

        //
        // dead threads should not be deleted on exit
        //
//        _webThread.reset(nullptr);
        //
    }
    catch(std::exception &e)
    {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
#ifdef USE_GUI
    return wxApp::OnExit();
#else
    return wxAppConsole::OnExit();
#endif
}

