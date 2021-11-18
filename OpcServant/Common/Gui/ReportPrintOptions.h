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
#ifndef REPORTPRINTOPTIONS_H
#define REPORTPRINTOPTIONS_H
#include "CommonGUI.h"

/*!
 * \brief The ReportPrintOptions class
 */
class ReportPrintOptions : public ReportPrintOptionsBase
{
public:
    ReportPrintOptions(wxWindow* parent);
    virtual ~ReportPrintOptions();
protected:
    virtual void onOk(wxCommandEvent& event);
};
#endif // REPORTPRINTOPTIONS_H
