/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "Mainframe.h"
#include "../buiapp.h"
#include <Common/Gui/SystemPropertiesDialog.h>
#include <Common/common.h>
#include <Common/objectmanager.h>
#include <MrlLib/states.h>
#include <Common/Daq/daq.h>
#include <Common/Daq/daqcommon.h>
#include <Common/messageids.h>
#include <Common/Gui/ReportGeneratorPanel.h>
#include <Common/Gui/AliasConfigurationDialog.h>

/*!
    \brief Mainframe::Mainframe
    \param parent
*/
Mainframe::Mainframe(wxWindow *parent)
    :
#ifdef RASPBERRY_PI_BUILD
    MainframeBase(parent),
#else
    MainframeBase(parent, wxID_ANY, _T("Navigator"),wxDefaultPosition,wxSize(800,600),wxDEFAULT_FRAME_STYLE),
#endif
    _autoLogoutTime(0, 5) {
    //
    //
    _reportPanel = new ReportGeneratorPanel(GetNotebook());
    GetNotebook()->InsertPage(1,_reportPanel, _("Report"));
    //
    // Connect to the DAQ
    join(MRL::Daq::instance());
    BObject::setEnabled(true);
    MRL::Common::instance()->messageNotify().connect(this,&Mainframe::handleMessage);
    _pinEntry = new PinEntryDialog(this);
}

/*!
*/
Mainframe::~Mainframe() {

}
/*!
 * \brief Mainframe::clearTabs
 */
void Mainframe::clearTabs() {
    Hide();
    for (auto i = MRL::Common::daq().tabWindows().begin(); i != MRL::Common::daq().tabWindows().end(); i++) {
        wxWindow *w = i->second;
        if (w) {
            int n = GetNotebook()->FindPage(w);
            if (n > 0) {
                GetNotebook()->DeletePage(n);
            }
        }
    }
    MRL::Common::daq().tabWindows().clear();
}
/*!
    \brief Mainframe::OnPopupClick
    \param evt
*/
void Mainframe::OnPopupClick(wxCommandEvent &evt) {
    switch (evt.GetId()) {
    case MRL::ObjectManagerAdd: {
        // Add a child item to the selected item
        unsigned t = MRL::Common::configuration().typeId(_currentPath);
        auto o = MRL::ObjectManager::find(t); // find the handler by type id
        if (o) {
            MRL::PropertyPath p;
            if (o->add(this, _currentPath, p)) {
                // create the runtime object if DAQ is started
                int id = MRL::Common::configuration().find(p);
                if (MRL::Daq::instance()->started()) { // is the daq process active
                    MRL::Daq::instance()->addObject(id);
                }
                //
                if (o->edit(this, p)) {
                    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                    if (r) {
                        r.get()->stop();
                        r.get()->start();
                    }
                }
                if(_reportPanel) _reportPanel->setValueList();
                wxLogDebug("Added");
            }
        }
    }
    break;
    case MRL::ObjectManagerDelete: {
        unsigned t = MRL::Common::configuration().typeId(_currentPath);
        auto o = MRL::ObjectManager::find(t); // find the handler by type id
        if (o) {
            int id = MRL::Common::configuration().find(_currentPath);
            if (id > 0) {
                MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                if (r) r.get()->stop();
            }
            o->remove(_currentPath);
            if(_reportPanel) _reportPanel->setValueList();
        }

    }
    break;
    case MRL::ObjectManagerProperties: {
        unsigned t = MRL::Common::configuration().typeId(_currentPath);
        auto o = MRL::ObjectManager::find(t); // find the handler by type id
        if (o) {
            int id = MRL::Common::configuration().find(_currentPath);
            if (id > 0) {
                if (o->edit(this, _currentPath)) {
                    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                    if (r) {
                        r.get()->stop();
                        r.get()->start();
                    }
                }
            }
        }

    }
    break;
    case MRL::ObjectManagerView: {
        unsigned t = MRL::Common::configuration().typeId(_currentPath);
        auto o = MRL::ObjectManager::find(t); // find the handler by type id
        if (o) {
            o->view((wxWindow *)this, _currentPath);
        }
    }
    break;

    default:
        break;
    }
}


