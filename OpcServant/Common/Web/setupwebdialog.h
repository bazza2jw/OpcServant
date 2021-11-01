#ifndef SETUPWEBDIALOG_H
#define SETUPWEBDIALOG_H
#include <Common/common.h>
#include <Common/Web/webpanel.h>

class SetupWebDialog
{
    const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree _configuration; // tree of configuration values

public:
    SetupWebDialog();
};

#endif // SETUPWEBDIALOG_H
