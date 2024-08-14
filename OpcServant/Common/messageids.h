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
#ifndef MESSAGEIDS_H
#define MESSAGEIDS_H
namespace MESSAGEID
{
    // the stock message ids
    // these are messages to the thread
    enum
    {
        None = 0,
        //
        // General messages
        Update_Object = 1, // Object has been updated (measured) in DAQ space - OPC and GUI/Web need to sync to the object
        Update_Value,      // a value has been updated
        Publish_Object,    // Object requests publication
        Publish_Record, // publish a record - expect JSON or CSV
        Log_Event, // Object has notified of event - typically in and out of fault
        Diagnostic_Event, // an object has emitted a diagnostic (trace)
        Audit_Event, // some security related event has happend - login/out or config update
        Add_Object, // object added to configuration
        Remove_Object, // object removed from configuration
        Edit_Object, // object configuration has been editted (persitent changes) - not the same as an OPC write
        Started, // object has started
        Stopped, // object has stopped
        //
        // View creation
        CreateTabView,
        CreateTopView,
        // Delete a tab view (if any) and restart it
        ReloadTabView,
        //
        // Define the stock DAQ ids - these are mesages to the DAQ thread
        Daq_Write_Value = 0x100, // set a value in an object - OPC write value
        Daq_Exec_Method, // get object to exec method
        Daq_Start, // request to start
        Daq_Stop, // request to stop
        Daq_Restart, // restart the DAQ process
        Daq_Running, // the DAQ thread has completed its start - hint for GUI and web to show
        Daq_Purge, // purge the local database
        //
        // Define the stock OPC messages - messages to the OPC thread
        //
        Opc_Update_Object = 0x200, // sync OPC with DAQ
        Opc_Clear,
        Opc_Restart, // clear the OPC Object section and reload from configuration
        //
        ObjectStart, // start an object
        ObjectStop,  // stop an object
        // Define the stock GUI / Web messages - should be the same
        //
        Gui_Update = 0x300, // sync with data view
        //
        SetOutput,
        IdleTimeout, // mouse / key inactivity event
        PowerOff, // power of has been selected - clean up and shutdown
        // Base of non-stock messages
        Last = 0x1000
    };

}
#endif // MESSAGEIDS_H
