#include "interpreterrt.h"

/*!
    \brief MRL::InterpreterRTstart
*/
void MRL::InterpreterRT::start() {
    try {
        RTObject::start();
        if (started() && _state) {
            if (_haveProcess) { // no point starting if no process function
                if (_haveStart) {
                    (*_state)["start"]();

                }
            }
        }
    }
    catch (...) {
        RTObject::stop();
    }
}

/*!
    \brief MRL::InterpreterRTstop
*/
void MRL::InterpreterRT::stop() {
    try {
        if (_state) {
            if (_haveStop) {
                (*_state)["stop"]();
            }
        }
    }
    catch (...) {

    }

    RTObject::stop();
}

/*!
    \brief MRL::InterpreterRTsetupApi
    \return
*/
void MRL::InterpreterRT::setupApi() {



    state()["Utility"].SetObj(*this,
                              "Trace", &MRL::InterpreterRT::trace
                             );
    //
    // Property tree interface
    //
    state()["Configuration"].SetObj(configuration(),
                                    "GetBool", &MRL::VariantPropertyTree::getBool,
                                    "GetInt", &MRL::VariantPropertyTree::getInt,
                                    "GetDouble", &MRL::VariantPropertyTree::getDouble,
                                    "GetString", &MRL::VariantPropertyTree::getString,
                                    "SetBool", &MRL::VariantPropertyTree::setBool,
                                    "SetInt", &MRL::VariantPropertyTree::setInt,
                                    "SetDouble", &MRL::VariantPropertyTree::setDouble,
                                    "SetString", &MRL::VariantPropertyTree::setString,
                                    "Erase", &MRL::VariantPropertyTree::erase
                                   );
    //
    state()["Runtime"].SetObj(runtime(),
                              "GetBool", &MRL::VariantPropertyTree::getBool,
                              "GetInt", &MRL::VariantPropertyTree::getInt,
                              "GetDouble", &MRL::VariantPropertyTree::getDouble,
                              "GetString", &MRL::VariantPropertyTree::getString,
                              "SetBool", &MRL::VariantPropertyTree::setBool,
                              "SetInt", &MRL::VariantPropertyTree::setInt,
                              "SetDouble", &MRL::VariantPropertyTree::setDouble,
                              "SetString", &MRL::VariantPropertyTree::setString,
                              "Erase", &MRL::VariantPropertyTree::erase);
}


/*!
    \brief MRL::InterpreterRTload
    \param file
    \return
*/
bool MRL::InterpreterRT::load(const std::string &file) {
    if (create()) { // create the state
        std::stringstream ss;
        ss << "package.path =\"" << MRL::Common::baseDir() << "/Interpreter/Scripts" << "/?.lua\""; // set the package path
        (*_state)(ss.str().c_str());
        setupApi(); // set up the basic LUA - C++ api
        if (_state.get()->Load(file)) { // load the script
            ss.str("");
            ss << "objectName=\"" << dbId() << "\""; // set the object name
            std::string p;
            path().toString(p);
            ss << " ; pathBase=\"" << p << "\""; // note the object path
            (*_state)(ss.str().c_str());
            //
            // Set the flags for what is and is not there
            //
            _haveStart = (*_state)["start"].exists();
            _haveStop = (*_state)["stop"].exists();
            _havePublish = (*_state)["publish"].exists();
            _haveMeasure = (*_state)["measure"].exists();
            _haveProcess = (*_state)["process"].exists();
            //
            return true;
        }
    }
    return false;
}


/*!
    \brief InterfaceScript::create
    \return
*/
bool MRL::InterpreterRT::create() {
    try {
        if (_state) { // delete old VM
            sel::State *p = _state.release();
            if (p) delete p;
        }
        _state = std::unique_ptr<sel::State>(new sel::State(true));

        return true;
    }
    catch (...) {
        wxLogDebug("Exception Creating sel::State");
    }
    return false;
}

/*!
    \brief MRL::InterpreterRTobjectProcess
*/
void MRL::InterpreterRT::process() {
    try {
        RTObject::process();
        if (_state) {
            if (_haveProcess) {
                (*_state)["process"]();
            }
        }
    }
    catch (...) {
        stop();
    }

}


/*!
 * \brief MRL::InterpreterRT::publish
 */
void MRL::InterpreterRT::publish() {
    try {
        RTObject::publish();
        if (_state) {
            if (_havePublish) {
                (*_state)["publish"]();
            }
        }
    }
    catch (...) {
        stop();
    }
}
/*!
    \brief measure
*/
void MRL::InterpreterRT::measure() {
    try {
        RTObject::measure();
        if (_state) {
            if (_haveMeasure) {
                (*_state)["measure"]();
            }
        }
    }
    catch (...) {
        stop();
    }
}
/*!
    \brief updateOpc
    \param server
    \param objectNode
*/
void MRL::InterpreterRT::updateOpc(Open62541::Server &server, Open62541::NodeId &objectNode) {

}
/*!
    \brief initialiseOpc
    \param server
    \param objectNode
*/
void MRL::InterpreterRT::initialiseOpc(Open62541::Server &server, Open62541::NodeId &objectNode) {

}
/*!
    \brief publishHtmlPage
*/
void MRL::InterpreterRT::publishHtmlPage(CTML::Node &s) { // publish the current runtime status as HTML text

}

/*!
    \brief measureHtmlPage
*/
void MRL::InterpreterRT::measureHtmlPage(CTML::Node &s) {

}



