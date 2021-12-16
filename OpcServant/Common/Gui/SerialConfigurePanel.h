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
#ifndef SERIALCONFIGUREPANEL_H
#define SERIALCONFIGUREPANEL_H
#include "BourbonCommonUI.h"
#include <Common/common.h>
/*!
 * \brief The SerialConfigurePanel class
 * Standard config panel for uart devices
 */
class SerialConfigurePanel : public SerialConfigurePanelBase
{
public:
    SerialConfigurePanel(wxWindow* parent);
    virtual ~SerialConfigurePanel(){}
    /*!
     * \brief read
     * \param c
     */
    virtual void read( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        MRL::SetChoice(GetDevice(), c.getValue<std::string>(p,"Device"));
        MRL::SetChoice(GetBaudrate(),c.getValue<std::string>(p,"BaudRate"));
        GetNodeAddress()->SetValue(c.getValue<int>(p,"NodeAddress"));
    }
    /*!
     * \brief write
     * \param c
     */
    virtual void write( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        c.setValue(p, "Device", MRL::GetChoice(GetDevice()));
        c.setValue(p, "BaudRate", MRL::GetChoice(GetBaudrate()));
        c.setValue(p, "NodeAdress", GetNodeAddress()->GetValue());
    }
    /*!
     * \brief showNodeAddress
     */
    void showNodeAddress()
    {
        GetNodeAddress()->Show();
        m_nodeLabel->Show();

    }
};
#endif // SERIALCONFIGUREPANEL_H