/*!
    \brief Mainframe::setupTopLevel
*/
void Mainframe::setupTopLevel() {
    //
    // this must agree with the WebDataModel initialisation
    // TO DO scan registered types for top level types and add to tree
    //
}

/*!
    \brief Mainframe::setMainTab
*/
void Mainframe::setMainTab() {
    size_t n = GetNotebook()->GetPageCount();
    if (n > 0) {
        for (int i = 0; i < int(n); i++) {
            if (GetNotebook()->GetPageText(i) == _mainTab) {
                GetNotebook()->SetSelection(i);
                break;
            }
        }
    }
}



/*!
    \brief Mainframe::OnItemActivated
    \param event
*/
void Mainframe::OnItemActivated(wxDataViewEvent &/*event*/) {

}
/*!
    \brief Mainframe::OnSelChanged
    \param event
*/
void Mainframe::OnSelChanged(wxDataViewEvent &event) {
    wxDataViewItem d =  event.GetItem();
    wxString s = GetNavigation()->GetItemText(d);
    if (!s.IsEmpty()) {
        _currentItem =  d;
        wxString s = GetNavigation()->GetItemText(_currentItem);
        if (!s.IsEmpty()) {
            MRL::Common::display().findDataItemPath(_currentItem, _currentPath);
            _currentId = MRL::Common::configuration().find(_currentPath);
            unsigned t = MRL::Common::configuration().typeId(_currentPath); // get item type
            auto o = MRL::ObjectManager::find(t);
            if (o) {
                std::string cfg;
                o->writeConfigurationPage(_currentId, cfg);
                GetConfiguration()->SetPage(cfg);
            }
        }
    }
}
/*!
    \brief Mainframe::OnContextMenu
    \param event
*/
void Mainframe::OnContextMenu(wxDataViewEvent &event) {
    _currentItem =  event.GetItem();
    wxString s = GetNavigation()->GetItemText(_currentItem);

    if (!s.IsEmpty()) {
        MRL::Common::display().findDataItemPath(_currentItem, _currentPath);
        _currentId = MRL::Common::configuration().find(_currentPath);
        unsigned t = MRL::Common::configuration().typeId(_currentPath); // get item type
        auto o = MRL::ObjectManager::find(t);
        if (o) {
            wxMenu mnu;
            if (o->buildMenu(&mnu, _currentPath)) {
                mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Mainframe::OnPopupClick), NULL, this);
                PopupMenu(&mnu);
            }
        }
    }

}

/*!
 * \brief Mainframe::showPinEntry
 */
void Mainframe::showPinEntry()
{
    MRL::PropertyPath p;
    p.push_back("System");
    if(MRL::SETTINGS().getValue<bool>(p,"ScreenLock"))
    {
        _pinEntry->Show();
        _pinEntry->Maximize(true);
    }
}

/*!
    \brief Mainframe::OnStartupTimer
*/
void Mainframe::OnStartupTimer(wxTimerEvent & /*event*/) {
    if(!_inStartup)
    {
        _inStartup = true;
        // delayed actions to allow initialisation to complete
        // sync model with configuration tree
        GetNavigation()->Expand(MRL::Common::display().root().guiItem());
        //
        MRL::PropertyPath p;
        p.push_back("System");
        _mainTab =  MRL::SETTINGS().getValue<std::string>(p, "MainTab");
        //
#ifdef RASPBERRY_PI_BUILD
        Maximize(true); // gives access to top menu on Pi
        //ShowFullScreen(true); // occupies all of display
#else
        // get the screen size
        SetSize(10,10,640,480);
        Maximize(true);
#endif
        setMainTab();

        //
        if (MRL::SETTINGS().getValue<bool>(p, "EnableVK")) {
            VirtualKeypad::setKeypad(this);
        }
        _inStartup = false;
    }
}

