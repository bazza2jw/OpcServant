
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
add_subdirectory(MrlLib)
add_subdirectory(reportdocument)
add_subdirectory(Common)
add_subdirectory(NodeFlow)
add_subdirectory(Mimic)
add_subdirectory(Modules)

add_subdirectory(Servant)
# The test harness for UI components etc - enable as required
add_subdirectory(Harness)
#


