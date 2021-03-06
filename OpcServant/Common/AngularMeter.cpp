/////////////////////////////////////////////////////////////////////////////
// Name:        AngularMeter.cpp
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

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include <wx/image.h>
#include <math.h>
#include <wx/log.h>

#include "AngularMeter.h"
#include <wx/event.h>


/*!
 * \brief kwxAngularMeter::SetValue
 * \param val
 */
void kwxAngularMeter::SetValue(int val)
{
	int deltarange = m_nRangeEnd - m_nRangeStart;
	int deltaangle = m_nAngleEnd - m_nAngleStart;
	double coeff = (double)deltaangle / (double)deltarange;

	m_nScaledVal = (int)((double)(val - m_nRangeStart) * coeff);
	m_nRealVal = val;
	Refresh();
}
/*!
 * \brief kwxAngularMeter::OnPaint
 * \param WXUNUSED
 */
void kwxAngularMeter::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	wxPaintDC old_dc(this);

	int w,h ;
	GetClientSize(&w,&h);
    if(membitmap && ((membitmap->GetWidth() != w) || (membitmap->GetHeight() != h)))
    {
        delete membitmap;
        membitmap = nullptr;

    }
    if(!membitmap)
    {
        membitmap = new wxBitmap(w,h);
    }

	/////////////////


	// Create a memory DC
	wxMemoryDC dc;
	dc.SelectObject(*membitmap);

    dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(m_cBackColour,wxBRUSHSTYLE_SOLID));
    dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(m_cBackColour,wxBRUSHSTYLE_SOLID));
	dc.Clear();


	///////////////////

	//Rettangolo

    dc.SetPen(*wxThePenList->FindOrCreatePen(m_cBorderColour, 1, wxPENSTYLE_SOLID));
	dc.DrawRectangle(0,0,w,h);

	//settori
	DrawSectors(dc) ;

	//tacche
	if (m_nTick > 0)
		DrawTicks(dc);

	//indicatore lancetta

	DrawNeedle(dc);


	//testo valore
	if (m_bDrawCurrent)
	{
		wxString valuetext;
		valuetext.Printf(wxT("%d"),m_nRealVal);
		dc.SetFont(m_Font);
		dc.DrawText(valuetext, (w / 2) - 10, (h / 2) + 10);
	}

	// We can now draw into the memory DC...
	// Copy from this DC to another DC.
	old_dc.Blit(0, 0, w, h, &dc, 0, 0);
}

void kwxAngularMeter::DrawNeedle(wxDC &dc)
{
	//indicatore triangolare
	double dxi,dyi, val;
	wxPoint ppoint[6];
	int w, h ;

	GetClientSize(&w,&h);

    dc.SetPen(*wxThePenList->FindOrCreatePen(m_cNeedleColour, 1,wxPENSTYLE_SOLID));

	val = (m_nScaledVal + m_nAngleStart) * m_dPI / 180; //radianti parametro angolo

	dyi = sin(val - 90) * 2; //coordinate base sinistra
	dxi = cos(val - 90) * 2;

	ppoint[0].x = (w / 2) - dxi;	//base sinistra
	ppoint[0].y = (h / 2) - dyi;

	dxi = cos(val) * ((h / 2) - 4); //coordinate punta indicatore
	dyi = sin(val) * ((h / 2) - 4);

	ppoint[2].x = (w / 2) - dxi;	//punta
	ppoint[2].y = (h / 2) - dyi;

	dxi = cos(val + 90) * 2; //coordinate base destra
	dyi = sin(val + 90) * 2;

	ppoint[4].x = (w / 2) - dxi;	//base destra
	ppoint[4].y = (h / 2) - dyi;

	ppoint[5].x = ppoint[0].x;	//ritorno base sinistra
	ppoint[5].y = ppoint[0].y;

//////////////////////////
	val = (m_nScaledVal + m_nAngleStart + 1) * m_dPI / 180;

	dxi = cos(val) * ((h / 2) - 10); //coordinate medio destra
	dyi = sin(val) * ((h / 2) - 10);

	ppoint[3].x = (w / 2) - dxi;	//base media destra
	ppoint[3].y = (h / 2) - dyi;

	val = (m_nScaledVal + m_nAngleStart - 1) * m_dPI / 180;

	dxi = cos(val) * ((h / 2) - 10); //coordinate medio sinistra
	dyi = sin(val) * ((h / 2) - 10);

	ppoint[1].x = (w / 2) - dxi;	//base media sinistra
	ppoint[1].y = (h / 2) - dyi;

/////////////////////////


    dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(m_cNeedleColour,wxBRUSHSTYLE_SOLID));

	dc.DrawPolygon(6, ppoint, 0, 0, wxODDEVEN_RULE);

	//cerchio indicatore
    dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(*wxWHITE,wxBRUSHSTYLE_SOLID));
	dc.DrawCircle(w / 2, h / 2, 4);
}
/*!
 * \brief kwxAngularMeter::DrawSectors
 * \param dc
 */
