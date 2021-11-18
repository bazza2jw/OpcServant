#ifndef SCALEOFFSETCONFIGUREPANEL_H
#define SCALEOFFSETCONFIGUREPANEL_H
#include "BourbonCommonUI.h"
#include <Common/common.h>
#include <MrlLib/scaleoffset.h>

/*!
 * \brief The ScaleOffsetConfigurePanel class
 */
class ScaleOffsetConfigurePanel : public ScaleOffsetConfigurePanelBase
{
    MRL::ScaleOffsetMap _map;
public:
    ScaleOffsetConfigurePanel(wxWindow* parent, int nrows);
    ScaleOffsetConfigurePanel(wxWindow* parent, const MRL::StringVector &l) : ScaleOffsetConfigurePanel(parent,l.size()) {}
    virtual ~ScaleOffsetConfigurePanel();
    virtual void read( MRL::VariantPropertyTree &c, MRL::PropertyPath &p);
    virtual void write( MRL::VariantPropertyTree &c, MRL::PropertyPath &p);
    MRL::ScaleOffsetMap & map() { return _map;}
};
#endif // SCALEOFFSETCONFIGUREPANEL_H
