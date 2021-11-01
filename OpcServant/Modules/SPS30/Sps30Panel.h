#ifndef SPS30PANEL_H
#define SPS30PANEL_H
#include "SPS30UI.h"
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/bobject.h>
#include <Common/Daq/rtobject.h>

class Sps30Panel : public Sps30PanelBase, public MRL::BObject
{
    unsigned _id;
    MRL::PropertyPath _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree _configuration; // tree of configuration values

public:
    Sps30Panel(wxWindow* parent, unsigned id);
    virtual ~Sps30Panel();
    bool processQueueItem(const MRL::Message &msg);

};
#endif // SPS30PANEL_H
