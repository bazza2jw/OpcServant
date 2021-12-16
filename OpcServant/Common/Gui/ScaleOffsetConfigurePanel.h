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
