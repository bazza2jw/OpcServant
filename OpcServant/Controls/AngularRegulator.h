/////////////////////////////////////////////////////////////////////////////
// Name:        AngularRegulator.h
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

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#pragma once
const int kwxEVT_ANGREG_FIRST = wxEVT_FIRST + 5402;

const wxEventType kwxEVT_ANGREG_CHANGE	= kwxEVT_ANGREG_FIRST + 1;

#define EVT_ANGULARREG_CHANGE(id, fn)	\
		DECLARE_EVENT_TABLE_ENTRY(kwxEVT_ANGREG_CHANGE, id, -1, \
		(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&fn, \
		(wxObject*)NULL ),


#define PGRECO (4.0 * atan(1.0))

class kwxAngularRegulator : public wxControl
{
public:

    kwxAngularRegulator() = default;
    kwxAngularRegulator(wxWindow* parent, const wxWindowID id,int value, int min, int max, const wxPoint& pos, const wxSize& size, const long int style) ;
    kwxAngularRegulator(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, const long int style) ;
	bool Create(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, const long int style);
	virtual ~kwxAngularRegulator();

    void	SetRange(int min, int max) { m_nMin = min ; m_nMax = max ; }
    void	SetAngle(int min, int max) { m_nAngleStart = min ; m_nAngleEnd = max ; }
    int	GetValue() { return m_nRealVal ; }
	void	SetValue(int val) ;

    void	SetExtCircleColour(wxColour colour) { m_cExtCircle = colour ; }
    void	SetIntCircleColour(wxColour colour) { m_cIntCircle = colour ; }

    void	SetKnobBorderColour(wxColour colour) { m_cKnobBorderColour = colour ; }
    void	SetKnobColour(wxColour colour) { m_cKnobColour = colour ; }

    void	SetLimitsColour(wxColour colour) { m_cLimitsColour = colour ; }

    void	SetTagsColour(wxColour colour) { m_cTagsColour = colour ; }

	void	AddTag(int value) ;

private:

	DECLARE_DYNAMIC_CLASS(kwxAngularRegulator)


	void	DrawKnob(wxDC &temp_dc) ;
	void	DrawLimit(wxDC &temp_dc) ;
	void	DrawTags(wxDC &temp_dc) ;

	void	SetPosition() ;
	double	GetAngleFromCord(int cx, int cy) ;

	void    OnPaint(wxPaintEvent& event);
	void	OnMouse(wxMouseEvent& event) ;


    int m_nStato = 0;
    int m_nMin = 0;
    int m_nMax = 100;
    int m_nAngleStart = 20;
    int m_nAngleEnd = 120;

    double m_nScaledVal = 0;
    double old_ang = 0;
    int m_nRealVal = 0;

	wxPoint m_mousePosition ;

    wxColour m_cExtCircle = *wxLIGHT_GREY ;
    wxColour m_cIntCircle  = *wxLIGHT_GREY  ;
    wxColour m_cKnobBorderColour   = *wxBLACK;
    wxColour m_cKnobColour = *wxLIGHT_GREY;
    wxColour m_cTagsColour  = *wxBLACK;
    wxColour m_cLimitsColour  = *wxBLACK;
    //
    int m_nClientWidth = 128;
    int m_nClientHeight = 128;
	int m_nTagsValue[20] ;
    int m_nTags = 0;
    double m_dxi = 1;
    double m_dyi = 1;

    wxBitmap *membitmap = nullptr;

} ;
