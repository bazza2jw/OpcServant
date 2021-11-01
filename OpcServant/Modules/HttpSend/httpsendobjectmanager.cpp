#include "httpsendobjectmanager.h"
#include <Common/Web/webdialog.h>
#include "httpsendrt.h"
#include <Common/Daq/daqcommon.h>
#include <Common/objectmanager.h>
#include "HttpSendSetupDialog.h"
#include "httpsendwebsetupdialog.h"

MRL::HttpSendObjectManager::HttpSendObjectManager() : ObjectManager("HttpSend")
{

}

/*!
    \brief MRL::HttpSendObjectManager::createRT
    \param path
    \return
*/
bool MRL::HttpSendObjectManager::createRT(const PropertyPath &path) {
    if (ObjectManager::createRT(path)) {
        return true;
    }
    return false;
}

/*!
    \brief MRL::HttpSendObjectManager::create
    \param path
    \return
*/
bool MRL::HttpSendObjectManager::create(const PropertyPath &path) {
    if (ObjectManager::create(path)) {

        return true;
    }
    return false;
}

/*!
    \brief MRL::HttpSendObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::HttpSendObjectManager::edit(wxWindow *w, const PropertyPath &s) {
    HttpSendSetupDialog dlg(w,s);
    return dlg.ShowModal();
}
/*!
    \brief MRL::HttpSendObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::HttpSendObjectManager::edit(Wt::WWidget *w, const PropertyPath &path) {
    WebDialogBase::showDialog<MRL::HttpSendWebSetupDialog>(w, path);
    return true;
}

/*!
    \brief MRL::HttpSendObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::HttpSendObjectManager::view(wxWindow */*parent*/, const PropertyPath &/*s*/) {
    return false;
}
/*!
    \brief MRL::HttpSendObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::HttpSendObjectManager::view(Wt::WWidget */*parent*/, const PropertyPath &/*s*/) {
    return false;
}

/*!
    \brief MRL::HttpSendObjectManager::update
    \param s
    \return
*/
bool MRL::HttpSendObjectManager::update(const PropertyPath &/*s*/) { // updated as an object so includes all children
    return false;
}

/*!
 * \brief MRL::HttpSendObjectManager::createRtObject
 * \param id
 * \param r
 * \return true on success
 */
bool MRL::HttpSendObjectManager::createRtObject(unsigned id, RtObjectRef &r)
{
   wxLogDebug("HttpSendObjectManager::createRtObject id %d",id);
   RtObjectRef o( new HttpSendRt(id));
   r = std::move(o); // assign across
   return true;
}
