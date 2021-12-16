/////////////////////////////////////////////////////////////////////////////
// Name:        AngularMeter.h
// Purpose:     wxIndustrialControls Library
// Author:      Marco Cavallini <m.cavallini AT koansoftware.com>
// Modified by:
// Copyright:   (C)2004-2006 Copyright by Koan s.a.s. - www.koansoftware.com
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
/////////////////////////////////////////////////////////////////////////////
//
//  Cleaned up and modified by Gary Harris for wxSmithKWIC, 2010.
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <math.h>
#define MAXSECTORCOLOR 10
/*!
 * \brief The kwxAngularMeter class
 */
class kwxAngularMeter : public wxControl {
    public:
        kwxAngularMeter(wxWindow *parent = nullptr, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
                        const wxSize &size = wxDefaultSize, long style = 0, const wxValidator &validator = wxDefaultValidator,
                        const wxString &name = wxControlNameStr) :
            wxControl(parent, id, pos, size, style, validator, name) {
            this->Connect(wxEVT_PAINT, wxPaintEventHandler(kwxAngularMeter::OnPaint), NULL, this);
            m_aSectorColor[0] = *wxWHITE;
        }


        virtual ~kwxAngularMeter() {
            delete membitmap;
        }
        void SetSectorColor(int nSector, wxColour colour) ;
        void SetNumSectors(int nSector) {
            if(nSector < MAXSECTORCOLOR) m_nSec = nSector ;
        }
        void SetNumTick(int nTick) {
            m_nTick = nTick ;
        }
        void SetRange(int min, int max) {
            m_nRangeStart = min ;
            m_nRangeEnd = max ;
        }
        void SetAngle(int min, int max) {
            m_nAngleStart = min ;
            m_nAngleEnd = max ;
        }
        void SetValue(int val);
        void SetNeedleColour(wxColour colour) {
            m_cNeedleColour = colour ;
        }
        void SetBackColour(wxColour colour) {
            m_cBackColour = colour ;
        }
        void SetBorderColour(wxColour colour) {
            m_cBorderColour = colour ;
        }
        void SetTxtFont(wxFont &font) {
            m_Font = font ;
        }
        void DrawCurrent(bool state) {
            m_bDrawCurrent = state ;
        }


    private:
        void OnPaint(wxPaintEvent &event);
        void    DrawTicks(wxDC &dc) ;
        void    DrawNeedle(wxDC &dc) ;
        void    DrawSectors(wxDC &dc) ;


    private:
        int     m_nRangeStart = 0;
        int     m_nRangeEnd = 220;
        int     m_nAngleStart = -20 ;
        int     m_nAngleEnd = 200;
        double  m_nScaledVal = 0;
        int     m_nTick = 0;
        int     m_nSec = 1;
        double  m_dPI = M_PI;
        int     m_nRealVal = 0 ;
        bool    m_bDrawCurrent = true;
        wxColour m_aSectorColor[MAXSECTORCOLOR] ;
        wxBitmap *membitmap  = nullptr;
        wxFont m_Font = *wxSWISS_FONT;
        wxColour m_cNeedleColour = *wxRED;
        wxColour m_cBackColour = *wxLIGHT_GREY;
        wxColour m_cBorderColour = *wxLIGHT_GREY ;
        wxBitmap  *m_pPreviewBmp = nullptr ;
};
