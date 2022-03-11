#ifndef FUNCTIONTYPENODE_H
#define FUNCTIONTYPENODE_H
/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include <functional>
#include <NodeFlow/NodeFlow/nodetype.h>
#include <NodeFlow/NodeFlow/nodeset.h>
#include <NodeFlow/NodeFlow/fparser.hh>
#include <NodeFlow/NodeFlow/PropertiesEditorDialog.h>
#include <NodeFlow/NodeFlow/webproperties.h>
#include <MrlLib/luascript.h>

// function nodes are generated from the functions definitions file on startup
// functions are for double values

namespace NODEFLOW
{




template <typename T, typename R, ConnectionType inType = Any, ConnectionType outType = Any>
/*!
 * \brief The BinaryTypeNode class
 */
class BinaryTypeNode : public NodeType // two inputs one output
{
public:
    typedef std::function<R (T,T)> BINARY_OPFUNC;
    enum
    {
        InputA = 0,
        InputB,
        Output = 0
    };

    BINARY_OPFUNC _op;
    ConnectionType _inType = inType;


public:

    BinaryTypeNode(const std::string s, BINARY_OPFUNC f) : NodeType(s),_op(f)
    {

    }
    virtual const char * nodeClass() const {
        return "Math Operators";
    }
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(2);
        //
        inputs()[0] = Connection("A",Single,inType);
        inputs()[1] = Connection("B",Single,inType);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,outType);
    }
    /*!
     * \brief start
     * \param n
     */
    virtual void start(NodeSet &ns,  NodePtr &n)
    {
        NodeType::start(ns,n);
        n->data()["A"] = T(0);
        n->data()["OUT"] = T(0);
        MRL::PropertyPath p;
        n->toPath(p);
        n->data()["HaveA"] = false;
        n->data()["HaveB"] = ns.data().getValue<bool>(p,"UseDefaultB");
        n->data()["B"] =  ns.data().getValue<T>(p,"DefaultB");
    }

    /*!
     * \brief process
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return true on success
     */
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);

        if(n && n->enabled())
        {
            bool haveA = n->data()["HaveA"].asBool();
            bool haveB = n->data()["HaveB"].asBool();
            switch(id)
            {
            case InputA:
                n->data()["A"] = data[DATA_PAYLOAD].as<T>();
                haveA = true;
                n->data()["HaveA"] = haveA;
                break;
            case InputB:
                n->data()["B"] = data[DATA_PAYLOAD].as<T>();
                haveB = true;
                n->data()["HaveB"] = haveB;
                break;
            default:
                break;
            }

            if(haveA && haveB)
            {
                R r = op(n->data()["A"].as<T>(), n->data()["B"].as<T>());
                VALUE result;
                setValueData(data,r,result);
                return post(ns,nodeId,Output,result);
            }
        }
        return false;
    }

    /*!
     * \brief evaluate
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return
     */
    bool evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data )
    {
        NodePtr  &n = ns.findNode(nodeId);
        if(n && n->enabled() && (id == Output))
        {
            VALUE a;
            VALUE b;
            if(calculate(ns,nodeId,InputA,a) && calculate(ns,nodeId,InputB,b))
            {
                R r = op(a[DATA_PAYLOAD].as<T>(),b.as<T>()) ;
                data[DATA_PAYLOAD] = r;
            }
        }
        return false;
    }

    /*!
     * \brief op
     * \param a
     * \param b
     * \return a op b
     */
    R op(T a, T b) {
        try
        {
            return _op(a,b);
        }
        catch(...)
        {
            return 0;
        }
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg.loader().addProp("DefaultB","DefaultB",ns.data().getValue<T>(p,"DefaultB"));
        dlg.loader().addBoolProperty("Use DefaultB","UseDefaultB",ns.data().getValue<bool>(p,"UseDefaultB")); // field[1]
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        T val = dlg.loader().get<T>(PropField1);
        ns.data().setValue(p,"DefaultB",val);
        ns.data().setValue(p,"UseDefaultB",dlg.loader().get<bool>(PropField2));
    }
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg->addProp("Use DefaultB",ns.data().getValue<bool>(p,"UseDefaultB"));
        dlg->addProp("DefaultB",ns.data().getValue<T>(p,"DefaultB"));
    }

    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        T v;
        dlg->get(NODEFLOW::PropField1,v);
        ns.data().setValue(p,"DefaultB",v);
        ns.data().setValue(p,"UseDefaultB",dlg->getBool(PropField2));
    }



};



