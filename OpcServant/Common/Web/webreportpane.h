#ifndef WEBREPORTPANE_H
#define WEBREPORTPANE_H

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WGridLayout.h>
#include <Wt/WToolBar.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTree.h>
#include <Wt/WIconPair.h>
#include <Wt/WTreeNode.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WPushButton.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WDateEdit.h>
#include <Common/reporter.h>
#include <Common/reportconfig.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WDialog.h>
#include <Wt/WLink.h>

namespace MRL {
/*!
 * \brief The WebReportCompleteDialog class
 */
class WebReportCompleteDialog : public Wt::WDialog
{
    Wt::WLink _link;
public:
    /*!
     * \brief WebReportCompleteDialog
     * \param url
     */
    WebReportCompleteDialog( const std::string &url) : Wt::WDialog("View Report"), _link(url)
    {
        setClosable(true);
        setResizable(true);
        setTitleBarEnabled(true);
        setMovable(true);
        setVerticalAlignment (Wt::AlignmentFlag::Middle);
        Wt::WPushButton *ok = footer()->addWidget(std::make_unique<Wt::WPushButton>("View Report"));
        ok->setDefault(true);
        Wt::WPushButton *cancel = footer()->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
        rejectWhenEscapePressed();
        /*
            Accept the dialog
        */
        ok->clicked().connect([ = ] {
            accept();
        });

        _link.setTarget(Wt::LinkTarget::NewWindow); // open in new tab on browser
        ok->setLink(_link);

        /*
            Reject the dialog
        */
        cancel->clicked().connect(this, &Wt::WDialog::reject);
    }
};

/*!
 * \brief The WebReportPane class
 */
class WebReportPane  : public Wt::WContainerWidget
{
    //
    // The web version of the report generator creates a PDF that can be viewed in the browser
    // The Desktop GUI version is interactive
    //
    Wt::WTabWidget * _tabWidget  = nullptr;
    Wt::WToolBar *  _toolBar = nullptr;
    //
    // Setup
    Wt::WComboBox * _reportList = nullptr;
    Wt::WComboBox * _reportType = nullptr;
    Wt::WDateEdit * _startDate = nullptr;
    Wt::WTimeEdit * _startTime = nullptr;
    Wt::WDateEdit * _endDate = nullptr;
    Wt::WTimeEdit * _endTime = nullptr;
    // Values list
    Wt::WSelectionBox * _valueList  = nullptr; // list of available values
    Wt::WSelectionBox * _aliasList  = nullptr; // list of available values
    // Buttons
    Wt::WPushButton * _newReport = nullptr;
    Wt::WPushButton * _loadReport = nullptr;
    Wt::WPushButton * _saveReport = nullptr;
    Wt::WPushButton * _removeReport = nullptr;
    Wt::WPushButton * _makeReport = nullptr; // this makes the report and generates the PDF that is then displayed - the browser can print this locally
    Wt::WPushButton * _viewReport = nullptr;

    /*!
     * \brief createColorButton
     * \param className
     * \param text
     */
    Wt::WPushButton * createColorButton(const char *className, const Wt::WString& text)
    {
        Wt::WPushButton *b = nullptr;
        auto button = Wt::cpp14::make_unique<Wt::WPushButton>();
        button->setTextFormat(Wt::TextFormat::XHTML);
        button->setText(text);
        button->addStyleClass(className);
        b = button.get();
        _toolBar->addButton(std::move(button));
        return b;
    }

    MRL::ReportConfig _config; // current configuration
    MRL::ReportGroup _group; // the results of a search
    std::unique_ptr<MRL::Reporter> _report; // report generator

public:
    WebReportPane();
    //
    // handlers
    void newReport();
    void loadReport();
    void saveReport();
    void removeReport();
    void makeReport();
    void toConfig();
    void updateReportList();
    void viewReport();
};

}
#endif // WEBREPORTPANE_H