/*!
    \brief Mainframe::OnTestPropertySheet
    \param event
*/
void Mainframe::OnTestPropertySheet(wxCommandEvent & /*event*/) {

}
/*!
    \brief Mainframe::OnClearMessages
*/
void Mainframe::OnClearMessages(wxCommandEvent & /*event*/) {
    GetMessages()->Clear();
}
/*!
    \brief Mainframe::onExit
    \param event
*/
void Mainframe::onExit(wxCommandEvent &/*event*/) {
    if (_exitEnabled) {
        clearTabs();
        MRL::BuiApp::GetInstance()->Exit();
    }
    else {
        Hide(); // otherwise we hide
    }
}
/*!
    \brief Mainframe::onProperties
    \param event
*/
void Mainframe::onProperties(wxCommandEvent &/*event*/) {
    if(adminPassword())
    {
        SystemPropertiesDialog dlg(this);
        if (dlg.ShowModal() == wxID_OK) {
            MRL::PropertyPath p;
            p.push_back("System");
            _mainTab = MRL::SETTINGS().getValue<std::string>(p, "MainTab");
            setMainTab();
        }
    }
}
/*!
 * \brief Mainframe::onPeriodicTimer
 */
void Mainframe::onPeriodicTimer(wxTimerEvent &/*event*/) {
    MRL::WriteLock l(_mutex);
    while (_msgBuffer.size() > 0) {
        GetMessages()->Append(_msgBuffer.front());
        _msgBuffer.pop();
    }
    //
    while (GetMessages()->GetCount() > MaxMessageBuffer) {
        GetMessages()->Delete(0); // purge the message buffer remove oldest entries
    }
    //
    process(); // drive the process loop
    MRL::Common::display().processGui();
    //
    // check the inactivity
    if (wxGetApp().inactvity()->IsInactiveFor(_autoLogoutTime)) {
        // disable the system panels
        MRL::Message m(MESSAGEID::IdleTimeout);
        BObject::sendMessage(MRL::Daq::instance(),m); // hint for idle timeout - UIs switch to default - close admin pages
        setMainTab();
        GetNavigation()->Disable();
    }
}

/*!
    \brief Mainframe::logMessage
    \param m
*/
void Mainframe::handleMessage(const MRL::LogMessage &m) {
    // format the message and put into message buffer
    std::stringstream os;
    os << std::ctime(&m._ts) << ":"  <<   m._state << ":"  <<  m._source << ":" << m._msg;
    MRL::WriteLock l(_mutex);
    _msgBuffer.push(os.str());
}

/*!
    \brief Mainframe::logMessage
    \param s
    \param m
    \param st
*/
void Mainframe::logMessage(const std::string &s, const std::string &m, const std::string &st) {
    MRL::LogMessage msg(s, m, st);
    handleMessage(msg);
}