template <typename T,  ConnectionType inType = Any>
/*!
 * \brief The BinaryRelationalTypeNode class
 * Does a relational operation and routes the packet based on the result to the true or false outputs
 */
class BinaryRelationalTypeNode : public NodeType // two inputs one output
{
public:
    typedef std::function<bool (T,T)> BINARY_REL_OPFUNC;
    enum
    {
        InputA = 0,
        InputB,
        //
        TrueOutput = 0, // packet route if true
        Output = 1, // result
        FalseOutput = 2
    };

    BINARY_REL_OPFUNC _op;
    ConnectionType _inType = inType;


public:
    /*!
     * \brief BinaryRelationalTypeNode
     * \param s
     * \param f
     */
    BinaryRelationalTypeNode(const std::string s, BINARY_REL_OPFUNC f) : NodeType(s),_op(f)
    {

    }
    virtual const char * nodeClass() const {
        return "Relation Operators";
    }
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(2);
        //
        inputs()[0] = Connection("A",Single,inType);
        inputs()[1] = Connection("B",Single,inType);
        //
        // set up the outputs
        outputs().resize(3);
        outputs()[0] = Connection("true",Multiple,inType);
        outputs()[1] = Connection("out",Multiple,Bool);
        outputs()[2] = Connection("false",Multiple,inType);
    }
    /*!
     * \brief start
     * \param n
     */
    virtual void start(NodeSet &ns,  NodePtr &n)
    {
        n->data()["A"] = T(0);
        n->data()["OUT"] = T(0);
        MRL::PropertyPath p;
        n->toPath(p);
        n->data()["HaveA"] = false;
        n->data()["HaveB"] = ns.data().getValue<bool>(p,"UseDefaultB");
        n->data()["B"] =  ns.data().getValue<T>(p,"DefaultB");;

    }

    /*!
     * \brief process
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return true on success
     */
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            bool haveA = n->data()["HaveA"].asBool();
            bool haveB = n->data()["HaveB"].asBool();
            switch(id)
            {
            case InputA:
                n->data()["A"] = data[DATA_PAYLOAD].as<T>();
                haveA = true;
                n->data()["HaveA"] = haveA;
                break;
            case InputB:
                n->data()["B"] = data[DATA_PAYLOAD].as<T>();
                haveB = true;
                n->data()["HaveB"] = haveB;
                break;
            default:
                break;
            }
            //
            if(haveA && haveB)
            {
                bool r = op(n->data()["A"].as<T>(), n->data()["B"].as<T>());
                VALUE result;
                setValueData(data,r,result);
                post(ns,nodeId,r?TrueOutput:FalseOutput,data); // route the packet
                return post(ns,nodeId,Output,result); // output the logical result
            }
        }
        return false;
    }

    /*!
     * \brief evaluate
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return
     */
    bool evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data )
    {
        NodePtr  &n = ns.findNode(nodeId);
        if(n && n->enabled() && (id == Output))
        {
            VALUE a;
            VALUE b;
            if(calculate(ns,nodeId,InputA,a) && calculate(ns,nodeId,InputB,b))
            {
                bool r = op(a[DATA_PAYLOAD].as<T>(),b.as<T>()) ;
                data[DATA_PAYLOAD] = r;
            }
        }
        return false;
    }

    /*!
     * \brief op
     * \param a
     * \param b
     * \return a op b
     */
    bool op(T a, T b) {
        try
        {
            return _op(a,b);
        }
        catch(...)
        {
            return 0;
        }
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg.loader().addProp("DefaultB","DefaultB",ns.data().getValue<T>(p,"DefaultB"));
        dlg.loader().addBoolProperty("Use DefaultB","UseDefaultB",ns.data().getValue<bool>(p,"UseDefaultB")); // field[1]
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        T val = dlg.loader().get<T>(PropField1);
        ns.data().setValue(p,"DefaultB",val);
        ns.data().setValue(p,"UseDefaultB",dlg.loader().get<bool>(PropField2));
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg->addProp("Use DefaultB",ns.data().getValue<bool>(p,"UseDefaultB"));
        dlg->addProp("DefaultB",ns.data().getValue<T>(p,"DefaultB"));
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        T v;
        dlg->get(NODEFLOW::PropField1,v);
        ns.data().setValue(p,"DefaultB",v);
        ns.data().setValue(p,"UseDefaultB",dlg->getBool(PropField2));
    }
};



