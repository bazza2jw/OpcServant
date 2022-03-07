cmake_minimum_required(VERSION 3.10)
set (MODULE OpcClient)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    opcclientplugin.cpp
    OpcClientConfigurationDialog.cpp
    OpcClientUI.cpp
    OpcClientUI_opcclient_bitmaps.cpp
    opcclientobjectmanger.cpp
    opcclientrt.cpp
    ClientBrowserDialog.cpp
    opcclienttypeobject.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
