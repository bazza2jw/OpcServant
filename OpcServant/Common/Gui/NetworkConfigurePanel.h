/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef NETWORKCONFIGUREPANEL_H
#define NETWORKCONFIGUREPANEL_H
#include "BourbonCommonUI.h"
#include <Common/common.h>
/*!
 * \brief The NetworkConfigurePanel class
 */
class NetworkConfigurePanel : public NetworkConfigurePanelBase
{
public:
    NetworkConfigurePanel(wxWindow* parent);
    virtual ~NetworkConfigurePanel();
    /*!
     * \brief read
     * \param c
     */
    virtual void read( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        GetAddress()->SetValue(c.getValue<std::string>(p,"Address"));
        GetPort()->SetValue(c.getValue<int>(p, "Port"));

    }
    /*!
     * \brief write
     * \param c
     */
    virtual void write( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
       c.setValue(p, "Address",GetAddress()->GetValue().ToStdString());
       c.setValue(p, "Port",GetPort()->GetValue());
    }
protected:
    virtual void onTest(wxCommandEvent& event);
};
#endif // NETWORKCONFIGUREPANEL_H
