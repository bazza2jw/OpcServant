/////////////////////////////////////////////////////////////////////////////
// Name:        LinearMeter.h
// Purpose:     wxIndustrialControls Library
// Author:      Marco Cavallini <m.cavallini AT koansoftware.com>
// Modified by:
// Copyright:   (C)2004-2006 Copyright by Koan s.a.s. - www.koansoftware.com
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
/////////////////////////////////////////////////////////////////////////////
//
//	Cleaned up and modified by Gary Harris for wxSmithKWIC, 2010.
//
/////////////////////////////////////////////////////////////////////////////


#ifndef __LINEARMETER_H__
#define __LINEARMETER_H__
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#define MAX_TAGS 10

class kwxLinearMeter : public wxControl
{
public:
    kwxLinearMeter(wxWindow *parent = nullptr, const wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0)
        :  wxControl(parent, id, pos, size, style)
    {
        this->Connect(wxEVT_PAINT, wxPaintEventHandler(kwxLinearMeter::OnPaint), NULL, this);
    }

    virtual ~kwxLinearMeter()
    {
        if(membitmap)delete membitmap;
    }

    void SetRange(int min, int max) {m_nMin = min; m_nMax = max ; } ;
	void SetActiveBarColour(wxColour colour) { m_cActiveBar = colour ; } ;
	void SetPassiveBarColour(wxColour colour) { m_cPassiveBar = colour ; } ;
	void SetBorderColour(wxColour colour) { m_cBorderColour = colour ; } ;
	void SetValue(int val) ;
	void SetTxtFont(wxFont &font) {m_Font = font ; } ;
	void SetTxtLimitColour(wxColour colour) { m_cLimitColour = colour ; } ;
	void SetTxtValueColour(wxColour colour) { m_cValueColour = colour ; } ;
	void SetTagsColour(wxColour colour) { m_cTagsColour = colour ; } ;

	void SetOrizDirection(bool dir) { m_bDirOrizFlag = dir ; } ;
	void ShowCurrent(bool show) { m_bShowCurrent = show ; } ;
	void ShowLimits(bool show) { m_bShowLimits = show ; } ;

	void AddTag(int val) ;

	int GetValue() { return m_nRealVal ; } ;

protected:
    int m_nScaledVal = 0;
    int m_nRealVal = 0;

    bool m_bDirOrizFlag = true ;

    int m_nMax = 100;
    int m_nMin = 0;

private:
    // any class wishing to process wxWindows events must use this macro

	void    OnPaint(wxPaintEvent& event);


private:

	void DrawLimits(wxDC &dc);
	void DrawCurrent(wxDC &dc);
	void DrawTags(wxDC &dc);

    wxBitmap *membitmap = nullptr ;

	int m_aTagsVal[MAX_TAGS] ;

    int m_nTagsNum = 0;


    bool m_bShowCurrent = true;
    bool m_bShowLimits = true;

    wxColour m_cActiveBar = *wxGREEN ;
    wxColour m_cPassiveBar = *wxWHITE ;
    wxColour m_cLimitColour = *wxBLACK ;
    wxColour m_cValueColour = *wxRED ;
    wxColour m_cBorderColour  = *wxRED;
    wxColour m_cTagsColour  = *wxGREEN;
    wxFont m_Font  = *wxSWISS_FONT;
};

#endif
