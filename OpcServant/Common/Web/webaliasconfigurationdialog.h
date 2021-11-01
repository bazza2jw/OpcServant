#ifndef WEBALIASCONFIGURATIONDIALOG_H
#define WEBALIASCONFIGURATIONDIALOG_H
#include <Common/Web/webdialog.h>

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
#include <Wt/WSelectionBox.h>
#include <Wt/WDialog.h>
#include <Wt/WLink.h>
#include <Common/Web/webdatamodel.h>
#include <Common/Web/webaddobjectdialog.h>
#include <Wt/WVBoxLayout.h>



namespace MRL
{
    /*!
     * \brief The WebAliasConfigurationDialog class
     */
    class WebAliasConfigurationDialog : public WebDialogBase
    {

        class WebAddAliasDialog  : public WebDialogBase
        {
        public:
            Wt::WLineEdit * _alias = nullptr;
            Wt::WComboBox * _valueList = nullptr;
            std::string alias;
            std::string value;
            WebAddAliasDialog() {}
            virtual void onOk();
            virtual void setup();
        };


        Wt::WPushButton * _add = nullptr;
        Wt::WPushButton * _edit = nullptr;
        Wt::WPushButton * _remove = nullptr;
        Wt::WSelectionBox * _aliasList  = nullptr; // list of available values
        Wt::WToolBar *  _toolBar = nullptr;
        // set up the model to list the aliases
        std::shared_ptr<Wt::WStandardItemModel> _model;
    public:
        WebAliasConfigurationDialog();
        virtual void setup() ;
        void add();
        void edit();
        void remove();
    };
}
#endif // WEBALIASCONFIGURATIONDIALOG_H
