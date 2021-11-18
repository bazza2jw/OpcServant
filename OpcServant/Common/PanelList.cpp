/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#include <Wt/WPanel.h>
#include "PanelList.h"

using namespace Wt;
/*!
 * \brief PanelList::PanelList
 */
PanelList::PanelList()
  : WContainerWidget()
{ }

/*!
 * \brief PanelList::addWidget
 * \param text
 * \param w
 * \return
 */
WPanel *PanelList::addWidget(const WString& text, std::unique_ptr<WWidget> w)
{
  std::unique_ptr<WPanel> p
      = cpp14::make_unique<WPanel>();
  WPanel *result = p.get();
  p->setTitle(text);
  p->setCentralWidget(std::move(w));

  addPanel(std::move(p));

  return result;
}

/*!
 * \brief PanelList::addPanel
 * \param panel
 */
void PanelList::addPanel(std::unique_ptr<WPanel> panel)
{
  panel->setCollapsible(true);
  panel->collapse();

  panel->expandedSS().connect(std::bind(&PanelList::onExpand, this, std::placeholders::_1, panel.get()));

  WContainerWidget::addWidget(std::move(panel));
}

/*!
 * \brief PanelList::onExpand
 * \param notUndo
 * \param panel
 */
void PanelList::onExpand(bool notUndo, WPanel *panel)
{
  if (notUndo) {
    wasExpanded_ = -1;

    for (unsigned i = 0; i < children().size(); ++i) {
      WPanel *p = dynamic_cast<WPanel *>(children()[i]);
      if (p != panel) {
        if (!p->isCollapsed())
          wasExpanded_ = i;
        p->collapse();
      }
    }
  } else {
    if (wasExpanded_ != -1) {
      WPanel *p = dynamic_cast<WPanel *>(children()[wasExpanded_]);
      p->expand();
    }
  }
}
