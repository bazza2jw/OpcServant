#include "sps30panelweb.h"

/*!
 * \brief MRL::Sps30PanelWeb::Sps30PanelWeb
 * \param id
 * \param slotId
 * \param m
 */
MRL::Sps30PanelWeb::Sps30PanelWeb(int id) :
    _id(id),
    _slotId(slotId)
{
    auto layout = std::make_unique<Wt::WFitLayout>();
    _table = layout->addWidget(std::make_unique<Wt::WTable>());
    //
    Wt::WLength rowHeight(50,Wt::LengthUnit::Pixel);
    Wt::WLength tableSize(50,Wt::LengthUnit::ViewportWidth );
    //
    Wt::WCssDecorationStyle s;
    Wt::WFont f;
    f.setSize(Wt::FontSize::XXLarge);
    s.setFont(f);
    //
    _table = boxLayout->addWidget(std::make_unique<Wt::WTable>(),2);
    _table->setHeaderCount(1);
    _table->setDecorationStyle(css);
    _table->setWidth(Wt::WLength("100%"));
    _table->addStyleClass("table form-inline");

    //
    setLayout(std::move(layout));
    resize(tableSize,tableSize);

}
