#ifndef SPS30PANELWEB_H
#define SPS30PANELWEB_H
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
#include <Wt/WFitLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTextArea.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WStandardItem.h>
#include <Wt/WTimer.h>
#include <Wt/WTable.h>

#define OK_ICON "icons/ledgreen.png"
#define FAULT_ICON "icons/ledred.png"

namespace MRL
{
/*!
     * \brief The Sps30PanelWeb class
     */
    class Sps30PanelWeb : public Wt::WContainerWidget
    {
        Wt::WTable * _table = nullptr;
        unsigned _id = 0;
    public:
        Sps30PanelWeb(int id);
    };
}
#endif // SPS30PANELWEB_H
