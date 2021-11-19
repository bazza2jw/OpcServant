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
#include "states.h"
#include "StatesIcons.h"
#include "stats.hpp"

static StatesImageList * _images;
//static  StatesImages _images;
// the state map

STATES::States::MAP STATES::States::_states;

//
// set up default set
//
void STATES::States::initialiseStates()
{
        _images = new StatesImageList();
        _states.clear();
        _states[STATE_NONE]     =  States(STATE_NONE, "Inactive", StateNone, "None");
        _states[STATE_WAITING]  =  States(STATE_WAITING, "Wait",StateWaiting,"Waiting");
        _states[STATE_DISABLED] =  States(STATE_DISABLED, "Disabled", StateDisabled, "Disabled",*wxBLACK,*wxLIGHT_GREY);
        _states[STATE_FAULT]    =  States(STATE_FAULT, "Fault",StateFault,"Fault",*wxRED,*wxBLACK);
        _states[STATE_OK]       =  States(STATE_OK, "Ok",StateOk, "Ok", *wxBLACK,*wxGREEN);
        _states[STATE_STARTED] = States(STATE_STARTED, "Started",StateOk, "Started", *wxBLACK,*wxGREEN);
        _states[STATE_STOPPED] = States(STATE_STOPPED, "Stopped",StateOk, "Stopped", *wxWHITE,*wxRED);
        _states[STATE_ON] = States(STATE_ON, "On",StateOk, "On", *wxBLACK,*wxWHITE);
        _states[STATE_OFF] = States(STATE_OFF, "Off",StateOk, "Off", *wxWHITE,*wxBLACK);

// Log levels
        _states[STATE_TRACE] =  States(STATE_TRACE, "Trace",0,
                                         "Trace", *wxBLACK, *wxLIGHT_GREY);
        _states[STATE_DEBUG] =  States(STATE_DEBUG, "Debug",0,
                                         "Debug", *wxRED, *wxLIGHT_GREY);
        _states[STATE_INFO] =  States(STATE_INFO, "Info",0,
                                       "Information", *wxWHITE, *wxBLUE);
        _states[STATE_WARNING] =  States(STATE_WARNING, "Warning",200,
                                         "Warning", *wxBLACK, *wxYELLOW);
        _states[STATE_ERROR] =  States(STATE_ERROR, "Error",300,
                                        "Error", *wxBLACK, *wxRED);
        //
        _states[STATE_CRITICAL] =  States(STATE_CRITICAL, "Critical",500,
                                         "Critical", *wxWHITE, *wxCYAN);
        _states[STATE_FATAL] =  States(STATE_FATAL, "Fatal",600,
                                         "Fatal", *wxWHITE, *wxBLUE);
        _states[STATE_WRITE] =  States(STATE_WRITE, "Write",0,
                                         "Write", *wxBLACK, *wxWHITE);
//
// define the alarm threshold colour scheme
//
        _states[STATE_HIHI] =  States(STATE_HIHI, "Action",StateAction,
                                  "High Action", *wxBLACK, *wxRED);
        _states[STATE_LOLO] =  States(STATE_LOLO, "Action",StateAction,
                                        "Low Action", *wxBLACK, *wxRED);
        _states[STATE_HILO] =  States(STATE_HILO, "Alert",StateAlert,
                                         "High Alert", *wxBLACK, *wxYELLOW);
        _states[STATE_LOHI] =  States(STATE_LOHI, "Alert",StateAlert,
                                         "Low Alert", *wxBLACK, *wxYELLOW);

        _states[STATE_ACTION] =  States(STATE_ACTION,"Action",StateAction,
                                        "Action", *wxBLACK, *wxRED);
        _states[STATE_ALERT] =  States(STATE_ALERT, "Alert",StateAlert,
                                         "Alert", *wxBLACK, *wxYELLOW);



}

/*!
 * \brief STATES::States::getIcon
 * \return bitmap for icon
 */
const wxBitmap &STATES::States::getIcon()
{
    return _images->Bitmap(iconName);
}

/*!
 * \brief STATES::States::loadStates
 * \return true on success
 */
bool STATES::States::loadStates(const std::string & /*s*/)
{
	return false;
}

/*!
 * \brief STATES::States::saveStates
 * \return true on success
 */
bool STATES::States::saveStates(const std::string &/*fileName*/)
{
    return false;
}


// translate  from statistics state to state name
/*!
 * \brief STATES::States::StatisticStateToState
 * \param st
 * \return
 */
std::string STATES::States::StatisticStateToState(MRL::StatisticsThresholdSet &st)
{
    std::string  res = STATE_OK;
     if(st.triggered())
     {
         switch(st.maxState())
         {
            case MRL::StatisticsThreshold::HiHi:
             {
                 res = STATE_HIHI;
             }
             break;
             case MRL::StatisticsThreshold::HiLo:
             {
                 res = STATE_HILO;
             }
             break;
             case MRL::StatisticsThreshold::LoHi:
             {
                 res = STATE_LOHI;
             }
             break;
             case MRL::StatisticsThreshold::LoLo:
             {
                 res = STATE_LOLO;
             }
             break;
         default:
             break;
         }
     }
     return res;
}

