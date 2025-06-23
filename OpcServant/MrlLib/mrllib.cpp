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
#include "mrllib.h"
#include "propertytree.h"
#include <wx/propgrid/manager.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/advprops.h>

/*!
 * \brief MRL::toList
 * \param s
 * \param l
 * \param seperator
 */

void MRL::toList(const std::string  &s, StringVector &l, const char *seperator ) {
    boost::char_separator<char> sep(seperator);
    tokenizer tokens(s, sep);
    for (auto i = tokens.begin(); i != tokens.end(); i++) {
        l.push_back(*i);
    }
}


/*!
 * \brief getTextWidth
 * \param f
 * \param n
 * \return
 */
int MRL::getTextWidth(wxFont &f, int n)
{
    wxSize sz = f.GetPixelSize();
    return sz.GetWidth() * n;
}


/*!
 * \brief getFileList
 * \param sp
 * \return number of files
 */
int MRL::getFileList(wxArrayString &sp, const wxString &dir, const wxString &filter )
{
    wxDir d(dir);
    int res = 0;
    if (d.IsOpened()) {
        wxString f;
        if (d.GetFirst(&f, filter, wxDIR_FILES)) {
            do {
                sp.Add(f);
                res++;
            }
            while (d.GetNext(&f));
        }
    }
    return res;
}
/*!
 * \brief getFileList
 * \param sp
 * \return number of files
 */
int MRL::getFileList(std::list<std::string> &sp, const std::string &dir, const std::string &filter)
{
    wxDir d(dir);
    int res = 0;
    if (d.IsOpened()) {
        wxString f;
        if (d.GetFirst(&f, filter, wxDIR_FILES)) {
            do {
                sp.push_back(f.ToStdString());
                res++;
            }
            while (d.GetNext(&f));
        }
    }
    return res;
}



/*!
 * \brief getSerialPortList
 * abstract for OS here
 * \param sp
 * \return number of ports found
 */
int MRL::getSerialPortList(wxArrayString &sp)
{
    wxDir d("/dev");
    int res = 0;
    if (d.IsOpened()) {
        wxString f;
        // if no udev rules for WC4A boards
        if (d.GetFirst(&f, "ttyUSB*", wxDIR_FILES)) {
            do {

                sp.Add(f.Prepend("/dev/"));
                res++;
            }
            while (d.GetNext(&f));
        }

        if (d.GetFirst(&f, "ttyACM*", wxDIR_FILES)) {
            do {
                sp.Add(f.Prepend("/dev/"));
                res++;
            }
            while (d.GetNext(&f));
        }


        if (d.GetFirst(&f, "ttyS*", wxDIR_FILES)) {
            do {
                sp.Add(f.Prepend("/dev/"));
                res++;
            }
            while (d.GetNext(&f));
        }


        // udev rules can map board serial ports
        if (d.GetFirst(&f, "ttyFixed*", wxDIR_FILES)) {
            do {
                sp.Add(f.Prepend("/dev/"));
                res++;
            }
            while (d.GetNext(&f));
        }
    }
    return res;
}

/*!
 * \brief MRL::getSerialPortList
 * \param sp
 * \return
 */
int MRL::getSerialPortList(std::list<std::string> &sp)
{
    wxDir d("/dev");
    int res = 0;
    if (d.IsOpened()) {
        wxString f;
        // if no udev rules
        if (d.GetFirst(&f, "ttyUSB*", wxDIR_FILES)) {
            do {
                sp.push_back(f.Prepend("/dev/").ToStdString());
                res++;
            }
            while (d.GetNext(&f));
        }

        if (d.GetFirst(&f, "ttyACM*", wxDIR_FILES)) {
            do {
                sp.push_back(f.Prepend("/dev/").ToStdString());
                res++;
            }
            while (d.GetNext(&f));
        }

        if (d.GetFirst(&f, "ttyS*", wxDIR_FILES)) {
            do {
                sp.push_back(f.Prepend("/dev/").ToStdString());
                res++;
            }
            while (d.GetNext(&f));
        }


        // udev rules can map board serial ports
        if (d.GetFirst(&f, "ttyFixed*", wxDIR_FILES)) {
            do {
                sp.push_back(f.Prepend("/dev/").ToStdString());
                res++;
            }
            while (d.GetNext(&f));
        }
    }
    return res;
}



static uint8_t charToNibble(int8_t c)
{
    uint8_t v = c & 0xF;
    if(c > '@')
    {
        v += 9;
    }
    return v;
}

static uint8_t hexToByte(int8_t a, int8_t b)
{
    uint8_t v = charToNibble(b);
    v |= (charToNibble(a) << 4);
    return v;
}

