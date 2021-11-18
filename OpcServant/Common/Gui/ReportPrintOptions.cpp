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
#include "ReportPrintOptions.h"

/*!
 * \brief ReportPrintOptions::ReportPrintOptions
 * \param parent
 */
ReportPrintOptions::ReportPrintOptions(wxWindow* parent)
    : ReportPrintOptionsBase(parent)
{
}
/*!
 * \brief ReportPrintOptions::~ReportPrintOptions
 */
ReportPrintOptions::~ReportPrintOptions()
{
}
/*!
 * \brief ReportPrintOptions::onOk
 */
void ReportPrintOptions::onOk(wxCommandEvent& /*event*/)
{
    EndModal(wxID_OK);
}
