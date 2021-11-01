cmake_minimum_required(VERSION 3.10)
set (MODULE SbcSupport)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
set(LIB_SOURCES
    sbcsupport.cpp
    SbcSupportUI_sbcsupport_bitmaps.cpp
    SbcSupportSetupDialog.cpp
    SbcSupportUI.cpp 
    sbcsupportobjectmanager.cpp 
    sbcsupportrt.cpp
    SbcSupportTabPanel.cpp
    SetClock.cpp
)

set(PROJECT_LIB_FLAGS Common MrlLib)
add_library(${MODULE} SHARED ${LIB_SOURCES})

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