// l is number of binary bytes
void MRL::stringToBin(const int8_t *s, uint8_t *b, int l)
{
    int j = 0;
    for(int i = 0; i < l; i++, j += 2)
    {
        b[i] = hexToByte(s[j], s[j+1]);
    }
}

static int8_t nibbleToAscii[] = "0123456789ABCDEF";
void  MRL::bytesToString(uint8_t *b, int8_t *s, int l)
{
    int j = 0;
    for(int i = 0; i < l; i++)
    {
        s[j++] = nibbleToAscii[b[i] >> 4];
        s[j++] = nibbleToAscii[b[i] & 0x0F];
    }
}


/*!
 * \brief MRL::stringToBool
 * \param s
 * \return
 */
bool MRL::stringToBool(const std::string &s)
{
    static const char *trueStr[] = {"True","true","1"};
    for(int i = 0; i < 3; i++)
        if(s == trueStr[i]) return true;
    return false;
}
/*!
 * \brief MRL::stringTimeToInt
 * \param s
 * \return
 */
int  MRL::stringTimeToInt(const std::string &s) {
    int ret = 0;
    boost::char_separator<char> sep(":");
    tokenizer tokens(s, sep);
    std::vector<std::string> l;
    for (auto i = tokens.begin(); i != tokens.end(); i++) {
        l.push_back(*i);
    }

    switch (l.size()) {
        case 1:
            ret = std::stoi(l[0]);
            break;
        case 2:
            ret = std::stoi(l[1]) * 60 + std::stoi(l[0]);
            break;
        case 3:
            ret = std::stoi(l[0]) * 3600 + std::stoi(l[1]) * 60 + std::stoi(l[2]) ;
            break;
        default:
            break;
    }
    return ret;
}
#ifdef USE_GUI

/*!
    \brief SetChoice
    \param w wxChoice control
    \param s string to find and set index for
*/
void MRL::SetChoice(wxChoice *w, const std::string &s) {
    if (w->GetCount() > 0) {
        wxString f(s);
        int i = w->FindString(f);
        if (i != wxNOT_FOUND) w->SetSelection(i);
    }
}

/*!
    \brief GetChoice
    \param w wxChoice control
    \return selected string
*/
std::string MRL::GetChoice(wxChoice *w) {
    int i = w->GetCurrentSelection();
    if (i != wxNOT_FOUND) {
        return w->GetStringSelection().ToStdString();
    }
    return std::string();
}

/*!
 * \brief SetChoice
 * \param c
 * \param s
 */
void MRL::SetChoice(Wt::WComboBox *c, const std::string &s)
{
    int i = c->findText(s);
    if(i >= 0)
    {
        c->setCurrentIndex(i);
    }
    else
    {
        c->setCurrentIndex(0);
    }
}

std::string MRL::GetChoice(Wt::WComboBox *c)
{
    return c->currentText().toUTF8();
}

/*!
 * \brief findProperty
 * \param manager
 * \return
 */
wxPGProperty * MRL::findProperty(wxPropertyGridManager *manager, const std::string &name)
{
    wxPGVIterator it;
    for ( it = manager->GetVIterator(wxPG_ITERATE_ALL);
          !it.AtEnd();
          it.Next() )
    {
        wxPGProperty *p = it.GetProperty();
        if(p->GetName().ToStdString() == name)
            return p;
    }
    return nullptr;
}


/*!
 * \brief propertiesToJson
 * \param manager
 * \param v
 */
void MRL::propertiesToJson(wxPropertyGridManager *manager, Json::Value &v)
{
    wxPGVIterator it;
    for ( it = manager->GetVIterator(wxPG_ITERATE_ALL);
          !it.AtEnd();
          it.Next() )
    {
        wxPGProperty* p = it.GetProperty();
        v[p->GetName().ToStdString()] = p->GetValueAsString().ToStdString();
    }
}

/*!
 * \brief jsonToProperties
 * \param manager
 * \param v
 */
void MRL::jsonToProperties(wxPropertyGridManager *manager, Json::Value &v)
{
    if(!v.isNull())
    {
        wxPGVIterator it;
        for ( it = manager->GetVIterator(wxPG_ITERATE_ALL);
              !it.AtEnd();
              it.Next() )
        {
            wxPGProperty* p = it.GetProperty();

            Json::Value s = v[p->GetName().ToStdString()];
            if(!s.isNull())
            {
                std::string a = s.asString();
                if(!a.empty())
                {
                    p->SetValueFromString(a);
                }
            }
        }
    }
}
/*!
 * \brief clearProperties
 * \param manager
 */
void MRL::clearProperties(wxPropertyGridManager *manager)
{
    wxPGVIterator it;
    for ( it = manager->GetVIterator(wxPG_ITERATE_ALL);
          !it.AtEnd();
          it.Next() )
    {
        wxPGProperty* p = it.GetProperty();
        p->SetValueFromString("");
    }
}
#endif



