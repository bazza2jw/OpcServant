/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided with open62541.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef TRACE_HPP
#define TRACE_HPP
//
// wxLog does all the work
#include <wx/log.h>
#include <wx/debug.h>

//
// Function/file/line
//
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
//
// use wxLogDebug - change to suit GUI and app framework
#define TRC_LINE(s) { std::stringstream ss; ss << __FILE__ << ":" << __FUNCTION__ << "," << __LINE__ << "," << s ; wxLogDebug("%s",ss.str().c_str());}

#ifdef TRACE_ON
#define TRC(s) TRC_LINE(s)
#define TRACE_ENTRY TRC("Entry");
#define TRACE_EXIT  TRC("Exit");
#else
#define TRC(s)
#define TRACE_ENTRY
#define TRACE_EXIT
#endif

#ifdef TRAP_ENABLE
    #define TRAP_FUNC wxTrap();
#else
    #define TRAP_FUNC
#endif

#define EXCEPT_TRC std::cerr << __FILE__ <<  __FUNCTION__ << ":Exception Caught:" << e.what() << std::endl;
#define EXCEPT_DEF std::cerr << __FILE__ << __FUNCTION__ << ":Exception Caught:" << std::endl;


#define CATCH_DEF \
    catch (std::runtime_error &e) { EXCEPT_TRC;TRAP_FUNC} \
    catch (std::exception &e) { EXCEPT_TRC;TRAP_FUNC } \
    catch (...) {  EXCEPT_DEF; TRAP_FUNC }
// inserts breakpoint
#define THROW_DEF(s) { TRAP_FUNC; std::runtime_error e(__FUNCTION__ );    throw(e);};

//
#endif // TRACE_HPP
