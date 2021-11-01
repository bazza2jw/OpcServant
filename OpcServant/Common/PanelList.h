/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided with open62541.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef PANEL_LIST_H_
#define PANEL_LIST_H_

#include <Wt/WContainerWidget.h>

namespace Wt {
  class WPanel;
}

class PanelList : public Wt::WContainerWidget
{
public:
  PanelList();

  Wt::WPanel *addWidget(const Wt::WString& text, std::unique_ptr<Wt::WWidget> w);
  void addPanel(std::unique_ptr<Wt::WPanel> panel);
  void removePanel(Wt::WPanel *panel);

  using WContainerWidget::addWidget;

private:
  void onExpand(bool notUndo, Wt::WPanel *panel);
  //void onExpand(bool notUndo);

  int wasExpanded_;
};

#endif // PANEL_LIST_H_
