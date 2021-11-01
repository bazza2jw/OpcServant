#include "interpreterobjectmanager.h"
#include <Common/Web/webdialog.h>
#include <Common/Daq/daqcommon.h>
#include <open62541cpp/open62541server.h>

/*!
    \brief MRL::InterpreterObjectManager::InterpreterObjectManager
*/
MRL::InterpreterObjectManager::InterpreterObjectManager(const std::string &n) : ObjectManager(n) {
    // load the configuration for the
}

/*!
    \brief MRL::InterpreterObjectManager::createRT
    \param path
    \return
*/
bool MRL::InterpreterObjectManager::createRT(const PropertyPath &path) {
    if (ObjectManager::createRT(path)) {
        return true;
    }
    return false;
}

/*!
    \brief MRL::InterpreterObjectManager::create
    \param path
    \return
*/
bool MRL::InterpreterObjectManager::create(const PropertyPath &path) {
    if (ObjectManager::create(path)) {

        return true;
    }
    return false;
}

/*!
    \brief MRL::InterpreterObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::InterpreterObjectManager::edit(wxWindow *w, const PropertyPath &s) {
   return false;
}
/*!
    \brief MRL::InterpreterObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::InterpreterObjectManager::edit(Wt::WWidget *w, const PropertyPath &path) {

    return true;
}

/*!
    \brief MRL::InterpreterObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::InterpreterObjectManager::view(wxWindow */*parent*/, const PropertyPath &/*s*/) {
    return false;
}
/*!
    \brief MRL::InterpreterObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::InterpreterObjectManager::view(Wt::WWidget */*parent*/, const PropertyPath &/*s*/) {
    return false;
}

/*!
    \brief MRL::InterpreterObjectManager::update
    \param s
    \return
*/
bool MRL::InterpreterObjectManager::update(const PropertyPath &/*s*/) { // updated as an object so includes all children
    return false;
}

/*!
 * \brief MRL::InterpreterObjectManager::createRtObject
 * \param id
 * \param r
 * \return true on success
 */
bool MRL::InterpreterObjectManager::createRtObject(unsigned id, RtObjectRef &r)
{
   wxLogDebug("InterpreterObjectManager::createRtObject id %d",id);
   return true;
}


