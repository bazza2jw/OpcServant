#ifndef OSCLIENTAPP_H
#define OSCLIENTAPP_H
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
#include <MrlLib/mrllib.h>
#include <wx/cmdline.h>
#include "wx/xrc/xmlres.h"
#include <wx/docview.h>
#include <wx/image.h>
#include <wx/thread.h>
#include <Wt/WServer.h>
#include <Common/database.h>
#include <MrlLib/anypropertytree.h>
#include <Common/displaypropertytree.h>
#include <Wt/WObject.h>
#include <Wt/WWidget.h>
#include <MrlLib/mrlsignals.h>
#include <Common/messages.h>
#include <Common/common.h>
//
#include <Common/database.h>
#include <Common/displaypropertytree.h>
//


namespace MRL {

    class OsClientApp :
        #ifdef USE_GUI
        public wxApp
        #else
        public wxAppConsole
        #endif
    {
            std::unique_ptr<Wt::WServer> _webThread; //! Web server thread

            bool _enableGui = true; // which sub systems are enabled - No GUI if running as deamon
            bool _enableWeb = true; // Web application
            //
            Database _configurationDatabase; // the configuration database
            std::unique_ptr<DataItemTree> _display;
            //
            std::string _configFileName = MRL::DEFAULT_SETTINGS_FILE;
            std::string _baseDir = MRL::DEFAULT_APPLICATION_DIR;

            MessageQueue _appQueue; // the queue for messages sent to the GUI and Web threads


        public:

            /*!
                \brief OnInit
                \return
            */
            virtual bool OnInit();
            /*!
                \brief OnInitCmdLine
                \param parser
            */
            void OnInitCmdLine(wxCmdLineParser &parser);
            /*!
                \brief OnCmdLineParsed
                \param parser
                \return true on success
            */
            bool OnCmdLineParsed(wxCmdLineParser &parser);
            /*!
                \brief OnExit
                \return
            */
            int OnExit();

            // Accessors
            Wt::WServer *webThread() {
                return  _webThread.get();   //! Web server thread
            }

            static MRL::OsClientApp *instance() {
                return static_cast<MRL::OsClientApp *>(MRL::OsClientApp::GetInstance());
            }
            /*!
                \brief display
                \return
            */
            static DataItemTree &display() {
                return *(instance()->_display);
            }

            /*!
                \brief configFileName
                \return
            */
            static std::string configFileName() {
                return instance()->_configFileName;
            }
            /*!
                \brief setConfigFileName
                \param s
            */
            static void setConfigFileName(const std::string &s) {
                instance()->_configFileName = s;
            }

            /*!
                \brief baseDir
                \return
            */
            static const std::string &baseDir() {
                return instance()->_baseDir;
            }
            /*!
                \brief configuration
                \return
            */
            static Database &configuration() {
                return instance()->_configurationDatabase;
            }


            bool initialise(const std::string &/*dir*/, const std::string &/*fs*/) {
                //
                instance()->_display = std::make_unique<MRL::DataItemTree>();
                //
                configuration().setFile("CLIENT");
                configuration().setDir(Common::baseDir() + "/data");
                configuration().createDatabase();
                configuration().loadFromDb();
                return true;
            }
    };
}


wxDECLARE_APP(MRL::OsClientApp);


#endif // OSCLIENTAPP_H
