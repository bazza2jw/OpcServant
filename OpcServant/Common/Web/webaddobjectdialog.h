#ifndef WEBADDOBJECTDIALOG_H
#define WEBADDOBJECTDIALOG_H
#include "webdialog.h"
#include <Wt/WDialog.h>
#include <Wt/WComboBox.h>
#include <Wt/WLineEdit.h>
#include <Wt/WGridLayout.h>
#include <Common/objectmanager.h>
#include <Wt/WToolBar.h>
namespace MRL {
    /*!
        \brief The WebAddObjectForm class
    */
    class WebAddObjectForm : public WebDialogBase {
            PropertyPath _path;
            Wt::WComboBox *_type = nullptr;
            Wt::WLineEdit *_name = nullptr;
            std::list<std::string> _currentNames; //!< list of children

        public:
            WebAddObjectForm(const PropertyPath &path) :
                _path(path) {

            }
            void setup();
            Wt::WComboBox *type() const {
                return  _type;
            }
            Wt::WLineEdit *name() const {
                return  _name;
            }
            void onOk() {
                //
                // dialog is model but not does not block the add item function
                // It is recommended way to use Wt dialogs
                std::string n = name()->text().toUTF8();
                std::string t = type()->currentText().toUTF8();
                _path.push_back(n);
                ObjectManager *o = ObjectManager::find(t);
                if (o) {
                    if (o->create(_path)) {
                        MRL::Common::configuration().saveToDb();
                    }
                }
            }
    };
    /*!
     * \brief createColorButton
     * \param toolBar
     * \param className
     * \param text
     * \return push button - this has been added to the toolbar
     */
    inline Wt::WPushButton * createColorButton(Wt::WToolBar * toolBar, const char *className, const std::string & text)
    {
        auto button = Wt::cpp14::make_unique<Wt::WPushButton>();
        button->setTextFormat(Wt::TextFormat::XHTML);
        button->setText(text);
        button->addStyleClass(className);
        //
        Wt::WPushButton * b = button.get();
        toolBar->addButton(std::move(button));
        return b;
    }


}
#endif // WEBADDOBJECTDIALOG_H
