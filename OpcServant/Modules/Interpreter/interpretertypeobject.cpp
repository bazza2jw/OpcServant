#include "interpretertypeobject.h"

/*
 *  Format of JSON OPC object type file
 {
    "ABC" : {type:"int",historical:1}
    "A_Folder" : {
                    type:"folder",
                    items:{
                            DEF:{type:"string"}
                          }

                 }

 }


 */

/*!
    \brief MRL::InterpreterTypeObject::parseJson
    \param v
    \return true on success
*/
bool MRL::InterpreterTypeObject::parseJson(Json::Value &v, Open62541::NodeId &parent) {
    // walk the value and interpret
    Json::Value::Members m = v.getMemberNames();
    for (int i = 0; i < int(m.size()); i++) {
        Json::Value &cv =  v[m[i]];
        if (!cv.isNull()) {
            std::string t = cv["type"].asString();
            const std::string &name = m[i];
            bool h =  cv["historical"].asBool();
            if (t == "int") {
                 add<int>(name, h, parent);
            }
            else if (t == "double") {
                add<double>(name, h, parent);
            }
            else if (t == "bool") {
                add<bool>(name, h, parent);
            }
            else if (t == "string") {
                add<std::string>(name, h, parent);
            }
            else if (t == "folder") {
                Json::Value &items = cv["items"]; // get the sub folder
                if(!items.isNull())
                {
                    Open62541::NodeId n;
                    addObjectTypeFolder(name, parent, n);
                    parseJson(items,n);
                }
            }
        }
    }
    // if it is a folder - recurse
    return true;
}
/*!
    \brief MRL::InterpreterTypeObject::addChildren
    \param parent
    \return
*/
bool MRL::InterpreterTypeObject::addChildren(Open62541::NodeId &parent) {
    Open62541::NodeId n;
    //
    // get the file name
    std::string fn = Common::baseDir() + "/Interpreter/" + name() + ".opc";
    std::ifstream is(fn);
    if (is.is_open()) {
        // find the OPC layout file
        // open it
        // Walk the file
        // fields can be bool,int,double,string or folder
        //
        try {
            Json::Value cfg;
            is >> cfg;
            return parseJson(cfg,parent);
        }
        catch (...) {
            wxLogDebug("Error Reading JSON File");
        }
    }
    return false;
}

/*!
    \brief MRL::InterpreterTypeObject::addInstance
    \param n
    \param parent
    \param nodeId
    \param requestNodeId
    \param context
    \return
*/
bool MRL::InterpreterTypeObject::addInstance(const std::string &n,
                                             const Open62541::NodeId &parent,
                                             Open62541::NodeId &nodeId,
                                             const Open62541::NodeId &requestNodeId,
                                             Open62541::NodeContext *context) {
    if (Open62541::ServerObjectType::addInstance(n, parent, nodeId, requestNodeId, context)) {

        return true;
    }
    return false;
}
