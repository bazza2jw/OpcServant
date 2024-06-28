#ifndef REPORTSELECTWEB_H
#define REPORTSELECTWEB_H
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

namespace MRL
{
/*!
     * \brief The ReportSelectWeb class
     * This allows only the selection and generation of a prepared report
     * This is intended for insertion into an IFRAME in something like WORDPRESS or Node Red
     */
    class ReportSelectWeb  : public Wt::WContainerWidget
    {
        Wt::WToolBar *  _toolBar = nullptr;
        Wt::WPushButton * _makeReport = nullptr; // this makes the report and generates the PDF that is then displayed - the browser can print this locally
        Wt::WComboBox * _reportList = nullptr;
        void makeReport();

        /*!
         * \brief createColorButton
         * \param className
         * \param text
         * \return create push button object
         */
        Wt::WPushButton * createColorButton(const char *className, const Wt::WString& text)
        {
            Wt::WPushButton *b = nullptr;
            auto button = std::make_unique<Wt::WPushButton>();
            button->setTextFormat(Wt::TextFormat::XHTML);
            button->setText(text);
            button->addStyleClass(className);
            b = button.get();
            _toolBar->addButton(std::move(button));
            return b;
        }

    public:
        ReportSelectWeb();
    };
}
#endif // REPORTSELECTWEB_H
