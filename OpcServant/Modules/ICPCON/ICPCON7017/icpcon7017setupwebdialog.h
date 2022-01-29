/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef ICPCON7017SETUPWEBDIALOG_H
#define ICPCON7017SETUPWEBDIALOG_H
#include <Common/Web/webpanel.h>
#include "icpcon7017rt.h"
namespace MRL {
/*!
     * \brief The ICPCON7017SetupWebDialog class
     */
    class ICPCON7017SetupWebDialog : public ConfigureSerialNodeWebDialog
    {
        ScaleOffsetWebPanel * _panel = nullptr;
    public:
        ICPCON7017SetupWebDialog(const PropertyPath &path) :  ConfigureSerialNodeWebDialog(path)
        {
        }
        virtual void addPanels()
        {
            ConfigureSerialNodeWebDialog::addPanels();
            auto bp =  std::make_unique<ScaleOffsetWebPanel >(6);
            _panel = bp.get();

            for(int i = 0; i < int(MRL::ICPCON7017RT::_channelNames.size()); i++)
            {
                _panel->map()[MRL::ICPCON7017RT::_channelNames[i]] = ScaleOffset(1.0,0.0); // setup the map
            }

            addPanel(_TR("Scale Offset"),bp);
        } // add panels to dialog

        virtual void read(MRL::VariantPropertyTree &c,MRL::PropertyPath &p)
        {
            ConfigureSerialNodeWebDialog::read(c,p);
            _panel->read(c,p);
        } // read the panel data - called by setup
        virtual void write(MRL::VariantPropertyTree &c,MRL::PropertyPath &p)
        {
            ConfigureSerialNodeWebDialog::write(c,p);
            _panel->write(c,p);
        } // write panel data to the configuration - called by onOK
    };
}
#endif // ICPCON7017SETUPWEBDIALOG_H