/*!
    \brief Mainframe::processQueueItem
    \param m
    \return
*/
bool Mainframe::processQueueItem(const MRL::Message &msg) {
    try {

        MRL::Message &m = const_cast<MRL::Message &>(msg);
        if (!MRL::BObject::processQueueItem(msg)) {
            // notifications from DAQ thread
            switch (msg.id()) {

            case MESSAGEID::Started: { // DAQ thread is running - show the window
                if(_reportPanel) _reportPanel->setValueList();
            }
            break;

            case MESSAGEID::Publish_Object: {
                unsigned id;
                m.data().get(PARAMETERID::ObjectId, id);
                if (_currentId == id) {
                    // update the publish status pane
                    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                    if (r) {
                        std::string txt;
                        r->publishHtmlPage(txt);
                        GetPublish()->SetPage(txt);
                    }
                    else {
                        GetPublish()->SetPage("<b>Empty</b>");
                    }
                }

            }
            break;
            case MESSAGEID::Update_Object: { // Object has been updated (measured) in DAQ space - OPC and GUI/Web need to sync
                // get the object Id
                unsigned id;
                m.data().get(PARAMETERID::ObjectId, id);
                if (_currentId == id) {
                    // update the measure status pane
                    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                    if (r) {
                        std::string txt;
                        r->measureHtmlPage(txt);
                        GetMeasure()->SetPage(txt);
                    }
                    else {
                        GetPublish()->SetPage("<b>Empty</b>");
                    }
                }
            }
            break;
            case MESSAGEID::Log_Event: // Object has notified of event - typically in and out of fault

                break;
            case MESSAGEID::Diagnostic_Event: // an object has emitted a diagnostic (trace)

                break;
            case MESSAGEID::Audit_Event: // some security related event has happend - login/out or config update

                break;
            case MESSAGEID::Add_Object: // model changes

                GetNavigation()->Update();
                break;
            case MESSAGEID::Remove_Object: // model changes
                GetNavigation()->Update();
                break;

            case MESSAGEID::CreateTabView: { // add a tab view for the given object or poke it to refresh
                unsigned id;
                m.data().get(PARAMETERID::ObjectId, id);
                auto *rw = MRL::Common::daq().tabWindows()[id];

                if (!rw) {
                    // find the object manager for this object
                    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                    if (r) {

                        unsigned t = MRL::Common::configuration().typeId(r->path()); // get item type
                        auto o = MRL::ObjectManager::find(t);
                        if (o) {
                            wxWindow *w = o->createTabWindow(GetNotebook(), id);
                            //
                            MRL::Common::daq().tabWindows()[id] = w;
                            GetNotebook()->AddPage(w, r->path().back(), false, 0);
                            setMainTab();
                        }
                    }
                }
            }
            break;
            case MESSAGEID::CreateTopView: {
                //
                // create a top view - implies we hide this window
                //
                unsigned id;
                m.data().get(PARAMETERID::ObjectId, id);
                auto *rw = MRL::Common::daq().topWindows()[id];
                if (rw) {
                    // window already exists - activate it
                    rw->Maximize();
                }
                else {
                    // find the object manager for this object
                    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                    if (r) {

                        unsigned t = MRL::Common::configuration().typeId(r->path()); // get item type
                        auto o = MRL::ObjectManager::find(t);
                        if (o) {
                            wxFrame *w = o->createTopWindow(id);
                            MRL::Common::daq().topWindows()[id] = w;
                            w->Maximize();
                        }
                    }
                }

            }
            break;


            case MESSAGEID::PowerOff:
            {
                wxGetApp().setShutdownOnExit();
                wxExit();
            }
            break;
            default:
                break;
            }
        }

    }
    catch (const std::runtime_error &e)
    {
        EXCEPT_TRC
    }
    catch (...) {
        EXCEPT_DEF
    }
    return true;
}

/*!
    \brief Mainframe::updateCurrent
*/
void Mainframe::updateCurrent() {

}
/*!
    \brief Mainframe::OnRestart
*/
void Mainframe::OnRestart(wxCommandEvent & /*event*/) {
    MRL::Daq::instance()->post(MESSAGEID::Daq_Restart);
}
/*!
    \brief Mainframe::OnStart
*/
void Mainframe::OnStart(wxCommandEvent & /*event*/) {
    MRL::Daq::instance()->post(MESSAGEID::Daq_Start);
}
/*!
    \brief Mainframe::OnStop
*/
void Mainframe::OnStop(wxCommandEvent & /*event*/) {
    MRL::Daq::instance()->post(MESSAGEID::Daq_Stop);
}

/*!
 * \brief Mainframe::adminPassword
 * \return
 */
bool Mainframe::adminPassword()
{
    wxPasswordEntryDialog dlg(this, _("Admin Password"));
    if (dlg.ShowModal() == wxID_OK) {
        std::string pw = dlg.GetValue().ToStdString();
        if (MRL::Common::checkUser("Admin",pw)) {
                 return true;
        }
    }
    return false;
}
/*!
    \brief Mainframe::onUnlockNavigator
    \param event
*/
void Mainframe::onUnlockNavigator(wxCommandEvent & /*event*/) {
    // ask for password
    if (GetNavigation()->IsEnabled()) {
        GetNavigation()->Disable(); // lock the system screen now - will time out eventually
    }
    else {
        GetNavigation()->Enable(adminPassword()); // lock the system screen now - will time out eventually
    }
}
/*!
    \brief Mainframe::onClose
*/
void Mainframe::onClose(wxCloseEvent & /*event*/) {
    if (_exitEnabled) {
        clearTabs();
        Destroy();
    }
}
//
/*!
 * \brief Mainframe::OnAliasConfigure
 * \param event
 */
void Mainframe::OnAliasConfigure(wxCommandEvent& /*event*/)
{
    if(adminPassword())
    {
        AliasConfigurationDialog dlg(this);
        dlg.ShowModal();
    }
}
