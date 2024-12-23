#include "HarnessTestDialog.h"
#include <wx/filename.h>
#include "buiapp.h"

/*!
 * \brief HarnessTestDialog::HarnessTestDialog
 * \param parent
 */
HarnessTestDialog::HarnessTestDialog(wxWindow* parent)
    : HarnessTestDialogBase(parent)
{

}
/*!
 * \brief HarnessTestDialog::~HarnessTestDialog
 */
HarnessTestDialog::~HarnessTestDialog()
{
}
/*!
 * \brief HarnessTestDialog::onCancel
 * \param event
 */
void HarnessTestDialog::onCancel(wxCommandEvent& event)
{
    BuiApp::GetInstance()->Exit();
}
/*!
 * \brief HarnessTestDialog::onOk
 * \param event
 */
void HarnessTestDialog::onOk(wxCommandEvent& event)
{
    BuiApp::GetInstance()->Exit();
}

/*!
 * \brief HarnessTestDialog::onBackwards
 * \param event
 */
void HarnessTestDialog::onBackwards(wxCommandEvent& event)
{
    if(_db && _scroll)
    {
        _scroll->scrollBackwards(_scroll->first());
        _scroll->records().print(std::cerr);
    }
}
/*!
 * \brief HarnessTestDialog::onForwards
 * \param event
 */
void HarnessTestDialog::onForwards(wxCommandEvent& event)
{
    if(_db && _scroll)
    {
        _scroll->scrollForwards(_scroll->last());
        _scroll->records().print(std::cerr);
    }
}
/*!
 * \brief HarnessTestDialog::onFileSelected
 * \param event
 */
void HarnessTestDialog::onFileSelected(wxFileDirPickerEvent& event)
{
    // create the Sqlite object
    wxFileName f (event.GetPath());
    _dbDir = f.GetPath().ToStdString();
    //
    _dbName = f.GetFullName().ToStdString();
    //
    if(_db)
    {
        _db->CloseConnection();
        delete _db;
        _db = nullptr;
    }
    //
    if(_scroll)
    {
        delete _scroll;
        _scroll = nullptr;
    }
    //
    _db = new MRL::SQLiteDB;
    if(_db)
    {
        if(_db->OpenConnection(_dbName,_dbDir))
        {
            _scroll = new MRL::SqliteScroll(_db);
            _scroll->setCursors("F_TIMESTAMP","SELECT F_TIMESTAMP,F_SAMPLE_CONC FROM CONC_TABLE WHERE F_MODULE=1 ");
             std::cerr << " Opened Database " << _dbDir << " " << _dbName << std::endl;
            _scroll->scrollForwards("2024-12-01T00:00:00");
             _scroll->records().print(std::cerr);
        }
        else
        {
            std::cerr << "Failed to open db " << _dbDir << " " << _dbName << std::endl;
        }
    }
}
