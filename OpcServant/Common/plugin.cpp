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
#include "plugin.h"


MRL::Plugin::MAP MRL::Plugin::_map; //!< map of plugins

/*!
 * \brief MRL::Plugin::Plugin
 * \param name
 */
MRL::Plugin::Plugin(const std::string &name)
{
    // register
    std::unique_ptr<Plugin> ptr(this); // add to map - self register
    _map.insert(std::make_pair(name,std::move(ptr)));
}

/*!
 * \brief MRL::Plugin::~Plugin
 */
MRL::Plugin::~Plugin()
{
    // should unload
}

/*!
 * \brief MRL::Plugin::load
 * \param dir
 * \param f
 * \return true on success
 */
bool MRL::Plugin::load(wxString &dir, wxString &f) // load all plugins in given directory
{
    // load plugins from directory
    wxString fpath = wxString::Format("%s/%s",dir,f);
    _library.Load(fpath);
    bool ret =  _library.IsLoaded();
    return ret;
}

/*!
 * \brief MRL::Plugin::loadPlugins
 * \param dir
 * \return true on success
 */
bool MRL::Plugin::loadPlugins(const std::string &dir)
{
    try
    {
        wxString dw(dir);
        wxDir d(dir);
        if(d.IsOpened())
        {
            wxString f;
#ifdef WIN32
            if(d.GetFirst (&f, "*.dll", wxDIR_FILES ))
#else
            if(d.GetFirst (&f, "*.so", wxDIR_FILES ))
#endif
            {
                do
                {
                    PropertyPath p;
                    p.push_back(PLUGIN_PATH);
                    p.push_back(f.ToStdString());
                    if(!GLOBALS().exists(p)) // does it exist?
                    {
                        auto a = new Plugin(f.ToStdString());
                        if(a->load(dw,f)) // load the plugin
                        {
                            wxLogDebug("Loaded %s",f);
                        }
                        else
                        {
                            wxLogDebug("Failed To Load %s",f);
                        }
                    }
                } while(d.GetNext(&f));
            }
        }
        return true;
    }
    catch(...)
    {
        wxLogError("Exception loading libraries");
    }
    return false;
}


//
/*!
 * \brief callAllPlugins
 * \param func
 */
void MRL::Plugin::callAllPlugins(std::function<void (Plugin *)> func)
{
       for(auto i = _map.begin(); i != _map.end(); i++)
       {
            Plugin * p = (i->second).get();
            if(p)
            {
                func(p);
            }
       }
}

/*!
 * \brief initialiseAll
 */
void MRL::Plugin::initialiseAll()
{
   auto f = [](Plugin * p) {if (p) p->initialise();};
   callAllPlugins(f);
}
/*!
 * \brief terminateAll
 * Full close down sequence
 */
void MRL::Plugin::terminateAll()
{
       auto f = [](Plugin * p) {
           if (p)
           {
               p->terminate(); // ask plugins to terminate
               p->library().Unload(); // unload dll
           }
       };
       callAllPlugins(f); // terminate and unload
}
/*!
 * \brief initialiseAllGui
 */
void MRL::Plugin::initialiseAllGui()
{
   auto f = [](Plugin * p) {if (p) p->initialiseGui();};
   callAllPlugins(f);
}
/*!
 * \brief initialiseAllOpc
 */
void MRL::Plugin::initialiseAllOpc()
{
   auto f = [](Plugin * p) {if (p) p->initialiseOpc();};
   callAllPlugins(f);
}
/*!
 * \brief initialiseAllWeb
 */
void MRL::Plugin::initialiseAllWeb()
{
   auto f = [](Plugin * p) {if (p) p->initialiseWeb();};
   callAllPlugins(f);
}
/*!
 * \brief initialiseAllDaq
 */
void MRL::Plugin::initialiseAllDaq()
{
   auto f = [](Plugin * p) {if (p) p->initialiseDaq();};
   callAllPlugins(f);
}