void kwxAngularMeter::DrawSectors(wxDC &dc)
{
	double starc,endarc;
	int secount,dx,dy;
	int w,h ;

	double val;

	GetClientSize(&w,&h);

	//arco -> settori
    dc.SetPen(*wxThePenList->FindOrCreatePen(*wxBLACK, 1, wxPENSTYLE_SOLID));

	starc = m_nAngleStart;
	endarc = starc + ((m_nAngleEnd - m_nAngleStart) / (double)m_nSec);
	for(secount=0;secount<m_nSec;secount++)
	{
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(m_aSectorColor[secount],wxBRUSHSTYLE_SOLID));
		dc.DrawEllipticArc(0,0,w,h,180 - endarc,180 - starc);
		starc = endarc;
		endarc += ((m_nAngleEnd - m_nAngleStart) / (double)m_nSec);
	}

	val = (m_nAngleStart * m_dPI) / 180.0;
	dx = cos(val) * h / 2.0;
	dy = sin(val) * h / 2.0;

	dc.DrawLine(w / 2, h / 2, (w / 2) - dx, (h / 2) - dy);	//linea sinistra

	val = (m_nAngleEnd * m_dPI) / 180.0;
	dx = cos(val) * h / 2.0;
	dy = sin(val) * h / 2.0;

	dc.DrawLine(w / 2, h / 2, (w / 2) - dx, (h / 2) - dy);	//linea destra

}

void kwxAngularMeter::DrawTicks(wxDC &dc)
{
	double intervallo = (m_nAngleEnd - m_nAngleStart) / (m_nTick + 1.0);
	double valint = intervallo + m_nAngleStart;
	double tx, ty;
	double val;
	double dx, dy;
	int n, w, h;
	int tw, th;
	wxString s;

	GetClientSize(&w, &h);

	for(n = 0;n < m_nTick;n++)
	{
		val=(valint * m_dPI) / 180;
		//wxLogTrace("v: %f",valint);
		dx = cos(val) * (h/2);	//punto sul cerchio
		dy = sin(val) * (h/2);

		tx = cos(val) * ((h / 2) - 10);	//punto nel cerchio
		ty = sin(val) * ((h / 2) - 10);

		dc.DrawLine((w / 2) - tx, (h / 2) - ty, (w / 2) - dx, (h / 2) - dy);

		int deltarange = m_nRangeEnd - m_nRangeStart;
		int deltaangle = m_nAngleEnd - m_nAngleStart;
		double coeff = (double)deltaangle / (double)deltarange;

		int rightval = (int)(((valint - (double)m_nAngleStart)) / coeff) + m_nRangeStart;

		s.Printf(wxT("%d"), rightval);

		dc.GetTextExtent(s, &tw, &th);

		val = ((valint - 4) * m_dPI) / 180;	//angolo spostato
		tx = cos(val) * ((h / 2) - 12);	//punto testo
		ty = sin(val) * ((h / 2) - 12);

		dc.SetFont(m_Font);

		dc.DrawRotatedText(s,(w / 2) - tx, (h / 2) - ty, 90 - valint);

		valint = valint + intervallo;
	}
}
/*!
 * \brief kwxAngularMeter::SetSectorColor
 * \param nSector
 * \param colour
 */
void kwxAngularMeter::SetSectorColor(int nSector, wxColour colour)
{
	m_aSectorColor[nSector] = colour;
}
