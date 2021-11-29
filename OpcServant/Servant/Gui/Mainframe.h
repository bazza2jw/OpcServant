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
#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "BUI.h"
#include <MrlLib/mrllib.h>
#include <Common/displaypropertytree.h>
#include <Common/bobject.h>
#include <queue>
#include <Common/VirtualKeypad.h>
namespace MRL
{
    class LogMessage;

}
class ReportGeneratorPanel;
/*!
 * \brief The Mainframe class
 */
class Mainframe : public MainframeBase, public MRL::BObject
{
    static constexpr int MaxMessageBuffer = 1000;
    //
    wxDataViewItem _currentItem;
    MRL::PropertyPath _currentPath;
    unsigned _currentId = 0;
    //
    MRL::ReadWriteMutex _mutex;
    std::queue<std::string> _msgBuffer;
    // SLOTS
    void handleMessage(const MRL::LogMessage &);
    //
    bool _exitEnabled = true;
    wxTimeSpan _autoLogoutTime; // 5 minute auto / inactivity logout
    wxString _mainTab = "System";
    //
    ReportGeneratorPanel * _reportPanel = nullptr;
    bool _inStartup = false;
public:
    Mainframe(wxWindow* parent);
    virtual ~Mainframe();
    void logMessage(const std::string &s, const std::string &m, const std::string &st = "Ok");
    void clearTabs();
protected:
    virtual void OnAliasConfigure(wxCommandEvent& event);
    virtual void onClose(wxCloseEvent& event);
    virtual void onUnlockNavigator(wxCommandEvent& event);
    virtual void OnRestart(wxCommandEvent& event);
    virtual void OnStart(wxCommandEvent& event);
    virtual void OnStop(wxCommandEvent& event);
    virtual void onPeriodicTimer(wxTimerEvent& event);
    virtual void onExit(wxCommandEvent& event);
    virtual void onProperties(wxCommandEvent& event);
    virtual void OnClearMessages(wxCommandEvent& event);
    virtual void OnTestPropertySheet(wxCommandEvent& event);
    virtual void OnStartupTimer(wxTimerEvent& event);
    virtual void OnContextMenu(wxDataViewEvent& event);
    virtual void OnItemActivated(wxDataViewEvent& event);
    virtual void OnSelChanged(wxDataViewEvent& event);
    void setupTopLevel();
    void OnPopupClick(wxCommandEvent &evt);
    bool processQueueItem(const MRL::Message &);
    void updateCurrent();
    void setMainTab();
public:
    void setExitEnabled(bool f)
    {
        _exitEnabled = f;
    }
};
#endif // Mainframe_H
