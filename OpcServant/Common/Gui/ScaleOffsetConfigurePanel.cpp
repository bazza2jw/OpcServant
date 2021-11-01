#include "ScaleOffsetConfigurePanel.h"

/*!
 * \brief ScaleOffsetConfigurePanel::ScaleOffsetConfigurePanel
 * \param parent
 * \param nrows
 */
ScaleOffsetConfigurePanel::ScaleOffsetConfigurePanel(wxWindow* parent, int nrows)
    : ScaleOffsetConfigurePanelBase(parent)
{
    GetGrid()->SetDefaultEditor(new wxGridCellFloatEditor (8, 3));
    GetGrid()->AppendRows(nrows);
    GetGrid()->SetColFormatFloat(0,6,1);
    GetGrid()->SetColFormatFloat(1,6,1);
    GetGrid()->SetColLabelValue(0,_("Scale"));
    GetGrid()->SetColLabelValue(1,_("Offset"));
    GetGrid()->SetRowLabelSize(150);
    //
    for(int i = 0; i < nrows; i++)
    {
        GetGrid()->SetCellValue(i,0,"1.0");
        GetGrid()->SetCellValue(i,1,"0.0");
    }
}

ScaleOffsetConfigurePanel::~ScaleOffsetConfigurePanel()
{
}
/*!
 * \brief ScaleOffsetConfigurePanel::read
 * \param c
 * \param p
 */
void ScaleOffsetConfigurePanel::read( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
{
    int j = 0;
    _map.read(c,p);
    for(auto i = _map.begin(); i != _map.end(); i++, j++)
    {
        MRL::ScaleOffset &o = i->second;
        GetGrid()->SetRowLabelValue(j,i->first);
        GetGrid()->SetCellValue(j,0,wxString::Format("%8.3f",o.getScale()));
        GetGrid()->SetCellValue(j,1,wxString::Format("%8.3f",o.getOffset()));
    }
}
/*!
 * \brief ScaleOffsetConfigurePanel::write
 * \param c
 * \param p
 */
void ScaleOffsetConfigurePanel::write( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
{
    int j = 0;
    for(auto i = _map.begin(); i != _map.end(); i++, j++)
    {
        MRL::ScaleOffset &o = i->second;
        double v;
        GetGrid()->GetCellValue(j,0).ToDouble(&v);
        o.setScale(v);
        GetGrid()->GetCellValue(j,1).ToDouble(&v);
        o.setOffset(v);
    }
    _map.write(c,p);
}

