#ifndef _FRAMECANVAS_H
#define _FRAMECANVAS_H

#include "ShapeCanvas/wxShapeFramework.h"

class SfMainFrm;
class FrameCanvas : public wxSFShapeCanvas
{
public:
    FrameCanvas(){;}
    FrameCanvas(wxSFDiagramManager* manager, SfMainFrm* parent = nullptr, wxWindowID id = 0);
	virtual ~FrameCanvas(void);

	// public virtual functions
	virtual void OnLeftDown(wxMouseEvent& event);
	virtual void OnRightDown(wxMouseEvent& event);
	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnConnectionFinished(wxSFLineShape* connection);
	virtual void OnMouseWheel(wxMouseEvent& event);

protected:
	// protected data members
    SfMainFrm *m_pParentFrame = nullptr;
};

#endif //_FRAMECANVAS_H
