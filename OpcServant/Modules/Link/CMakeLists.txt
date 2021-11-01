cmake_minimum_required(VERSION 3.10)
set (MODULE Link)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    Linkplugin.cpp
    Linkobjectmanager.cpp
    Linktypeobject.cpp
    Linkrt.cpp
    LinkSetupDialog.cpp
    LinkUI_Link_bitmaps.cpp
    Linkwebsetupdialog.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
