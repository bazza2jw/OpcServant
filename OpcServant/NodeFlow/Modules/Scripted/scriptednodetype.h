#ifndef SCRIPTEDNODETYPE_H
#define SCRIPTEDNODETYPE_H

#include <MrlLib/luascript.h>
#include "NodeFlow/NodeFlow/nodeflow.h"
#include "NodeFlow/NodeFlow/nodetype.h"
#include "NodeFlow/NodeFlow/nodeset.h"
#include <wx/timer.h>
#include "NodeFlow/NodeFlow/PropertiesEditorDialog.h"


namespace NODEFLOW
{
    /*!
     * \brief The ScriptedNodeType class
     * LUA scripted nodes
     */
    class ScriptedNodeType : public NodeType
    {
        //
        LUASCRIPTPTR _state;
        std::string _scriptName;
        sel::State & state() { return *_state; }
        void trace(std::string s) { wxLogMessage("Script:%s",s.c_str());}
        virtual bool setupApi(); // set up the API links so the LUA script can use C++ functions and C++ can call LUA functions
        //

    public:
        ScriptedNodeType(const std::string &s) : NodeType(s), _scriptName(s)
        {
            _state = std::make_unique<sel::State>(true);
            std::string f = NODEFLOW_ROOT_DIR "/scripts/" + _scriptName + ".lua";
            _state->Load(f); // load the script
        }
        //
        virtual const char * nodeClass() const { return "Scripted";}
        //
        virtual void start(NodeSet &ns,  NodePtr &n);
        void setupConnections();
        //
        virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data);
        //
        virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
        virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
        virtual void load(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p);
        virtual void save(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p);
        //
        //

    };
}
#endif // SCRIPTEDNODETYPE_H
