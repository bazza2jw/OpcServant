#ifndef ICOPCON7065SETUPDIALOG_H
#define ICOPCON7065SETUPDIALOG_H
#include "ICPCON7065UI.h"
#include <Common/common.h>
#include <Common/Gui/SetupDialog.h>
#include <wx/dir.h>

class ICPCON7065SetupDialog : public SetupNodeSerial
{
public:
    ICPCON7065SetupDialog(wxWindow* parent, const MRL::PropertyPath &path);
};
#endif // ICOPCON7065SETUPDIALOG_H
