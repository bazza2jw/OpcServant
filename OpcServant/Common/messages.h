/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef MESSAGES_H
#define MESSAGES_H
#include <MrlLib/variant.h>
#include <MrlLib/mrlmutex.h>
#include <queue>
#include <boost/config.hpp>
#include <boost/bimap.hpp>
#include <MrlLib/json/json.h>

namespace MRL {
//
// inter thread message
//
typedef boost::bimap< std::string, unsigned > mapping_bimap;
typedef mapping_bimap::value_type mapping_position;
//
/*!
     * \brief The MessageVariantMap class
     */
class MessageVariantMap : public std::map<int, Variant>
{
public:
    MessageVariantMap() {}
    MessageVariantMap(const MessageVariantMap &) = default;
    bool contains(int i) const {
        return find(i) != end(); // is item present
    }

    template <typename T>
    /*!
        \brief isType
        \param i
        \return
    */
    bool isType(int i)   {
        if (contains(i)) {
            return (*this)[i].type().hash_code() == typeid(T).hash_code();
        }
        return false;
    }

    template <typename T>
    /*!
        \brief add
        \param i
        \param v
    */
    void add(int i, const T &v) {
        // add to data set
        (*this)[i] = Variant(v);
    }
    template <typename T>
    /*!
        \brief get
        \param i
        \param v
        \return true on success
    */
    bool get(int i, T &v) const  {
        try {
            MessageVariantMap::const_iterator j = this->find(i);
            if(j != this->end())
            {
                const Variant &a = j->second;
                v = boost::get<T>(a);
                return true;
            }
        }
        catch (...) {

        }
        return false;
    }

    /*!
        \brief asString
        \param i
        \param s
        \return true on success
    */
    bool asString(int i, std::string &s) const  {
        try {
            MessageVariantMap::const_iterator j = this->find(i);
            if(j != this->end())
            {
                const Variant &a = j->second;
                s = toString(a); // return value as string
                return true;
            }
        }
        catch (...) {

        }
        return false;
    }
};

/*!
    \brief The MessageHandler class
*/

/*!
    \brief The Message class
*/
class Message {
    unsigned _id = 0;   // message id / op code
    unsigned _source = 0; // where the message comes from
    unsigned _destination = 0; // where it is to go to
    MessageVariantMap _data; // payload
    static mapping_bimap _messages; // map registered message name to id
    static mapping_bimap _parameters; // map of paramter names to parameter ids
    static int _messageId; // next message id
    static int _parameterId; // next parameter id
    static bool _initialised;

public:
    Message(unsigned i = 0, unsigned d = 0, unsigned s = 0) : _id(i), _source(s), _destination(d) {
    }


    Message(const Message &) = default;

    /*!
        \brief data
        \return
    */
    MessageVariantMap &data() {
        return _data;
    }

    /*!
        \brief id
        \return message  id
    */
    int id() const {
        return _id;
    }
    /*!
        \brief setId
        \param i
    */
    void setId(int i) {
        _id = i;
    }

    /*!
     * \brief source
     */
    unsigned source() const {
        return _source;    // where the message comes from
    }
    /*!
     * \brief destination
     */
    unsigned destination() const {
        return _destination;   // where it is to go to
    }

    void setSource(unsigned i) {
        _source = i;
    }
    void setDestination(unsigned i) {
        _destination = i;
    }
    //
    // Register message op codes
    // this is needed by interpreters
    //
    /*!
        \brief registerMessage
        \param n name of message
        \param v message id - zero if auto assign
        \return message id
    */
    static unsigned registerMessage(const std::string &n, unsigned v = 0) {
        unsigned ret = 0;
        auto i = _messages.left.find(n);
        if (i != _messages.left.end()) {
            if (!ret) {
                ret = (v) ? v : _messageId++;
                _messages.insert(MRL::mapping_position(n, ret));
            }
        }
        return ret;
    }
    /*!
        \brief message
        \param s
        \return message id
    */
    static unsigned message(const std::string &s) {
        unsigned ret = 0;
        auto i = _messages.left.find(s);
        if (i != _messages.left.end()) {
            ret = i->second;
        }
        return ret;
    }

    /*!
        \brief message
        \param s
        \return
    */
    static  std::string  message(unsigned s) {
        auto i = _messages.right.find(s);
        if (i != _messages.right.end()) {
            return i->second;
        }
        return std::string();
    }

    /*!
        \brief registerParameter
        \param n
        \return parameter id code
    */
    static int registerParameter(const std::string &n, unsigned v = 0) {
        int ret = parameter(n);

        if (!ret)                {
            ret = (v) ? v : ++_parameterId;
            _parameters.insert(MRL::mapping_position(n, v));
        }
        return ret;
    }
    /*!
        \brief message
        \param s
        \return parameter id or zero
    */
    static int parameter(const std::string &s) {
        int ret = 0;
        auto i = _parameters.left.find(s);
        if (i != _parameters.left.end()) {
            ret = i->second;
        }
        return ret;
    }

    /*!
        \brief parameter
        \param s
        \return
    */
    static std::string  parameter(int s) {
        auto i = _parameters.right.find(s);
        if (i != _parameters.right.end()) {
            return i->second;
        }
        return std::string();
    }

    /*!
        \brief toJson
        \param v
        \return true on success
    */
    bool toJson(Json::Value &v); // convert a message to JSON

    /*!
        \brief fromJson
        \param v
        \return true on success
    */
    bool fromJson(Json::Value &v); // convert from JSON to a message


    static void initialise();

};

typedef std::unique_ptr<Message> MessageRef; // unique pointer references
/*!
    \brief The MessageQueue class
    Assumes many writers and one reader
    Thread safe
*/
class MessageQueue : protected std::queue<MessageRef> { // queue of messages
    ReadWriteMutex _mutex; // locking mutex
    size_t _limit = 1000;  //  maximum queue length - guard against slow / dead thread
public:
    MessageQueue() {}

    /*!
        \brief setLimit
        \param l
    */
    void setLimit(size_t l) {
        _limit = l;
    }

    /*!
        \brief full
        \return true
    */
    bool full() {
        ReadLock l(_mutex);
        return size() >= _limit; // callers can use as a hint to wait until the queue is empty
    }

    /*!
        \brief add
        \param r
        \return true on success
    */
    bool add(MessageRef &r) {
        if (r.get()) { // avoid empty messages
            WriteLock l(_mutex);
            if (size() < _limit) {
                push(std::move(r)); // move the item to the end of the queue
                return true;
            }
        }
        return false;
    }


    /*!
        \brief next
        \return pointer to message or null
    */
    Message *next() {
        ReadLock l(_mutex);
        if (!empty()) {
            return front().get(); // requires there is only one message reader
        }
        return nullptr;
    }
    /*!
        \brief remove
    */
    void remove() {
        WriteLock l(_mutex);
        if (!empty()) {
            pop();
        }
    }

    /*!
        \brief hasMessage
        \return if there is a message waiting
    */
    bool hasMessage() {
        ReadLock l(_mutex);
        return !empty();
    }

    /*!
        \brief clear
    */
    void clear() {
        WriteLock l(_mutex);
        while (!empty()) pop();
    }
};

template <typename T>
/*!
 * \brief The MessageHandler class
 */
class MessageHandler {
public:
    MessageHandler() {}
    virtual ~MessageHandler() {}
    virtual void action(const Message &, T &) {}
    typedef std::unique_ptr<MessageHandler<T> > Ref;
    typedef std::map<unsigned, Ref > Map;
};

}


#endif // MESSAGES_H
