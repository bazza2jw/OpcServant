cmake_minimum_required(VERSION 3.10)
set (MODULE NodeFlowSet)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    nodeflowplugin.cpp
    nodeflowobjectmanager.cpp
    nodeflowrt.cpp
    nodeflowtypeobject.cpp
    nodeflownodes.cpp
    NodeFlowConfiguration.cpp
    NodeFlowUI.cpp
    NodeFlowUI_nodeflow_bitmaps.cpp
    )
include_directories("/usr/local/NodeFlow/include")
link_directories("/usr/local/NodeFlow/lib")
add_definitions(-DUSE_WT)
set(PROJECT_LIB_FLAGS Common MrlLib NodeFlow ${NODEFLOWLIBS})
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
