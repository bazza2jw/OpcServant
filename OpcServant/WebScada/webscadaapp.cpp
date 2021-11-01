#include "webscadaapp.h"
#include <webscadaapplication.h>
#include <Wt/WException.h>

/*!
    \brief OnInit
    \return
*/
bool MRL::WebScadaApp::OnInit() {
    try {
        // Building without GUI means smaller executable and smaller wx libraries
        wxAppConsole::OnInit(); // parse the command line
        wxLogDebug("OnInit()");
        //
        //
        TRC("Starting Web")
        // start the web interface
        // Now start the web thread
        static const char *av[] = {"WebScada", "--docroot=.", "--http-address=0.0.0.0", "--http-port=8083"};
        //
        std::string av0 = wxAppConsole::argv[0].ToStdString();
        _webThread = std::make_unique<Wt::WServer>(av0);
        _webThread->setServerConfiguration(4, (char **)(av), WTHTTP_CONFIGURATION);
        _webThread->addEntryPoint(Wt::EntryPointType::Application,
        [](const Wt::WEnvironment & env) {
            auto app = std::make_unique<MRL::WebScadaApplication>(env);
            app->setTwoPhaseRenderingThreshold(0);
            app->setTitle("Web Scada");
            app->useStyleSheet("styles.css");
            //app->messageResourceBundle().use(Wt::WApplication::appRoot() + "about");
            app->refresh();
            return std::move(app);
        });
        // initialise the model for the Wt model and link to the configuration and runtime
        _webThread->start(); // start the server thread
        //
        wxThread::Sleep(100); // give everything a chance to get going
    }
    catch (Wt::WException &e) {
        wxLogDebug("Web Exception %s", e.what());
    }
    catch (std::exception &e) {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
    //
    return true;
}

/*!
    \brief OnInitCmdLine
    \param parser
*/
void MRL::WebScadaApp::OnInitCmdLine(wxCmdLineParser &parser) {
    wxAppConsole::OnInitCmdLine(parser);
    parser.AddSwitch("web");
}

/*!
    \brief OnCmdLineParsed
    \param parser
    \return
*/
bool MRL::WebScadaApp::OnCmdLineParsed(wxCmdLineParser &parser) {
    // Override flags
    //_enableWeb |= parser.Found("web");
    return true;
}

/*!
    \brief OnExit
    \return
*/
int  MRL::WebScadaApp::OnExit() {
    wxLogDebug("OnExit()");
    try {
        // Shutdown Web
        if (_webThread && _webThread->isRunning()) {
            _webThread->stop();
            while (_webThread->isRunning()) {
                wxThread::Sleep(100);
            }
        }
        _webThread.reset(nullptr);
    }
    catch (std::exception &e) {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
    return wxAppConsole::OnExit();
}

