/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided with open62541.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "ICPCON7017Panel.h"
#include <Common/common.h>
#include <Common/Daq/daqcommon.h>
#include <Common/Daq/rtobject.h>
#include "icpcon7017rt.h"
#include <wx/valnum.h>
#include <MrlLib/variantpropertytree.h>
/*!
    \brief ICPCON7017Panel::ICPCON7017Panel
    \param parent
    \param id
*/
ICPCON7017Panel::ICPCON7017Panel(wxWindow *parent, unsigned id)
    : ICPCON7017PanelBase(parent), _id(id) {
    //
    // get notifications
    join(MRL::Daq::instance());
    MRL::BObject::setEnabled(true);
    //
    _values[0] = GetIN0();
    _values[1] = GetIN1();
    _values[2] = GetIN2();
    _values[3] = GetIN3();
    _values[4] = GetIN4();
    _values[5] = GetIN5();
    _values[6] = GetIN6();
    //
    _units[0] = GetIN0units();
    _units[1] = GetIN1units();
    _units[2] = GetIN2units();
    _units[3] = GetIN3units();
    _units[4] = GetIN4units();
    _units[5] = GetIN5units();
    _units[6] = GetIN6units();
    //
    // Get the object's data
    //
    loadFields();


}

ICPCON7017Panel::~ICPCON7017Panel() {
}


void ICPCON7017Panel::loadFields() {
    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[_id];
    // get the configration database
    if (r) {
        MRL::VariantPropertyTree &t = r->configuration();
        //
        // get the units fields
        _units[0]->SetLabelText(t.getString("IN0_Units"));
        _units[1]->SetLabelText(t.getString("IN1_Units"));
        _units[2]->SetLabelText(t.getString("IN2_Units"));
        _units[3]->SetLabelText(t.getString("IN3_Units"));
        _units[4]->SetLabelText(t.getString("IN4_Units"));
        _units[5]->SetLabelText(t.getString("IN5_Units"));
        _units[6]->SetLabelText(t.getString("IN6_Units"));
    }
}


/*!
    \brief AerisNOxUI::processQueueItem
    \return
*/
bool ICPCON7017Panel::processQueueItem(const MRL::Message &msg) {
    MRL::Message &m = const_cast<MRL::Message &>(msg);
    if (!MRL::BObject::processQueueItem(m)) {
        // notifications from DAQ thread
        switch (m.id()) {
            //
            case MESSAGEID::CreateTabView: {
                unsigned id = 0;
                m.data().get(PARAMETERID::ObjectId, id); // set the source of the message
                if (id == _id) {
                    loadFields(); // start triggered
                }
            }
            break;
            // catch the events from the RT object Get the measure events and update
            case MESSAGEID::Update_Object: {
                if (!GetPauseButton()->GetValue()) {
                    // unpack the bits we want and update the UI
                    unsigned id = 0;
                    m.data().get(PARAMETERID::ObjectId, id); // set the source of the message
                    if (m.data().isType<double>(PARAMETERID::Value)) {
                        double dv = 0.0;
                        std::string tag;
                        m.data().get(PARAMETERID::Tag, tag); // get any tag value - blank if nonexistant
                        m.data().get(PARAMETERID::Value, dv);
                        //
                        // update the row
                        // The tags are IN0 - IN6
                        switch (tag[2]) {
                            case '0':
                                _values[0]->SetLabelText(wxString::FromDouble(dv, 2));
                                break;
                            case '1':
                                _values[1]->SetLabelText(wxString::FromDouble(dv, 2));
                                break;
                            case '2':
                                _values[2]->SetLabelText(wxString::FromDouble(dv, 2));
                                break;
                            case '3':
                                _values[3]->SetLabelText(wxString::FromDouble(dv, 2));
                                break;
                            case '4':
                                _values[4]->SetLabelText(wxString::FromDouble(dv, 2));
                                break;
                            case '5':
                                _values[5]->SetLabelText(wxString::FromDouble(dv, 2));
                                break;
                            case '6':
                                _values[6]->SetLabelText(wxString::FromDouble(dv, 2));
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
            break;
            //
            default:
                return false;
        }
    }
    return true;
}

void ICPCON7017Panel::onPause(wxCommandEvent & /*event*/) {

}
void ICPCON7017Panel::onProcessTimer(wxTimerEvent& /*event*/)
{
    process(); // drive the message loop
}