//

template <typename T, typename R, ConnectionType inType = Any, ConnectionType outType = Any>
/*!
 * \brief The UnaryTypeNode class
 */
class UnaryTypeNode : public NodeType // two inputs one output
{
public:
    typedef std::function<R (T)>  UNARY_OPFUNC;
    enum
    {
        InputA = 0,
        Output = 0
    };

    UNARY_OPFUNC _op;


public:
    /*!
     * \brief UnaryTypeNode
     * \param s
     * \param f
     */
    UnaryTypeNode(const std::string s, UNARY_OPFUNC f) : NodeType(s),_op(f)
    {

    }
    virtual const char * nodeClass() const {
        return "Unary Operators";
    }
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        //
        inputs()[0] = Connection("in",Single,inType);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,outType);
    }
    /*!
     * \brief start
     * \param n
     */
    virtual void start(NodeSet &/*ns*/,  NodePtr &n)
    {
        n->data()["IN"] = T(0);
        n->data()["OUT"] = T(0);
    }


    /*!
     * \brief process
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return true on success
     */
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            switch(id)
            {
            case InputA:
            {
                R r = op(data[DATA_PAYLOAD].as<T>());
                n->data()["OUT"] = r;
                VALUE result;
                setValueData(data,r,result);
                return post(ns,nodeId,Output,result);
            }
            default:
                break;
            }
        }
        return false;
    }

    /*!
     * \brief evaluate
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return
     */
    bool evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data )
    {
        NodePtr  &n = ns.findNode(nodeId);
        if(n && n->enabled() && (id == Output))
        {
            VALUE a;
            if(calculate(ns,nodeId,InputA,a))
            {
                R r = op(a[DATA_PAYLOAD].as<T>()) ;
                data[DATA_PAYLOAD] = r;
                n->data()["OUT"] = r; // cache it
                return true;
            }
        }
        return false;
    }

    /*!
     * \brief op
     * \param a
     * \param b
     * \return a op b
     */
    T op(T a) {
        try
        {
            return _op(a);
        }
        catch(...)
        {
            return 0;
        }
    }
};

/*!
 * \brief The TopicChangeTypeNode class
 * Sets the topic for a packet
 */
class TopicChangeTypeNode : public NodeType // two inputs one output
{
public:
    enum
    {
        InputA = 0,
        Output = 0
    };



public:
    /*!
     * \brief TopicChangeTypeNode
     * \param s
     */
    TopicChangeTypeNode(const std::string s) : NodeType(s)
    {

    }
    /*!
     * \brief nodeClass
     * \return
     */
    virtual const char * nodeClass() const {
        return "Topic Changers";
    }
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        inputs()[0] = Connection("in",Multiple,Any);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,Any);
    }
    /*!
     * \brief start
     * \param n
     */
    virtual void start(NodeSet &ns,  NodePtr &n)
    {
        MRL::PropertyPath p;
        n->toPath(p);
        std::string t = ns.data().getValue<std::string>(p,"Topic");
        n->data()["TOPIC"] = t;
    }


    /*!
     * \brief process
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return true on success
     */
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            switch(id)
            {
            case InputA:
            {
                VALUE result = data;
                result[DATA_TOPIC] = n->data()["TOPIC"]; // change the topic
                return post(ns,nodeId,Output,result);
            }
            default:
                break;
            }
        }
        return false;
    }



    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg.loader().addStringProperty("Change Topic","Topic",ns.data().getValue<std::string>(p,"Topic")); // field[2]
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[PropField1]->GetValue();
        ns.data().setValue(p,"Topic",v.GetString().ToStdString());
    }

    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg->addStringProperty("Change Topic",ns.data().getValue<std::string>(p,"Topic"));
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        std::string v;
        dlg->get(NODEFLOW::PropField1,v);
        ns.data().setValue(p,"Topic",v);
    }

};

