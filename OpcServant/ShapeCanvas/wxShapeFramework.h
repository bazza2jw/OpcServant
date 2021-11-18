/***************************************************************
 * Name:      wxShapeFramework.h
 * Purpose:   Main header file
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSF_H_
#define _WXSF_H_

// main library classes
#include <ShapeCanvas/DiagramManager.h>
#include <ShapeCanvas/ShapeCanvas.h>
#include <ShapeCanvas/Thumbnail.h>
#include <ShapeCanvas/AutoLayout.h>

// shapes' declarations
#include <ShapeCanvas/RectShape.h>
#include <ShapeCanvas/RoundRectShape.h>
#include <ShapeCanvas/FixedRectShape.h>
#include <ShapeCanvas/EllipseShape.h>
#include <ShapeCanvas/CircleShape.h>
#include <ShapeCanvas/DiamondShape.h>
#include <ShapeCanvas/TextShape.h>
#include <ShapeCanvas/EditTextShape.h>
#include <ShapeCanvas/BitmapShape.h>
#include <ShapeCanvas/PolygonShape.h>
#include <ShapeCanvas/ControlShape.h>
#include <ShapeCanvas/GridShape.h>
#include <ShapeCanvas/FlexGridShape.h>

// arrows' declarations
#include <ShapeCanvas/OpenArrow.h>
#include <ShapeCanvas/SolidArrow.h>
#include <ShapeCanvas/DiamondArrow.h>
#include <ShapeCanvas/CircleArrow.h>

// connection lines' declarations
#include <ShapeCanvas/LineShape.h>
#include <ShapeCanvas/CurveShape.h>
#include <ShapeCanvas/OrthoShape.h>
#include <ShapeCanvas/RoundOrthoShape.h>

// library events
#include <ShapeCanvas/SFEvents.h>

// printing support
#include <ShapeCanvas/Printout.h>

// common functions
#include <ShapeCanvas/CommonFcn.h>

// serialize/deserialize functionality
#include <ShapeCanvas/XmlSerializer.h>

#endif //_WXSF_H_
