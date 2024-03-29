/////////////////////////////////////////////////////////////////////////////
// Name:        BmpSwitcher.cpp
// Purpose:     wxIndustrialControls Library
// Author:      Marco Cavallini <m.cavallini AT koansoftware.com>
// Modified by:
// Copyright:   (C)2004-2006 Copyright by Koan s.a.s. - www.koansoftware.com
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
/////////////////////////////////////////////////////////////////////////////

//	La classe kwxBmpSwitcher riceve una o pi� bitmap tramite il metodo
//	AddBitmap, e permette lo scambio di esse tramite il metodo IncState o SetState.
//
//	IncState():			Passa alla bitmap successiva, se si trova all'ultima
//						bitmap riparte dalla prima inserita.
//
//	SetState(stato):	Carica direttamente la bitmap indicata dallo stato
//						passato come parametro.
//
//	Nel caso in cui sia stata inserita una sola bitmap il metodo IncState
//	non ha effetto sul controllo.
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

#include "BmpSwitcher.h"
#include <wx/event.h>

#include <wx/listimpl.cpp>

WX_DEFINE_LIST(CBmpList)

/*!
 * \brief kwxBmpSwitcher::kwxBmpSwitcher
 * \param parent
 * \param id
 * \param pos
 * \param size
 * \param style
 */

kwxBmpSwitcher::kwxBmpSwitcher(wxWindow* parent,
                           const wxWindowID id,
                           const wxPoint& pos,
                           const wxSize& size, const long style)
    : wxWindow(parent, id, pos, size, style)
{
	if (parent)
		SetBackgroundColour(parent->GetBackgroundColour());
	else
		SetBackgroundColour(*wxLIGHT_GREY);

	SetSize(size.GetWidth(), size.GetHeight());

	SetAutoLayout(TRUE);
	Refresh();

	m_nState = 0 ;
	membitmap = new wxBitmap(size.GetWidth(), size.GetHeight()) ;
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(kwxBmpSwitcher::OnPaint), NULL, this);

}
/*!
 * \brief kwxBmpSwitcher::~kwxBmpSwitcher
 */
kwxBmpSwitcher::~kwxBmpSwitcher()
{
	delete membitmap;

	CBmpList::Node *node= m_bmplist.GetFirst() ;

	while(node)
	{
        wxBitmap *current = node->GetData();
		delete current ;
		node = node->GetNext() ;
	}
}
/*!
 * \brief kwxBmpSwitcher::OnPaint
 * \param WXUNUSED
 */
void kwxBmpSwitcher::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	wxPaintDC old_dc(this);

	int w,h;
	wxBitmap *pCurrent ;

	GetClientSize(&w,&h);

	/////////////////

	// Create a memory DC
	wxMemoryDC dc;
	dc.SelectObject(*membitmap);
    dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(),wxBRUSHSTYLE_SOLID));
	dc.Clear();

	// Cryogen 16/4/10 Fixed to prevent a crash when m_nCount = 0. This is necessary for
	// wxSmithKWIC to be able to initialise the control before bitmaps are added.
	// Also moved update of m_nCount and m_nState to the appropriate functions.
	CBmpList::Node *node;
	switch(m_nCount){
		case 0:
			break;
		case 1:
			node = m_bmplist.GetFirst();
			pCurrent = node->GetData() ;
			dc.DrawBitmap(*pCurrent, 0, 0, TRUE);
			break;

		default:
			node = m_bmplist.Item(m_nState);
			pCurrent = node->GetData() ;

			dc.DrawBitmap(*pCurrent, 0, 0, TRUE);
			break;
	}

	old_dc.Blit(0, 0, w, h, &dc, 0, 0);
}
/*!
 * \brief kwxBmpSwitcher::IncState
 */
void kwxBmpSwitcher::IncState()
{
	if (m_nCount > 1 )
	{
		m_nState++ ;
		if (m_nState >= m_nCount )
			m_nState = 0 ;

		Refresh() ;
	}
}
/*!
 * \brief kwxBmpSwitcher::SetState
 * \param state
 */
void kwxBmpSwitcher::SetState(int state)
{
	if (m_nCount > 1 )
	{
		m_nState = state ;
		if (m_nState >= m_nCount )
			m_nState = 0 ;

		Refresh() ;
	}
}
/*!
 * \brief kwxBmpSwitcher::AddBitmap
 * \param bitmap
 */
void kwxBmpSwitcher::AddBitmap(wxBitmap *bitmap)
{
	m_bmplist.Append(bitmap);
	m_nCount = m_bmplist.GetCount() ;
}
