
 #* Copyright (C) 2018 -  B. J. Hill
 #*
 #* This file is part of OpcServant. OpcServant C++ classes are free software: you can
 #* redistribute it and/or modify it under the terms of the Mozilla Public
 #* License v2.0 as stated in the LICENSE file
 #*
 #* These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 #* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 #* A PARTICULAR PURPOSE.
#
#
cmake_minimum_required(VERSION 3.10)
project(OpcServant)
include (Common.cmake)
message("MRLLIB")
add_subdirectory(MrlLib)
message("ReportDocument")
add_subdirectory(reportdocument)
message("Common")
add_subdirectory(Common)
message("NodeFlow")
add_subdirectory(NodeFlow)
message("Mimic")
add_subdirectory(Mimic)
message("Modules")
add_subdirectory(Modules)
message("Servant")
add_subdirectory(Servant)
# The test harness for UI components etc - enable as required
#add_subdirectory(Harness)
#


