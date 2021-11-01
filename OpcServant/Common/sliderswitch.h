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
#ifndef SLIDERSWITCH_H
#define SLIDERSWITCH_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif

class SliderSwitch :   public wxControl {
        bool _checked = false;
        wxColour _background;
        wxString _label;
        wxColour _onColour;
        wxColour _offColour;
        wxColour _darkOnColour;
        wxColour _darkOffColour;
    public:


        SliderSwitch() = default;
        SliderSwitch(wxWindow *parent = nullptr, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
                     const wxSize &size = wxDefaultSize, long style = wxTRANSPARENT_WINDOW | wxBORDER_NONE, const wxValidator &validator = wxDefaultValidator,
                     const wxString &name = wxControlNameStr);

        SliderSwitch(wxWindow *parent, wxWindowID id, const wxString &label = wxEmptyString,
                     const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                     long style = wxTRANSPARENT_WINDOW | wxBORDER_NONE, const wxValidator &validator = wxDefaultValidator,
                     const wxString &name = wxButtonNameStr);
        void setOnColour(wxColour c) {
            _onColour = c;
            _darkOnColour.MakeDisabled(70);
        }
        void setOffColour(wxColour c) {
            _offColour = c;
            _darkOffColour.MakeDisabled(70);
        }

        bool IsChecked() const {
            return _checked;
        }
        void SetChecked(bool f) {
            _checked = f;
            Refresh();
        }
    protected:
        bool Create(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize, long style = 0,
                    const wxValidator &validator = wxDefaultValidator,
                    const wxString &name = wxControlNameStr);
        //
        virtual void OnPaint(wxPaintEvent &event);
        virtual void OnLeftDown(wxMouseEvent &event);
        virtual void OnLeftUp(wxMouseEvent &event);
        virtual void Draw(wxDC &dc);
        //
        virtual wxSize DoGetBestSize() const {
            return wxSize(64, 48);
        }



};

#endif // SLIDERSWITCH_H
