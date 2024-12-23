#ifndef HARNESSTESTDIALOG_H
#define HARNESSTESTDIALOG_H
#include "HarnessUI.h"
#include <MrlLib/sqlitedb.h>

class HarnessTestDialog : public HarnessTestDialogBase
{
    std::string _dbDir;
    std::string _dbName;
    MRL::SQLiteDB  * _db = nullptr;
    MRL::SqliteScroll * _scroll = nullptr;
public:
    HarnessTestDialog(wxWindow* parent);
    virtual ~HarnessTestDialog();
protected:
    virtual void onFileSelected(wxFileDirPickerEvent& event);
    virtual void onBackwards(wxCommandEvent& event);
    virtual void onForwards(wxCommandEvent& event);
    virtual void onCancel(wxCommandEvent& event);
    virtual void onOk(wxCommandEvent& event);

};
#endif // HARNESSTESTDIALOG_H