/*!
 * \brief The RbeTypeNode class
 * Report by exception
 */
class RbeTypeNode : public NodeType // two inputs one output
{
public:
    enum
    {
        InputA = 0,
        Output = 0
    };

public:
    /*!
     * \brief TopicChangeTypeNode
     * \param s
     */
    RbeTypeNode(const std::string s) : NodeType(s)  {    }
    /*!
     * \brief nodeClass
     * \return
     */
    virtual const char * nodeClass() const {
        return "Unary Operators";
    }

    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        inputs()[0] = Connection("in",Multiple,Any);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,Any);
    }


    /*!
     * \brief process
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return true on success
     */
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            switch(id)
            {
            case InputA:
            {
                if(data[DATA_PAYLOAD] != n->data()[DATA_PAYLOAD])
                {
                    n->data() = data;
                    return post(ns,nodeId,Output,data);
                }
            }
            default:
                break;
            }
        }
        return false;
    }
};



/*!
 * \brief The TopicFilterTypeNode class
 * Routes a packet based on the topic to either the match or reject outputs
 */
class TopicFilterTypeNode : public TopicChangeTypeNode // two inputs one output
{
public:
    enum
    {
        InputA = 0,
        MatchOutput = 0,
        RejectOutput = 1
    };

public:
    /*!
     * \brief TopicFilterTypeNode
     * \param s
     */
    TopicFilterTypeNode(const std::string s) : TopicChangeTypeNode(s)
    {

    }
    virtual const char * nodeClass() const {
        return "Topic";
    }
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        inputs()[0] = Connection("in",Multiple,Any);
        //
        // set up the outputs
        outputs().resize(2);
        outputs()[0] = Connection("match",Multiple,Any);
        outputs()[1] = Connection("reject",Multiple,Any);

    }

    /*!
     * \brief process
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return true on success
     */
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            switch(id)
            {
            case InputA:
            {
                VALUE result = data;
                result[DATA_TOPIC] = n->data()["TOPIC"]; // change the topic
                return post(ns,nodeId,(data[DATA_TOPIC] == n->data()["TOPIC"])?MatchOutput:RejectOutput,result); // route the packet based on
            }
            default:
                break;
            }
        }
        return false;
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg.loader().addStringProperty("Filter Topic","Topic",ns.data().getValue<std::string>(p,"Topic")); // field[2]
    }

    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[PropField1]->GetValue();
        ns.data().setValue(p,"Topic",v.GetString().ToStdString());
    }

    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg->addStringProperty("Filter Topic",ns.data().getValue<std::string>(p,"Topic"));
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        std::string v;
        dlg->get(NODEFLOW::PropField1,v);
        ns.data().setValue(p,"Topic",v);
    }

};



/*!
 * \brief The FunctionTypeNode class
 */
class FunctionTypeNode : public NodeType
{
protected:
    /*!
     * \brief The FunctionNode class
     */

    /*!
     * \brief The FunctionNodeInterface struct
     * The way Selene works requires this - references cannot be used to set up interfaces
     */
    struct FunctionNodeInterface
    {
        NodeSet * _set = nullptr;
        Node *    _node = nullptr;
        VALUE     _data;

