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
#include "VisualCalcrt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#ifdef RASPBERRY_PI_BUILD
    #include "pigpio.h"
#endif




/*!
    \brief start
*/
void MRL::VisualCalcRT::start()
{
    //
    RTObject::start();
     _state = STATE_START;

}

/*!
    \brief stop
*/
void MRL::VisualCalcRT::stop()
{
    RTObject::stop();
    _state = STATE_IDLE;

}

/*!
    \brief publish
*/
void MRL::VisualCalcRT::publish()
{
    RTObject::publish();
}

/*!
    \brief measure
*/
void MRL::VisualCalcRT::measure()
{
    RTObject::measure();
}

/*!
 * \brief MRL::VisualCalcRT::onOneSecond
 * \param t
 * fetch each input
 */
void MRL::VisualCalcRT::onOneSecond(time_t t)
{
    RTObject::onOneSecond(t);
}

/*!
 * \brief MRL::VisualCalcRT::process
 */
void MRL::VisualCalcRT::process()
{
   RTObject::process();
   // drive any timers
}

bool MRL::VisualCalcRT::processQueueItem(const Message &m)
{
    if(!RTObject::processQueueItem(m))
    {
        //
    }
    return true;
}

/*!
 * \brief MRL::VisualCalcRT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::VisualCalcRT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/)
{
    // sync with the local tree
}


/*!
 * \brief MRL::VisualCalcRT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::VisualCalcRT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
    // set up the browse tree
    _tree = std::make_unique<Open62541::ServerNodeTree>(server,node,server.applicationNamespace());
    server.browseTree(node,*_tree); // set up the tree

}


/*!
 * \brief publishHtmlPage
 */
void MRL::VisualCalcRT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
    //
}

/*!
 * \brief measureHtmlPage
 */
void MRL::VisualCalcRT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
}
