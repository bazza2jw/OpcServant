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
#ifndef REPORTCONFIG_H
#define REPORTCONFIG_H

#include <math.h>
#include <stdlib.h>
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
#include <Common/reporteritem.h>

namespace MRL {
/*!
 * \brief The ReportConfig class
 */
class ReportConfig
{
    //
    VariantPropertyTree _config;
    std::string _name;
    //
public:
    enum
    {
        TF_LAST_HOUR = 0, TF_LAST_DAY, TF_LAST_WEEK, TF_EXACT
    };

    constexpr static const char * reportDir = "/data/reports";
    constexpr static const char * reportExt = ".rpt";
    ReportConfig() {}
    bool load(const std::string &n);
    bool save();
    bool saveAs(const std::string &n);
    bool toReportGroup(ReportGroup &);
    bool deleteReport(const std::string &n);
    VariantPropertyTree & config() {
        return _config;
    }
    const std::string & name() const {
        return _name;
    }
};


}

#endif // REPORTCONFIG_H