        // Data accessors
        void clearData()
        {
            _data.clear();
        }
        //
        int getInt(std::string t)
        {
            return _data[t].asInt();
        }
        //
        double getDouble(std::string t)
        {
            return _data[t].asDouble();
        }
        //
        bool getBool(std::string t)
        {
            return _data[t].asBool();
        }
        //
        std::string getString(std::string t)
        {
            return _data[t].asString();
        }
        //
        void setInt(std::string t, int v)
        {
            _data[t] = v;
        }
        //
        void setDouble(std::string t, double v)
        {
            _data[t]= v;
        }
        //
        void setBool(std::string t, bool v)
        {
            _data[t] = v;
        }
        //
        void setString(std::string t, std::string s)
        {
            _data[t] = s;
        }

        //
        unsigned id()
        {
            return _node->id();
        }
        /*!
         * \brief post
         * \param ns
         * \param nodeId
         * \param id
         * \param tag - NB strings must be passed as std::string
         * \param v
         * \return
         */
        bool post( unsigned id)
        {
            return _set->post(_node->id(),id,_data);
        }


    };
    /*!
     * \brief The FunctionNode class
     */
    class FunctionNode : public Node
    {
        LUASCRIPTPTR _state;
        FunctionNodeInterface _interface;
    public:
        /*!
         * \brief FunctionNode
         * \param id
         * \param type
         */
        FunctionNode(unsigned id, unsigned type) : Node(id,type)
        {
            _state = std::make_unique<sel::State>(true);
        }
        /*!
         * \brief load
         * \param file
         */
        virtual void load(const NodeSet &ns, const std::string &script)
        {
            _state.reset();
            _state = std::make_unique<sel::State>(true);
            (*_state)(script.c_str()); // set the script
            _interface._node = this;
            _interface._set = const_cast<NodeSet *>(&ns);
            (*_state)["Node"].SetObj(_interface,
                                     "Id",&FunctionNodeInterface::id,
                                     "Post",&FunctionNodeInterface::post,
                                     "GetInt",&FunctionNodeInterface::getInt,
                                     "GetDouble",&FunctionNodeInterface::getDouble,
                                     "GetBool",&FunctionNodeInterface::getBool,
                                     "GetString",&FunctionNodeInterface::getString,
                                     "SetInt",&FunctionNodeInterface::setInt,
                                     "SetDouble",&FunctionNodeInterface::setDouble,
                                     "SetBool",&FunctionNodeInterface::setBool,
                                     "SetString",&FunctionNodeInterface::setString
                                    );

        }

        //
        VALUE & iData()
        {
            return _interface._data;
        }
        /*!
         * \brief state
         * \return
         */
        sel::State & state() {
            return  *_state;
        }
        /*!
         * \brief stateRef
         * \return
         */
        LUASCRIPTPTR &stateRef() {
            return _state;
        }

    };

public:
    /*!
     * \brief FunctionTypeNode
     * \param s
     */
    FunctionTypeNode(const std::string &s) : NodeType(s)
    {

    }

    virtual NODEFLOW::Node * create(unsigned i)
    {
        return new FunctionNode(i,id());
    }

    /*!
     * \brief nodeClass
     * \return
     */
    virtual const char * nodeClass() const {
        return "Function";
    }


    /*!
     * \brief start
     */
    virtual void start(NodeSet &ns,  NodePtr &node)
    {
        try
        {
            if(node)
            {
                MRL::PropertyPath p;
                node->toPath(p);
                std::string f = ns.data().getValue<std::string>(p,"Script");
                FunctionNode *n = static_cast<FunctionNode *>(node.get());
                n->load(ns,f);
                LUASCRIPTPTR &sr =  n->stateRef();
                ns.setupLuaApi(sr); // access node set functions
            }
        }
        CATCH_DEF
    }

    /*!
     * \brief process
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return
     */
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data);

    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        //
        inputs()[0] = Connection("in",Multiple,Any);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,Any);
    }


    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p);
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p);

};




}
#endif // FUNCTIONTYPENODE_H
